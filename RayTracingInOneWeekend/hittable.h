#pragma once

#include "ray.h"
#include "vec3.h"

// forward declare the existence of a class named material.
class Material;

struct hit_record {
	point3 p; // the point where ray hits 
	vec3 normal; // the normal vector from eye to surface where point is
	double t; // a + bt, in the ray formula
	bool front_face; // is it coming out from surface or going in
	shared_ptr<Material> hit_material; // the type of material we hit

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		if (dot(r.direction(), outward_normal) > 0) {
			// < 90, same face, ray originated from within the sphere
			front_face = true;
			normal *= -1;
		}
		else {
			front_face = false;
		}
	}
};

class hittable
{
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

