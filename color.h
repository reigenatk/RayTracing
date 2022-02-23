#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

inline void write_color(int samples_per_pixel, std::ostream& out, 
    color pixel_color, std::vector<std::pair<double,std::pair<double, double>>> &v) {

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

    std::pair<double, std::pair<double, double>> entry;
    entry.first = r;
    entry.second.first = g;
    entry.second.second = b;
    v.push_back(entry);
    
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