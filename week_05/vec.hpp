#pragma once

#include <cmath>
#include <iostream>

class vec3d {
   public:
	double x, y, z;

	vec3d(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
	vec3d operator+(const vec3d &other) const { return vec3d(x + other.x, y + other.y, z + other.z); }
	vec3d operator-(const vec3d &other) const { return vec3d(x - other.x, y - other.y, z - other.z); }
	vec3d operator/(const vec3d &other) const { return vec3d(x / other.x, y / other.y, z / other.z); }
	vec3d operator*(const vec3d &other) const { return vec3d(x * other.x, y * other.y, z * other.z); }

	vec3d &operator+=(const vec3d &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	vec3d &operator-=(const vec3d &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	vec3d &operator*=(const vec3d &other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	vec3d &operator/=(const vec3d &other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	vec3d operator*(double val) const { return vec3d(x * val, y * val, z * val); }
	vec3d operator/(double val) const { return vec3d(x / val, y / val, z / val); }

	vec3d &operator*=(double val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	vec3d &operator/=(double val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}

	friend vec3d operator*(double val, const vec3d &vec) { return vec * val; }

	double		  dot(const vec3d &other) const { return x * other.x + y * other.y + z * other.z; }
	static double dot(const vec3d &left, const vec3d &right) { return left.dot(right); }

	double lengthSquared() const { return this->dot(*this); }
	double length() const { return std::sqrt(this->lengthSquared()); }
	vec3d &normalize() { return *this /= length(); }

	constexpr static double epsilon = 1e-9;
	bool					isZeroEps(double eps = epsilon) const { return lengthSquared() <= eps * eps; }
	bool operator==(const vec3d &other) const { return (*this - other).isZeroEps(); }
	bool operator!=(const vec3d &other) const { return !(*this == other); }
	operator bool() const { return !isZeroEps(); }
	
	friend std::ostream &operator<<(std::ostream &out, const vec3d &vec) {
		return out << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	}

	friend std::istream &operator>>(std::istream &in,vec3d &vec) {
		return in >> vec.x >> vec.y >> vec.z;
	}

};
