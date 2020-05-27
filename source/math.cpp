#include "math.h"

void Vector3::NormalizeQ24()
{
	Fix24i mag = Fix24i(Len(), true);
	x.val = (Fix24i(x, true) / mag).val;
	y.val = (Fix24i(y, true) / mag).val;
	z.val = (Fix24i(z, true) / mag).val;
}

Fix24i Vector3::DotQ24(const Vector3& v) const
{
	return Fix24i(x, true) * Fix24i(v.x, true)
	     + Fix24i(y, true) * Fix24i(v.y, true)
	     + Fix24i(z, true) * Fix24i(v.z, true);
}

void Vector3::ThisFromCrossQ24(const Vector3& v0, const Vector3& v1)
{
	x.val = (Fix24i(v0.y, true) * Fix24i(v1.z, true) - Fix24i(v0.z, true) * Fix24i(v1.y, true)).val;
	y.val = (Fix24i(v0.z, true) * Fix24i(v1.x, true) - Fix24i(v0.x, true) * Fix24i(v1.z, true)).val;
	z.val = (Fix24i(v0.x, true) * Fix24i(v1.y, true) - Fix24i(v0.y, true) * Fix24i(v1.x, true)).val;
}

void Vector3::Lerp(const Vector3& a, const Vector3& b, Fix12i t)
{
	*this = b;
	*this -= a;
	*this *= t;
	*this += a;
}

Vector3_Q24 Vector3_Q24::operator* (Fix24i scalar) const
{
	return Vector3_Q24 {get_x() * scalar, get_y() * scalar, get_z() * scalar};
}

Vector3_Q24 Vector3_Q24::operator/ (Fix24i scalar) const
{
	return Vector3_Q24 {get_x() / scalar, get_y() / scalar, get_z() / scalar};
}

Fix24i Vector3_Q24::MagSquared() const
{
	return get_x() * get_x()
	     + get_y() * get_y()
	     + get_z() * get_z();
}

constexpr uint64_t fac(uint64_t n)
{
	return n > 0 ? n * fac(n - 1) : 1;
}

Fix24i cos(short a)
{
	int b = a;
	if (b < 0) b = -b;
	
	bool flipped;

	if (b > 0x4000)
	{
		b = 0x8000 - b;
		flipped = true;
	}
	else flipped = false;

	uint64_t x = (b * 0x3243F6A8885A3 + (uint64_t(1) << 31)) >> 32; // Q31

	constexpr int64_t h = int64_t(1) << 31;

	uint64_t y2  = x  * x;         // Q62
	uint64_t x2  = (y2 + h) >> 32; // Q30
	uint64_t y4  = x2 * x2;        // Q60
	uint64_t x4  = (y4 + h) >> 32; // Q28
	uint64_t y6  = x2 * x4;        // Q58
	uint64_t x6  = (y6 + h) >> 32; // Q26
	uint64_t y8  = x4 * x4;        // Q56
	uint64_t x8  = (y8 + h) >> 32; // Q24
	uint64_t y10 = x4 * x6;        // Q54
	uint64_t y12 = x6 * x6;        // Q52
	uint64_t y14 = x6 * x8;        // Q50
	uint64_t y16 = x8 * x8;        // Q48

	uint64_t resQ48 = (uint64_t(1) << 48)
		- (((Div64(y2  >> 1, fac( 2)) << 1) + (uint64_t(1) << (23 + 14))) >> 14)
		+ (((Div64(y4  >> 1, fac( 4)) << 1) + (uint64_t(1) << (23 + 12))) >> 12)
		- (((Div64(y6  >> 1, fac( 6)) << 1) + (uint64_t(1) << (23 + 10))) >> 10)
		+ (((Div64(y8  >> 1, fac( 8)) << 1) + (uint64_t(1) << (23 +  8))) >> 8)
		- (((Div64(y10 >> 1, fac(10)) << 1) + (uint64_t(1) << (23 +  6))) >> 6)
		+ (((Div64(y12 >> 1, fac(12)) << 1) + (uint64_t(1) << (23 +  4))) >> 4)
		- (((Div64(y14 >> 1, fac(14)) << 1) + (uint64_t(1) << (23 +  2))) >> 2)
		+  ((Div64(y16 >> 1, fac(16)) << 1));

	Fix24i resQ24 ((resQ48 + (1 << 23)) >> 24, true);

	return flipped ? -resQ24 : resQ24;
}

Fix12i CubicHermiteSpline(Fix12i t)
{
	return 3 * t * t - (t * t * t << 1);
}