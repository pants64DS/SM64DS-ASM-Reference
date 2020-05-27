#ifndef O_STREAM_INCLUDED
#define O_STREAM_INCLUDED

#include <cstdint>

class ostream
{
	static constexpr unsigned buffer_size = 120;
	static char buffer[buffer_size];
	
public:
	static void set_buffer(const char* char_ptr);
	static void flush();
	static void flush(uint64_t p0);
	static void flush(uint32_t r0, uint32_t r1, uint32_t r2);
	static void flush(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);

	ostream& operator<<(const char* char_ptr);
	ostream& operator<<(char character);
	ostream& operator<<(uint32_t val);
	ostream& operator<<(uint64_t val);

	template<typename T>
	inline ostream& operator<<(T* ptr) {return *this << reinterpret_cast<uint32_t>(ptr);}
	inline ostream& operator<<(int val) {return *this << static_cast<uint32_t>(val);}
	inline ostream& operator<<(unsigned val) {return *this << static_cast<uint32_t>(val);}
	inline ostream& operator<<(int64_t val) {return *this << static_cast<uint64_t>(val);}
	inline ostream& operator<<(bool b) {return *this << (b ? "true" : "false");}
	
#ifdef SM64DS_COMMON_INCLUDED

	inline ostream& operator<<(const Vector3& vec)
	{
		set_buffer("{0x%r0%_f, 0x%r1%_f, 0x%r2%_f}");
		flush(vec.x.val, vec.y.val, vec.z.val);

		return *this;
	}

	inline ostream& operator<<(const Vector3_16& vec)
	{
		set_buffer("{0x%r0%, 0x%r1%, 0x%r2%}");
		flush(vec.x, vec.y, vec.z);

		return *this;
	}

	inline ostream& operator<<(const Matrix4x3& m)
	{
		set_buffer("[ 0x%r0%_f  0x%r1%_f  0x%r2%_f  0x%r3%_f ]\n");

		flush(m.c0.x.val, m.c1.x.val, m.c2.x.val, m.c3.x.val);
		flush(m.c0.y.val, m.c1.y.val, m.c2.y.val, m.c3.y.val);
		flush(m.c0.z.val, m.c1.z.val, m.c2.z.val, m.c3.z.val);

		return *this;
	}

	inline ostream& operator<<(const Matrix3x3& m)
	{
		set_buffer("[ 0x%r0%_f  0x%r1%_f  0x%r2%_f ]\n");

		flush(m.c0.x.val, m.c1.x.val, m.c2.x.val);
		flush(m.c0.y.val, m.c1.y.val, m.c2.y.val);
		flush(m.c0.z.val, m.c1.z.val, m.c2.z.val);

		return *this;
	}

	inline ostream& operator<<(Fix12i fix)
	{
		set_buffer("0x%r0%_f");
		flush(fix.val);

		return *this;
	}

	inline ostream& operator<<(Fix12s fix)
	{
		set_buffer("0x%r0%_fs");
		flush(fix.val);

		return *this;
	}
	
#endif
}
extern cout;

#endif