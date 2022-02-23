#pragma once

#include "constants.h"
#include "ray.h"
#include "vec3.h"
struct hit_record;

class Material {
public:
	virtual bool scatter(ray& r, const hit_record& rec, color& attentuation,
		ray& scattered) const = 0;

};

// lambertian diffusion
class Lambertian : public Material {
public:
	Lambertian(const color& a) : albedo(a) {}
	virtual bool scatter(ray& r, const hit_record& rec, color& attenuation,
		ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		// avoid issues where the random unit vector + normal is a near-zero vector. 
		if (scatter_direction.near_zero()) {
			scatter_direction = rec.normal;
		}
		attenuation = albedo;
		scattered = ray(rec.p, scatter_direction);
		return true;
	}


	color albedo;
};

// metal (reflection)
class Metal : public Material {
public:
	Metal(const color& a) : albedo(a) {}
	virtual bool scatter(ray& r, const hit_record& rec, color& attenuation,
		ray& scattered) const override {
		vec3 ray_direction = r.direction();
		auto reflected = reflect(ray_direction, rec.normal);

		attenuation = albedo;
		scattered = ray(rec.p, reflected);

		// still both goin in same direction (within 90 deg)
		return dot(scattered.direction(), rec.normal) > 0;
	}

	color albedo;
};
