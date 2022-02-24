# RayTracing
Inspired by Ray Tracing In One Weekend

A little side project I did inspired by this:

https://raytracing.github.io/books/RayTracingInOneWeekend.html

Generate a nice scene using raytracing math. Here's an example:

![lol](https://user-images.githubusercontent.com/69275171/155463202-93d66f35-3458-45b7-aa9a-a05b76b96ba8.jpg)

# Running

Build the solution, run the solution and pipe to an .ppm file like this:

```
.\RayTracing.exe > image.ppm
```

Then open .ppm in a image processing program like Photoshop

# Things I added to the base program:

- Multithreading capability using std::thread

# Todo:

- Thread pools
- Render out a pixel at a time using some graphics library instead of piping it to a file and opening in photoshop 
