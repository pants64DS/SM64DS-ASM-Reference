#ifndef GRAVITY_MATH_INCLUDED
#define GRAVITY_MATH_INCLUDED
#define GRAVITY_MATH

#include "include/NDSCore.h"
#include "ostream.h"

struct Vector3_Q24;

extern "C"
{
	int DivQ24(int num, int den);
	uint64_t Div64(uint64_t nom, uint64_t den);
}

struct Fix24i
{
	int val;

	inline constexpr Fix24i() : val() {}
	inline constexpr explicit Fix24i(int value, bool raw = false) : val(raw ? value : value << 24) {}
	inline constexpr explicit Fix24i(long double value) : val(value * 16777216.) {}
	template<typename U> inline constexpr explicit Fix24i(Fix12<U> fix, bool raw = false) : val(raw ? fix.val : fix.val << 12) {}

	inline constexpr Fix24i   operator-  ()            const { return Fix24i(-val, true); }
	inline void               operator+= (Fix24i fix)        { val += fix.val; }
	inline constexpr Fix24i   operator+  (Fix24i fix)  const { return Fix24i(val + fix.val, true); }
	inline void               operator-= (Fix24i fix)        { val -= fix.val; }
	inline constexpr Fix24i   operator-  (Fix24i fix)  const { return Fix24i(val - fix.val, true); }
	inline void               operator*= (Fix24i fix)        { val = ((bigger_t<int>)val * fix.val + 0x800000) >> 24; }
	inline void               operator*= (int integer)       { val *= integer; }
	inline constexpr Fix24i   operator*  (Fix24i fix)  const { return Fix24i(((bigger_t<int>)val * fix.val + 0x800000) >> 24, true); }
	inline constexpr Fix24i   operator*  (int integer) const { return Fix24i(val * integer, true); }
	inline void               operator/= (Fix24i fix)        { val = DivQ24(val, fix.val); }
	inline void               operator/= (int integer)       { val /= integer; }
	inline Fix24i             operator/  (Fix24i fix)  const { return Fix24i(DivQ24(val, fix.val), true); }
	inline constexpr Fix24i   operator/  (int integer) const { return Fix24i(val / integer, true); }
	inline void               operator<<=(int amount)        { val <<= amount; }
	inline constexpr Fix24i   operator<< (int amount)  const { return Fix24i(val << amount, true); }
	inline void               operator>>=(int amount)        { val >>= amount; }
	inline constexpr Fix24i   operator>> (int amount)  const { return Fix24i(val >> amount, true); }
	inline constexpr bool     operator== (Fix24i fix)  const { return val == fix.val; }
	inline constexpr bool     operator!= (Fix24i fix)  const { return val != fix.val; }
	inline constexpr bool     operator<  (Fix24i fix)  const { return val <  fix.val; }
	inline constexpr bool     operator<= (Fix24i fix)  const { return val <= fix.val; }
	inline constexpr bool     operator>  (Fix24i fix)  const { return val >  fix.val; }
	inline constexpr bool     operator>= (Fix24i fix)  const { return val >= fix.val; }

	inline constexpr Fix24i Abs() const { return this->val >= 0 ? *this : -*this; }
	inline constexpr explicit operator int()    const { return val >> 24; } //warning! Floors!

	template<typename U> inline constexpr explicit operator Fix12<U>() const {return Fix12<U>((val + 0x1000) >> 12, true);}


	inline bool lerp(Fix24i dest, Fix24i step) { return Lerp(val, dest.val, step.val); }
};

inline constexpr Fix24i operator* (int integer, Fix24i fix) {return Fix24i(integer * fix.val, true);}
inline           Fix24i operator/ (int integer, Fix24i fix) {return Fix24i(DivQ24(integer << 24, fix.val), true);}

constexpr Fix24i operator""_f24 (unsigned long long val) {return Fix24i(val, true);}
constexpr Fix24i operator""_f24 (long double val) {return Fix24i(val);}

struct Vector3_Q24 : public Vector3
{
	inline Fix24i get_x() const {return Fix24i(x.val, true);}
	inline Fix24i get_z() const {return Fix24i(z.val, true);}
	inline Fix24i get_y() const {return Fix24i(y.val, true);}

	inline void set_x(Fix24i fix) {x.val = fix.val;}
	inline void set_z(Fix24i fix) {z.val = fix.val;}
	inline void set_y(Fix24i fix) {y.val = fix.val;}

	inline constexpr Vector3_Q24() {};
	inline Vector3_Q24(Fix24i a, Fix24i b, Fix24i c) {x.val = a.val; y.val = b.val; z.val = c.val;};

	inline explicit Vector3_Q24(const Vector3& v, bool raw = false)
	{
		x.val = Fix24i(v.x, raw).val;
		y.val = Fix24i(v.y, raw).val;
		z.val = Fix24i(v.z, raw).val;
	}
	
	static inline Vector3_Q24 FromCross(const Vector3& v0, const Vector3& v1)
	{
		Vector3_Q24 v;
		v.ThisFromCrossQ24(v0, v1);
		return v;
	}

	inline Fix24i HorzDist (const Vector3_Q24& v) const {return Fix24i(Vec3_HorzDist(this, &v), true);}
	inline Fix24i HorzLen  ()                     const {return Fix24i(Vec3_HorzLen(this),      true);}
	inline Fix24i Dist     (const Vector3_Q24& v) const {return Fix24i(Vec3_Dist(this, &v),     true);}
	inline Fix24i Len      ()                     const {return Fix24i(LenVec3(this),           true);}

	void        operator*= (Fix24i scalar);
	Vector3_Q24 operator*  (Fix24i scalar) const;
	void        operator/= (Fix24i scalar);
	Vector3_Q24 operator/  (Fix24i scalar) const;

	inline constexpr Vector3 ToQ12() const {return Vector3 {(x + 0x800_f) >> 12, (y + 0x800_f) >> 12, (z + 0x800_f) >> 12};}
	Fix24i MagSquared() const;

	inline void Normalize() {NormalizeQ24();}
	inline Fix24i Dot(const Vector3_Q24& v) const {return DotQ24(v);}
	inline void ThisFromCross(const Vector3_Q24& v0, const Vector3_Q24& v1) {ThisFromCrossQ24(v0, v1);}

private:
	using Vector3::NormalizeQ24;
	using Vector3::DotQ24;
	
	using Vector3::x;
	using Vector3::y;
	using Vector3::z;
};

inline ostream& operator<<(ostream& os, Fix24i fix)
{
	ostream::set_buffer("0x%r0%_f24");
	ostream::flush(fix.val);

	return os;
}

inline ostream& operator<<(ostream& os, const Vector3_Q24& vec)
{
	ostream::set_buffer("{0x%r0%_f24, 0x%r1%_f24, 0x%r2%_f24}");
	ostream::flush(vec.get_x().val, vec.get_y().val, vec.get_z().val);

	return os;
}

Fix24i sqrt(Fix24i x) __attribute__((const));
Fix24i cos(short a) __attribute__((const));
Fix12i CubicHermiteSpline(Fix12i t);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

inline short Asin(Fix24i x)
{
	return cstd::atan2(Fix12i(x), Fix12i(sqrt(0x1000000_f24 - x*x)));
}

inline short Acos(Fix24i x)
{
	return cstd::atan2(Fix12i(sqrt(0x1000000_f24 - x*x)), Fix12i(x));
}

inline Fix24i sin(short a)
{
	return cos(a - 0x4000);
}

#endif