#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "constants.h"

class vec3
{
public:
	double e[3];

	vec3() : e{ 0,0,0 } {}
	vec3(double e1, double e2, double e3) : e{ e1,e2,e3 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	
	// get and set
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3& operator+=(const vec3 &v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this; // returns a reference to the newly made object 
	}

	vec3& operator*=(const double& v) {
		e[0] *= v;
		e[1] *= v;
		e[2] *= v;
		return *this;
	}

	vec3& operator/=(const double t) {
		return *this *= (1 / t);
	}

	double length() {
		return sqrt(length_squared());
	}

	double length_squared() {
		return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
	}

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));

	}

	bool near_zero() const {
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}
};

// type alias
using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// section 9.4
inline vec3 reflect(const vec3& v, const vec3& norm) {
	vec3 b = dot(v, norm) * norm;
	vec3 reflected = v - 2 * b;
	return reflected;
}

inline vec3 randomInUnitSphere() {
	while (true) {
		vec3 v = vec3::random(-1, 1); // picks 3 random x,y,z components between -1 and 1
		if (v.length() <= 1) {
			return v;
		}
	}
}

inline vec3 random_unit_vector() {
	return unit_vector(randomInUnitSphere());
}

#endif