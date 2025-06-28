#include "point.hpp"

#include <stdexcept>

template <typename T, unsigned int S>
Point<T, S>::Point()
{
	
}

template <typename T, unsigned int S>
Point<T, S>::Point(std::initializer_list<T> init)
{
	if (init.size() > S) throw (std::out_of_range("Too many initializers"));

	unsigned int i = 0;
	for (const T& element : init)
	{
		data[i] = element;
		i++;
	}
}

template <typename T, unsigned int S>
Point<T, S>::~Point()
{
	
}

template <typename T, unsigned int S>
T& Point<T, S>::operator[](unsigned int i)
{
	if (i >= S) throw (std::out_of_range("Index out of bounds"));

	return (data[i]);
}

template <typename T, unsigned int S>
std::ostream& operator<<(std::ostream& out, Point<T, S>& point)
{
	for (int i = 0; i < S; i++) out << point[i] << (i + 1 < S ? ", " : "");

	return (out);
}