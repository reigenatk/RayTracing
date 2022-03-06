# RayTracing

A little side project I did inspired by this:

https://raytracing.github.io/books/RayTracingInOneWeekend.html

Generate a nice scene using raytracing math. Here's an example:

![lol](https://user-images.githubusercontent.com/69275171/155463202-93d66f35-3458-45b7-aa9a-a05b76b96ba8.jpg)

**Raytracing** is when you draw a scene by simulating light. You first make a scene with a bunch of objects, each of which has a certain **material**. The material specifies the color of the object as well as the type of interaction that light should have with it (is it a metal, a lambertian, a dielectric, etc.)

Next you place your position (called the camera) at a certain point, and then using a for loop, you emit light particles onto every coordinate point (x,y) on a big grid. Then using vector math, you let the light particles bounce a certain number of times, taking note of the color of the materials that it bumps into each time. Finally, taking all your collisions into account, you will assign that point on the grid a certain color value.

What you get is a 3D scene, which is rendered on a 2D plane of course. All generated for us by computer math!

# Running

Build the solution in Visual Studio 2019, then configure the settings that you want (Anti Aliasing Depth + Child Ray Depth, number of threads to Use)

Finally, just press "Start Render" which will start rendering the image.



# Things I added to the base program:

- Multithreading capability using std::thread
- Thread Pools to speed up multithreading even more (ThreadPool.h)
- [Dear ImGUI](https://github.com/ocornut/imgui) integration for easy configuration and nice UI
- Real-time statistics features (fps, pixels/sec)

