#pragma once

#include <iostream>

#define TYPE(a) static_cast<uint8_t>(a)

template <typename T>
class Bitmask
{
	public:
		uint8_t value;
		Bitmask(T value) : value(TYPE(value)){};
		bool Has(T flag) const { return ((value & TYPE(flag)) == TYPE(flag)); }
};
