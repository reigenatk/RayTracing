#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	point3 orig; // origin
	vec3 dir;

	ray() {}
	ray(const point3& origin, const vec3& direction)
		: orig(origin), dir(direction) {}

	point3 origin() const { return orig;  }
	point3 direction() const { return dir; }
	
	// scaling a ray via its direction
	point3 at(double t) const {
		return orig + t * dir;
	}

};

#endif