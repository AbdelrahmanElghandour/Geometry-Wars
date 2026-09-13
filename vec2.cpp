#include <math.h>
#include "vec2.h"


Vec2::Vec2() {}
Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

bool Vec2::operator == (const Vec2& other) const {
	return (x == other.x && y == other.y);
}
bool Vec2::operator != (const Vec2& other) const {
	return !(*this == other);
}

Vec2 Vec2::operator + (const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::operator - (const Vec2& other) const {
	return Vec2(x - other.x, y - other.y);
}
Vec2 Vec2::operator * (const float val) const {
	return Vec2(x * val, y * val);
}
Vec2 Vec2::operator / (const float val) const {
	return Vec2(x / val, y / val); 
}

void Vec2::operator += (const Vec2& other) {
	x += other.x;
	y += other.y;
}
void Vec2::operator -= (const Vec2& other) {
	x -= other.x;
	y -= other.y;
}
void Vec2::operator *= (const float val) {
	x *= val;
	y *= val;
}
void Vec2::operator /= (const float val) {
	x /= val;
	y /= val;
}

float Vec2::dist(const Vec2& rhs) const {
	// TODO
	float Dx = x - rhs.x;
	float Dy = y - rhs.y;
	return sqrt(Dx * Dx + Dy * Dy);
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}
