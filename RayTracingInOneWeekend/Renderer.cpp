#include "Renderer.h"

using namespace std::chrono;

Renderer::Renderer() {
    aspect_ratio = 3.0 / 2.0;

    // for anti aliasing
    samples_per_pixel = 80;

    // more 'accurate' the larger this number is, but will take longer
    child_ray_depth = 25;

    c = Camera(90.0, aspect_ratio, point3(0, 1, 0), point3(8, 2, 2), point3(0, 0, 0));

    currentState = Renderer::NotStarted;
}

color Renderer::ray_color(ray& r, hittable_list& list, int depth) {
    hit_record hrecord;

    if (depth <= 0) {
        return color(0, 0, 0);
    }
    if (list.hit(r, 0.001, inf, hrecord)) {
        color attenuation;
        ray scatter;

        // this call will populate scattered with the right ray to consider
        if (hrecord.hit_material->scatter(r, hrecord, attenuation, scatter)) {
            return attenuation * ray_color(scatter, list, depth - 1);
        }
        else {
            return color(0, 0, 0);
        }
    }

    // if it hits nothing. Basically the color of empty space.
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list Renderer::random_scene() {
    hittable_list world;
    auto ground_material = make_shared<Lambertian>(color(0.75, 0.75, 0.75));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -8; a < 8; a++) {
        for (int b = -8; b < 8; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.5) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // metal
                    auto albedo = color::random(0.2, 1);
                    auto fuzz = random_double(0, 1);
                    sphere_material = make_shared<Metal>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                //else {
                //    // glass
                //    sphere_material = make_shared<dielectric>(1.5);
                //    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                //}
            }
        }
    }

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5));
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    return world;
}

// bot left = 0,0 and top right is (x,y)
void Renderer::handleRendering(int startheight, int endheight, int threadNo) {
    std::thread::id threadID = std::this_thread::get_id();
    for (int j = startheight; j >= endheight; --j) {
        std::cerr << "Thread " << threadID << " has " << j - endheight + 1 << " Scanlines remaining" << '\n';
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int k = 0; k < samples_per_pixel; k++) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = c.getRay(u, v);
                // decide what color will be used
                color color_at_target = ray_color(r, hl, child_ray_depth);

                pixel_color += color_at_target;
            }
            // draw the color using ostream
            // pixel color is accumulation of 100 random samples around the target area
            // write color will divide this final sum by 100 to get a nice average value, which generates a good hue

            // convention in the buffer is that top left is height 0, not bot left. So height is transformed using this formula before passing in
            int flipped_y = image_height - j - 1;
            int idx = (i + flipped_y * image_width) * 4; // we write 4 values per pixel, and so since its a char array we need to *4
            write_color(samples_per_pixel, std::cout, pixel_color, m_renderBuffer, idx);
        }
    }
}

void Renderer::SetWidthAndHeight(int width, int height) {
    image_width = width;
    image_height = height;
}

void Renderer::StartRenderThread() {
    // start a thread that will spawn other threads, and wait on the threads that it spawns to finish.
    RenderThread = std::thread(&Renderer::StartRender, this);
}

void Renderer::StartRender() {
    auto start = high_resolution_clock::now();
    unsigned long const hardware_threads =
        std::thread::hardware_concurrency();
    std::cerr << "Hardware threads: " << hardware_threads << '\n';

    // set the state to running
    currentState = Renderer::Running;

    // following page 52 in multithreading book
    int min_per_thread = 600;
    int length = image_width * image_height;

    // if length = 0, this returns 0. Otherwise, 1-min_perthread will use 1 thread,
    // min_perthread+1 to 2*min_perthread will use 2 threads, so on
    unsigned long const max_threads =
        (length + min_per_thread - 1) / min_per_thread;

    numThreads = hardware_threads - 3;
    // how many pixels of the total height, each thread is responsible for
    // for example, if I have an image of height 800 and 7 threads, then each thread
    // handles 115 of the height, last thread handles 800 - 6*115 = 110
    int block_size = (int)ceil(1.0 * image_height / numThreads);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // world objects 

    hl = random_scene();

    std::vector<std::thread> running_threads;

    int curHeightStart = 0;
    int curHeightEnd = 0;
    std::cerr << "Height Block size: " << block_size << '\n';
    for (int i = 0; i < numThreads - 1; i++) {
        std::cerr << "Starting thread " << i << '\n';
        curHeightEnd += block_size;
        // its actually start, end in handleRendering arguments but it goes from 
        // high to low height sooo 
        // I have it flipped here
        std::thread t(&Renderer::handleRendering, this, curHeightEnd - 1, curHeightStart, i);
        running_threads.emplace_back(move(t));
        curHeightStart = curHeightEnd;
    }

    // do the last thread manually, might be a different number than the rest
    std::cerr << "Starting thread " << numThreads - 1 << '\n';
    curHeightEnd = image_height - 1; // the very last height
    std::thread t(&Renderer::handleRendering, this, curHeightEnd, curHeightStart, numThreads - 1);
    running_threads.emplace_back(move(t));

    for (auto& x : running_threads) {
        x.join();
    }

    std::cerr << "\nDone.\n";
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cerr << "Program took " << (double)duration.count() / 1000000.0 << " seconds" << '\n';
}

int Renderer::getNumThreads() {
    return numThreads;
}