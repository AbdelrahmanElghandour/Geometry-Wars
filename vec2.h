#pragma once
#include <iostream>

class Vec2 {
public:
	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float xin, float yin);

	bool operator == (const Vec2& other) const;
	bool operator != (const Vec2& other) const;

	Vec2 operator + (const Vec2& other) const;
	Vec2 operator - (const Vec2& other) const;
	Vec2 operator * (const float val) const;
	Vec2 operator / (const float val) const;

	void operator += (const Vec2& other);
	void operator -= (const Vec2& other);
	void operator *= (const float val);
	void operator /= (const float val);

	float dist(const Vec2& rhs) const;


	friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);
};