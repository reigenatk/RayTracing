#pragma once

#include "constants.h"
#include "ray.h"

class Camera {
public:

    Camera() {}
    // trying to make our camera accept any vertical fov (from bottom to top) 
    // and corresponding aspect ratio. Distance is still z = -1 i think tho
	Camera(double vertical_fov, double aspect_ratio, vec3 vup, point3 lookfrom, point3 lookat) {
        /*
        // fixed 16/9 example
        aspect_ratio = 16.0 / 9.0;
        //image_width = 400;
        //image_height = static_cast<int>(image_width / aspect_ratio);
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height; // 3.555 at height = 2.0
        */

        //new
        auto theta = degrees_to_radians(vertical_fov);
        auto h = tan(theta / 2); // (image_height / 2) / focal_length (1) TAN = opp/adj
        auto viewport_height = h * 2;
        auto viewport_width = viewport_height * aspect_ratio;

        
        //auto focal_length = 1.0; // distance to the plane from eye

        //origin = point3(0, 0, 0);
        //horizontal = vec3(viewport_width, 0, 0);
        //vertical = vec3(0, viewport_height, 0);
        //lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
        

        //new
        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
	}
    ray getRay(double u, double v) {
        // -origin doesn't do anything cuz here its 0,0,0 
        // but if u had a diff origin ofc it would matter
        auto vector_to_target_from_eye = lower_left_corner + u * horizontal + v * vertical - origin;
        // write it as a ray
        return ray(origin, vector_to_target_from_eye);
    }
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
    int image_width;
    int image_height;
};