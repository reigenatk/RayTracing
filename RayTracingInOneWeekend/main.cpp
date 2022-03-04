
//#include <iostream>
//#include <vector>
//#include <thread>
//#include "constants.h"
//#include "hittable_list.h"
//#include "sphere.h"
//#include "color.h"
//#include "camera.h"
//#include "material.h"
//#include <chrono>

#include "AppWindow.h"

using namespace std::chrono;
/*
// checks if the ray r will intersect with the sphere centered at point (center) with radius (radius)
// returns -1 if no solutions, otherwise, returns the solution with negative discriminant
double hit_sphere(const point3& center, double radius, const ray& r) {
    // ax^2+bx+c, solves for t in P(t) = A + tb
    auto a = dot(r.direction(), r.direction());
    auto b = 2 * dot(r.origin() - center, r.direction());
    auto c = dot(r.origin() - center, r.origin() - center) - radius * radius;

    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}*/

//color ray_color(const ray& r) {
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = 0.5 * (unit_direction.y() + 1.0);
//    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//}

/*
color ray_color(const ray& r) {
    // check which value of t will make this ray land on the sphere (anywhere on it, not just border)
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);

    // if there is a t value that can land us on the sphere
    if (t > 0.0) {
        // getting a vector perpendicular to point of intersection:
        // formula = vector from point to center - vector from eye to center.
        vec3 to_circle = r.at(t);
        // kinda get this but not rly lol, but it generate a normal protruding
        // from the sphere at the point of intersection
        vec3 normal = to_circle - vec3(0, 0, -1); 
        vec3 N = unit_vector(normal);
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    // y ranges from -1 to 1 because height = 2
    // so we add to get 0 to 2 instead, and then scale by 1/2 so its 0 to 1.
     t = 0.5 * (unit_direction.y() + 1.0);
    // blue color if t is close to 1, white color if near 0.
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}*/



// start of old working

//color ray_color(ray &r, hittable_list &list, int depth) {
//    hit_record hrecord;
//
//    if (depth <= 0) {
//        return color(0, 0, 0);
//    }
//
//    /*
//    // this is just copy of code above but formatted nicer
//    // if it hit anything in the world
//    // use t_min of 0.001 to avoid super small value of t (shadow acne problem)
//    if (list.hit(r, 0.001, inf, hrecord)) {
//        // diffuse (random light reflection). Add the normal onto the point
//        // and then find a random point inside the unit sphere at the new point.
//        // this is the effect we will use.
//        point3 target = hrecord.p + hrecord.normal + randomInUnitSphere();
//
//        // return 0.5 * color(hrecord.normal + color(1, 1, 1));
//
//        // use this instead of the norm (its offset by a little bit)
//        // we do target - hrecord.p, because that is the formula to get 
//        // this new offset normal vector's value (section 8.1)
//        ray newnorm(hrecord.p, target - hrecord.p);
//        return 0.5 * ray_color(newnorm, list, depth-1);
//    }*/
//    
//
//    if (list.hit(r, 0.001, inf, hrecord)) {
//        color attenuation;
//        ray scatter;
//
//        // this call will populate scattered with the right ray to consider
//        if (hrecord.hit_material->scatter(r, hrecord, attenuation, scatter)) {
//            return attenuation * ray_color(scatter, list, depth - 1);
//        }
//        else {
//            return color(0, 0, 0);
//        }
//    }
//
//    // if it hits nothing. Basically the color of empty space.
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = 0.5 * (unit_direction.y() + 1.0);
//    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//}
//
//
//hittable_list random_scene() {
//    hittable_list world;
//    auto ground_material = make_shared<Lambertian>(color(0.75, 0.75, 0.75));
//    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
//
//    for (int a = -8; a < 8; a++) {
//        for (int b = -8; b < 8; b++) {
//            auto choose_mat = random_double();
//            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
//
//            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
//                shared_ptr<Material> sphere_material;
//
//                if (choose_mat < 0.5) {
//                    // diffuse
//                    auto albedo = color::random() * color::random();
//                    sphere_material = make_shared<Lambertian>(albedo);
//                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                }
//                else {
//                    // metal
//                    auto albedo = color::random(0.2, 1);
//                    auto fuzz = random_double(0, 1);
//                    sphere_material = make_shared<Metal>(albedo);
//                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                }
//                //else {
//                //    // glass
//                //    sphere_material = make_shared<dielectric>(1.5);
//                //    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                //}
//            }
//        }
//    }
//
//    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
//    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
//
//    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5));
//    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
//    return world;
//}
//
//// configuration
//double aspect_ratio = 3.0 / 2.0;
//int image_width = 1000;
//int image_height = static_cast<int>(image_width / aspect_ratio); // 200
//// for anti aliasing
//int samples_per_pixel = 80;
//
//// more 'accurate' the larger this number is, but will take longer
//int child_ray_depth = 25;
//hittable_list hl;
//
//// Args: Camera(double vertical_fov, double aspect_ratio, vec3 vup, 
//// point3 lookfrom, point3 lookat)
//Camera c = Camera(90.0, aspect_ratio, point3(0, 1, 0), point3(8, 2, 2), point3(0, 0, 0));
//
//auto outputVector = std::vector<std::vector<std::pair<double, std::pair<double, double>>>>();
//
//void handleRendering(int startheight, int endheight, int threadNo) {
//    std::thread::id threadID = std::this_thread::get_id();
//    for (int j = startheight; j >= endheight; --j) {
//        std::cerr << "Thread " << threadID << " has " << j - endheight + 1 << " Scanlines remaining" << '\n';
//        for (int i = 0; i < image_width; ++i) {
//            color pixel_color(0, 0, 0);
//            for (int k = 0; k < samples_per_pixel; k++) {
//                auto u = (i + random_double()) / (image_width - 1);
//                auto v = (j + random_double()) / (image_height - 1);
//                ray r = c.getRay(u, v);
//                // decide what color will be used
//                color color_at_target = ray_color(r, hl, child_ray_depth);
//
//                pixel_color += color_at_target;
//            }
//            // draw the color using ostream
//            // pixel color is accumulation of 100 random samples around the target area
//            // write color will divide this final sum by 100 to get a nice average value, which generates a good hue
//
//            // write_color(samples_per_pixel, std::cout, pixel_color, outputVector[threadNo]);
//        }
//    }
//}



int main() {
    App::AppWindow aw = App::AppWindow({ 700, 100 }, { 1000, 667 });
    aw.Run();

 //   auto start = high_resolution_clock::now();
 //   unsigned long const hardware_threads =
 //       std::thread::hardware_concurrency();
 //   std::cerr << "Hardware threads: " << hardware_threads << '\n';

 //   // following page 52 in multithreading book
 //   int min_per_thread = 600;
 //   int length = image_width * image_height;

 //   // if length = 0, this returns 0. Otherwise, 1-min_perthread will use 1 thread,
 //   // min_perthread+1 to 2*min_perthread will use 2 threads, so on
 //   unsigned long const max_threads =
 //       (length + min_per_thread - 1) / min_per_thread;

 //   int numThreads = hardware_threads - 2;
 //   // how many pixels of the total height, each thread is responsible for
 //   // for example, if I have an image of height 800 and 7 threads, then each thread
 //   // handles 115 of the height, last thread handles 800 - 6*115 = 110
 //   int block_size = (int) ceil(1.0 * image_height / numThreads);

	//std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

 //   // world objects 
 //   
 //   hl = random_scene();

 //   outputVector.resize(numThreads);

 //   /*
 //   // materials 
 //   auto m1 = make_shared<Metal>(color(0.4, 0.5, 0.9));
 //   auto m2 = make_shared<Lambertian>(color(0.6, 0.1, 0.42));
 //   auto m3 = make_shared<Lambertian>(color(0.21, 0.91, 0.86));

 //   // add to world list
 //   hl.add(make_shared<sphere>(point3(0, 0, -1), 0.5, m2));
 //   hl.add(make_shared<sphere>(point3(0, -100.5, -1), 100, m1));
 //   hl.add(make_shared<sphere>(point3(0.9, 0, -1), 0.3, m3));
 //   */
 //   


 //   //for (int j = image_height - 1; j >= 0; --j) {
 //   //    std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
 //   //    for (int i = 0; i < image_width; ++i) {
 //   //        /*
 //   //        auto u = double(i) / (c.image_width - 1);
 //   //        auto v = double(j) / (c.image_height - 1);

 //   //        // something like (0.245,0.3532,-1). Basically the vector to the point on the plane
 //   //        auto vector_to_target_from_eye = c.lower_left_corner + u * c.horizontal + v * c.vertical;
 //   //        

 //   //        // write it as a ray
 //   //        ray r(c.origin, vector_to_target_from_eye);

 //   //        color color_at_target = ray_color(r, hl);
 //   //        write_color(std::cout, color_at_target);
 //   //        */
 //   //        color pixel_color(0, 0, 0);
 //   //        for (int k = 0; k < samples_per_pixel; k++) {
 //   //            auto u = (i + random_double()) / (image_width - 1);
 //   //            auto v = (j + random_double()) / (image_height - 1);
 //   //            ray r = c.getRay(u, v);
 //   //            // decide what color will be used
 //   //            color color_at_target = ray_color(r, hl, child_ray_depth);
 //   //         
 //   //            pixel_color += color_at_target;
 //   //        }
 //   //        // draw the color using ostream
 //   //        // pixel color is accumulation of 100 random samples around the target area
 //   //        // write color will divide this final sum by 100 to get a nice average value, which generates a good hue
 //   //        write_color(samples_per_pixel, std::cout, pixel_color);
 //   //    }
 //   //}

 //   std::vector<std::thread> running_threads;

 //   int curHeightStart = 0;
 //   int curHeightEnd = 0;
 //   std::cerr << "Height Block size: " << block_size << '\n';
 //   for (int i = 0; i < numThreads -1; i++) {
 //       std::cerr << "Starting thread " << i << '\n';
 //       curHeightEnd += block_size;
 //       // its actually start, end in handleRendering arguments but it goes from 
 //       // high to low height sooo 
 //       // I have it flipped here
 //       std::thread t(handleRendering, curHeightEnd-1, curHeightStart, i);
 //       running_threads.emplace_back(move(t));
 //       curHeightStart = curHeightEnd;
 //   }

 //   // do the last thread manually, might be a different number than the rest
 //   std::cerr << "Starting thread " << numThreads -1 << '\n';
 //   curHeightEnd = image_height-1; // the very last height
 //   std::thread t(handleRendering, curHeightEnd, curHeightStart, numThreads - 1);
 //   running_threads.emplace_back(move(t));

 //   for (auto &x : running_threads) {
 //       x.join();
 //   }
 //   // after all threads have joined, output in order
 //   for (int j = numThreads - 1; j >= 0; j--) {
 //       std::cerr << outputVector[j].size() << '\n';
 //       for (auto x : outputVector[j]) {
 //           double r = x.first;
 //           double g = x.second.first;
 //           double b = x.second.second;
 //           std::cout << static_cast<int>(256 * clamp(r, 0, 0.999)) << " " << static_cast<int>(256 * clamp(g, 0, 0.999)) << " " << static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
 //          /* std::cout << static_cast<int>(255.999 * r) << ' '
 //               << static_cast<int>(255.999 * g) << ' '
 //               << static_cast<int>(255.999 * b) << '\n';*/
 //       }
 //   }
 //   std::cerr << "\nDone.\n";
 //   auto stop = high_resolution_clock::now();
 //   auto duration = duration_cast<microseconds>(stop - start);
 //   std::cerr << "Program took " << (double) duration.count()/1000000.0 << " seconds" << '\n';
    return 0;
}