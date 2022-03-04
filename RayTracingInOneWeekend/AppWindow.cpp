#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <string>

#include "AppWindow.h"

// Forward declare message handler from imgui_impl_win32.cpp
// Still not sure why this is necessary for this func and not for other IMGUI funcs 
// that we are calling.. but ok
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace App {
    AppWindow::AppWindow(ImVec2 pos, ImVec2 windowSize) : m_windowSize{ windowSize }, m_renderer{ std::make_shared<Renderer>() } {
        // Win32/D3D setup:
        wc = { sizeof(WNDCLASSEX),
              CS_CLASSDC,
              &AppWindow::WndMsgHandler,
              0L,
              0L,
              GetModuleHandle(nullptr),
              nullptr,
              nullptr,
              nullptr,
              nullptr,
              _T("ImGui Example"),
              nullptr };

        // Create application window
        // ImGui_ImplWin32_EnableDpiAwareness();
        ::RegisterClassEx(&wc);

        // call CreateWindow API function, which returns a handle to the window
        // the stuff we're passing in here is like window size, position, window title, etc.
        m_hwnd = ::CreateWindow(wc.lpszClassName, _T("Raytracing in a weekend!"), WS_OVERLAPPEDWINDOW,
            static_cast<UINT>(pos.x), static_cast<UINT>(pos.y), static_cast<UINT>(windowSize.x),
            static_cast<UINT>(windowSize.y), nullptr, nullptr, wc.hInstance, this);

        CreateImageBuffer();

        // Initialize Direct3D
        if (!CreateDeviceD3D()) {
            CleanupDeviceD3D();
            ::UnregisterClass(wc.lpszClassName, wc.hInstance);
            throw std::runtime_error("Cannot initialize Direct3D device");
        }

        // Show the window
        ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
        ::UpdateWindow(m_hwnd);

        // Imgui setup:
        //
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(m_hwnd);
        ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext);

        // Set targets for Renderer
        m_renderer->SetTargetBuffer(m_imageBuffer.get());
        m_renderer->SetWidthAndHeight((unsigned int)m_windowSize.x, (unsigned int)m_windowSize.y);
    }



    // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#about-imtextureid
    // followed D3D11 example closely from above link
    ID3D11ShaderResourceView* AppWindow::RenderToTexture() {
        // our return value
        D3D11_TEXTURE2D_DESC desc{};
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = static_cast<UINT>(m_windowSize.x);
        desc.Height = static_cast<UINT>(m_windowSize.y);
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        ID3D11Texture2D* pTexture = nullptr;
        D3D11_SUBRESOURCE_DATA subResource{};
        subResource.pSysMem = m_imageBuffer.get();
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        m_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
        if (!pTexture) {
            throw std::runtime_error("Could not initialize image buffer texture");
        }

        ID3D11ShaderResourceView* out_srv;

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        m_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
        pTexture->Release();

        return out_srv;
    }

    void AppWindow::UpdateTexturizedImage(ID3D11ShaderResourceView* srv)  {
        ID3D11Resource* res;
        srv->GetResource(&res);
        D3D11_MAPPED_SUBRESOURCE mres;
        m_pd3dDeviceContext->Map(res, 0, D3D11_MAP_WRITE_DISCARD, 0, &mres);

        if (const auto width = static_cast<unsigned int>(m_windowSize.x); mres.RowPitch == width * 4) {
            // we do * sizeof(uint8_t) here because we want # of bytes, and each 
            // pixel is 1 byte since its unsigned char. So really this is not necessary.
            memcpy(mres.pData, m_imageBuffer.get(), BufferSize() * sizeof(uint8_t));
        }
        else {
            uint8_t* src = m_imageBuffer.get(), * dest = reinterpret_cast<uint8_t*>(mres.pData);
            for (unsigned int iy = 0; iy < m_windowSize.y; ++iy) {
                memcpy(dest, src, width * 4 * sizeof(uint8_t));
                dest += mres.RowPitch;
                src += width * 4;
            }
        }

        m_pd3dDeviceContext->Unmap(res, 0);
    }

    void AppWindow::Run() {

        // setup initial textures
        ID3D11ShaderResourceView* texturized_image = RenderToTexture();
        bool done = false;
        while (!done) {
            MSG msg;
            while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                if (m_resized) {
                    texturized_image = RenderToTexture();
                    m_resized = false;
                }
                if (msg.message == WM_QUIT) {
                    done = true;
                }
            }
            if (done)
                break;

            // Start the Dear ImGui frame
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // Here is where we actually output stuff to the window

            // updates the texture
            UpdateTexturizedImage(texturized_image);

            // this window will hold our image only
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            // info about window? 
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::Begin("Ray Tracing...", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
            ImGui::Image(static_cast<void*>(texturized_image), m_windowSize);
            ImGui::End();
            ImGui::PopStyleVar(2);

            // make another window that holds the controls
            ImGui::SetNextWindowSize(ImVec2{ 300, 200 });
            ImGui::Begin("Controls");
            int numThreads = m_renderer->getNumThreads();
            if (numThreads != -1) {
                ImGui::Text(("Number of Threads: " + std::to_string(numThreads)).c_str());
            }

            auto RendererState = m_renderer->getState();

            // disable the Start button if the renderer is already running! We know start button has been pressed already
            // by just examining state of the renderer using getState()
            if (RendererState == Renderer::Running) {
                ImGui::BeginDisabled();
            }
            // simple start Button for renderer
            if (ImGui::Button("Start Render", ImVec2(100,50))) {
                // if we get here, button was clicked
                memset(m_imageBuffer.get(), 0, BufferSize());
                m_renderer->StartRenderThread();
            }
            if (RendererState == Renderer::Running) {
                ImGui::EndDisabled();
            }
            ImGui::End();

            // Rendering (standard stuff)
            ImGui::Render();
            m_pd3dDeviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, nullptr);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            m_pSwapChain->Present(1, 0); // Present with vsync
        }
    }

    AppWindow::~AppWindow() {
        // Cleanup
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
        ::DestroyWindow(m_hwnd);
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    }

    bool AppWindow::CreateDeviceD3D()
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = m_hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

        if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
            createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, 
            &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext) != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    void AppWindow::CreateRenderTarget() {
        ID3D11Texture2D* pBackBuffer = nullptr;
        m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if (!pBackBuffer) {
            throw std::runtime_error("Could not create Imgui render buffer");
        }
        m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void AppWindow::CleanupDeviceD3D() {
        CleanupRenderTarget();
        if (m_pSwapChain) {
            m_pSwapChain->Release();
            m_pSwapChain = nullptr;
        }
        if (m_pd3dDeviceContext) {
            m_pd3dDeviceContext->Release();
            m_pd3dDeviceContext = nullptr;
        }
        if (m_pd3dDevice) {
            m_pd3dDevice->Release();
            m_pd3dDevice = nullptr;
        }
    }

    void AppWindow::CleanupRenderTarget() {
        if (m_mainRenderTargetView) {
            m_mainRenderTargetView->Release();
            m_mainRenderTargetView = nullptr;
        }
    }


    void AppWindow::CreateImageBuffer() {
        // unique ptr takes in a pointer as an argument
        auto newBuffer = std::unique_ptr<uint8_t[]>(new uint8_t[BufferSize()]());
        m_imageBuffer.swap(newBuffer);
    }

    //void AppWindow::ResizeWindow(const ImVec2 newSize) {
    //    m_windowSize = newSize;
    //    CreateImageBuffer();
    //    m_renderer->SetImageSize(static_cast<unsigned int>(m_windowSize.x), static_cast<unsigned int>(m_windowSize.y));
    //    m_renderer->SetTargetBuffer(m_imageBuffer.get());
    //}

    // Win32 message handler
    LRESULT WINAPI AppWindow::WndMsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        AppWindow* pThis = nullptr;
        switch (msg) {
        case WM_NCCREATE:
            pThis = static_cast<AppWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
            SetLastError(0);
            if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis))) {
                if (GetLastError() != 0)
                    return false;
            }
            break;
        case WM_SIZE:
            pThis = reinterpret_cast<AppWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

            if (pThis->m_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED) {
                const ImVec2 newSize{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) };
                pThis->CleanupRenderTarget();
                pThis->m_pSwapChain->ResizeBuffers(0, static_cast<UINT>(newSize.x), static_cast<UINT>(newSize.y),
                    DXGI_FORMAT_UNKNOWN, 0);
                pThis->CreateRenderTarget();
                //pThis->ResizeWindow(newSize);
                //pThis->m_resized = true;
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        default:
            break;
        }

        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
}