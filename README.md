# Generate a nice scene using raytracing math

A little side project I did inspired by this:

![stuff](https://images-na.ssl-images-amazon.com/images/I/414m4ETx0AL._SX342_SY445_QL70_ML2_.jpg)

https://raytracing.github.io/books/RayTracingInOneWeekend.html

 Here's an example:

![image](https://user-images.githubusercontent.com/69275171/156908085-4ffbeebf-ffff-44de-84fe-42eab716bdc3.png)

**Raytracing** is when you draw a scene by simulating light. You first make a scene with a bunch of objects, each of which has a certain **material**. The material specifies the color of the object as well as the type of interaction that light should have with it (is it a metal, a lambertian, a dielectric, etc.)

Next you place your position (called the camera) at a certain point, and then using a for loop, you emit light particles onto every coordinate point (x,y) on a big grid. Then using vector math, you let the light particles bounce a certain number of times, taking note of the color of the materials that it bumps into each time. Finally, taking all your collisions into account, you will assign that point on the grid a certain color value.

What you get is a 3D scene, which is rendered on a 2D plane of course. All generated for us by computer math!

# Running

Build the solution in Visual Studio 2019, then Run the solution. You will see a black screen and a UI. Configure the settings that you want (Anti Aliasing Depth + Child Ray Depth, number of threads to Use). 

Then just press "Start Render" which will start rendering the image.

https://user-images.githubusercontent.com/69275171/156908469-10fdfcbd-effe-4d7b-bcab-7a2e4c6a549b.mp4

Also note that this program will probably make your CPU heat up, since by default its set to max number of threads. 

![image](https://user-images.githubusercontent.com/69275171/156908519-4398e2d4-d397-4567-8e5f-51d15f8d9b54.png)

If you don't want that, just turn the number of threads down.

# Some things to note:

1. This project uses C++17, so make sure you change that in (Right Click) Properties->General->C++ Language Standard, and set that to C++17.

2. This project uses d3d11.lib, aka DirectX11. I added a pragma comment so it would work, but in case it doesn't, this means you need to add that library. Go to Linker->Input->Additional Dependencies then just type "d3d11.lib" next to all the other libraries already present.

3. As with most visual studio projects, Release mode runs substantially faster than Debug mode. So try to run it in Release.

# Things I added to the base program:

- Multithreading capability using std::thread
- Thread Pools to speed up multithreading even more (ThreadPool.h)
- [Dear ImGUI](https://github.com/ocornut/imgui) integration for easy configuration and nice UI
- Real-time statistics features (fps, pixels/sec)
- Rendering an image in realtime using DirectX11 API (mostly handled by [this wonderful example](https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#about-imtextureid) by ImGUI, thankfully). The steps to do this involve storing a array in memory of the pixel values, then each frame, rendering that buffer into a DirectX11 texture (ID3D11ShaderResourceView), then finally using ImGui::Image() to call the [DirectX11 imgui backend](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_dx11.cpp)
- A bit of Windows API stuff for window handle, window creation + message system. Again I mostly just read the example on how to do this.

# Thanks 

To [Valerio Formato](https://github.com/valerioformato) + [The Cherno](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw) for [this repo](https://github.com/valerioformato/RTIAW), and [this video](https://www.youtube.com/watch?v=mOSirVeP5lo), respectively. I drew a lot of inspiration (perhaps too much inspiration) from them. I learned a ton about multithreading, and modern C++ practices that I otherwise wouldn't have.

And of course, to the author of Ray Tracing In a Weekend, [Peter Shirley](https://github.com/petershirley).
