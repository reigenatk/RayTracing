#pragma once

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "material.h"

class sphere : public hittable 
{
public:
	vec3 center;
	double radius;
	shared_ptr<Material> sphere_material;

	sphere() {}
	sphere(vec3 cen, double rad, shared_ptr<Material> m) : center(cen), radius(rad), sphere_material(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

// basically does the same as hit_sphere except we make sure the root is 
// between t_min and t_max, 
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	auto a = dot(r.direction(), r.direction());
	auto b = 2 * dot(r.origin() - center, r.direction());
	auto c = dot(r.origin() - center, r.origin() - center) - radius * radius;

	auto discriminant = b * b - 4 * a * c;
	// does not intersect since quadratic eq has 0 solutions
	if (discriminant < 0) {
		return false;
	}
	double root = (-b - sqrt(discriminant)) / (2.0 * a);
	if (root < t_min || root > t_max) {
		// check other root
		root = (-b + sqrt(discriminant)) / (2.0 * a);
		if (root < t_min || root > t_max) {
			return false;
		}
	}
	vec3 to_circle = r.at(root);
	// kinda get this but not rly lol, but it generate a normal protruding
	// from the sphere at the point of intersection
	vec3 normal = to_circle - vec3(0, 0, -1);
	vec3 N = unit_vector(normal);
	
	rec.t = root;
	rec.p = to_circle;
	rec.normal = N;
	rec.set_face_normal(r, rec.normal);
	// rec.normal = (rec.p - center) / radius; (SAME THING)

	// set the matieral too
	rec.hit_material = sphere_material;

	return true;
}

