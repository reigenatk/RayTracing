#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

inline void write_color(int samples_per_pixel, std::ostream& out, 
    color pixel_color, uint8_t* &v, int idx) {

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);
    
    // uint8_t = unsigned char 
    r = static_cast<uint8_t>(255 * clamp(r, 0, 0.999));
    g = static_cast<uint8_t>(255 * clamp(g, 0, 0.999));
    b = static_cast<uint8_t>(255 * clamp(b, 0, 0.999));
    uint8_t l = 255;

    // write values into buffer. Pointer is just like array, you can use [] operator and even add stuff inside.
    // As long as you cast the values you are putting in it will know what you mean.
    v[idx] = r;
    v[idx + 1] = g;
    v[idx + 2] = b;
    v[idx + 3] = 255;
    
    
    // std::cerr << "pushed";
    // Write the translated [0,255] value of each color component.
    /*out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
    */

    //// Write the translated [0,255] value of each color component.
    //out << static_cast<int>(255.999 * pixel_color.x()) << ' '
    //    << static_cast<int>(255.999 * pixel_color.y()) << ' '
    //    << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}


#endif