#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:

	std::vector<shared_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void add(shared_ptr<hittable> a) {
		objects.push_back(a);
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;


};

// loop through all the objects in our world, see if this ray hits any of them
// with a t between t_min and t_max, if so, return true, else return false. Also store
// the contents of the hit information inside rec on return.
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record hrec;
	bool has_hit_anything = false;

	// we keep this because the light will stop at the first thing it hits
	// and smallest value of t means smallest distance since formula is a+bt
	auto closest_so_far = t_max;

	for (auto &x : objects) {
		if (x->hit(r, t_min, t_max, hrec) && hrec.t < closest_so_far) {
			has_hit_anything = true;
			closest_so_far = hrec.t;
			rec = hrec;
		}
		
	}

	return has_hit_anything;
}