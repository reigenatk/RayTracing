#pragma once

#pragma comment(lib, "d3d11.lib")
#include <functional>
#include <stdexcept>
#include <tchar.h>
#include <memory>

#include <d3d11.h>
#include "imgui.h"
#include "Renderer.h"


namespace App {
	class AppWindow
	{
	public:


		AppWindow(ImVec2 pos, ImVec2 windowSize);
		~AppWindow();
		void CreateImageBuffer();
		void CreateRenderTarget();
		bool CreateDeviceD3D();
		void CleanupDeviceD3D();
		void CleanupRenderTarget();
		void ResizeWindow(const ImVec2 newSize);


		// main loop
		void Run();

		ImVec2 m_windowSize;

		// a pointer to our Renderer Object
		std::shared_ptr<Renderer> m_renderer;

		// our internal representation of the rendered image
		std::unique_ptr<uint8_t[]> m_imageBuffer;

		// renders our image from byte array to GPU texture, so that ImGUI can render using ImGui::Image()
		[[nodiscard]] ID3D11ShaderResourceView* RenderToTexture();
		void UpdateTexturizedImage(ID3D11ShaderResourceView* srv); // runs once a loop of AppWindow::Run()

		// win32 stuff about the open window + its handle
		HWND m_hwnd; // handle to the window object
		LONG m_hwndDefaultStyle;
		bool m_hwndResizeDisabled = false;
		bool m_resized = false;
		WNDCLASSEX wc; // A windows class structure.

		// D3D backend stuff, generated in CreateRenderTarget() and CreateDevice3D() in AppWindow constructor
		ID3D11Device* m_pd3dDevice = nullptr;
		ID3D11DeviceContext* m_pd3dDeviceContext = nullptr;
		IDXGISwapChain* m_pSwapChain = nullptr;
		ID3D11RenderTargetView* m_mainRenderTargetView = nullptr;



		// windows message handler? static cuz its independent of the window class
		static LRESULT WINAPI WndMsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// get size of window
		// size_t = unsigned __int64
		[[nodiscard]] size_t BufferSize() const {
			return static_cast<size_t>(m_windowSize.x) * static_cast<size_t>(m_windowSize.y) * 4l;
		}

	};
}

