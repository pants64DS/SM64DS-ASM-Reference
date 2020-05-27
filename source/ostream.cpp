#include "ostream.h"

ostream cout;

void ostream::set_buffer(const char* char_ptr)
{
	for (unsigned i = 0; i < buffer_size; i++)
	{
		if ((buffer[i] = char_ptr[i]) == '\0')
			break;
	}
}

ostream& ostream::operator<<(const char* char_ptr)
{
	set_buffer(char_ptr);
	flush();

	return *this;
}

ostream& ostream::operator<<(char character)
{
	buffer[0] = character;
	buffer[1] = '\0';

	flush();

	return *this;
}

ostream& ostream::operator<<(uint32_t val)
{
	set_buffer("0x%r0%");
	flush(val);

	return *this;
}

ostream& ostream::operator<<(uint64_t val)
{
	set_buffer("0x%r1%%r0%");
	flush(val);

	return *this;
}