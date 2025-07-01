#include "point.hpp"

#include <stdexcept>

TEMPLATE
Point<T, S>::Point()
{
	
}

TEMPLATE
Point<T, S>::Point(const T init)
{
	for (int i = 0; i < S; i++)
	{
		data[i] = init;
	}
}

TEMPLATE
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

TEMPLATE
Point<T, S>& Point<T, S>::operator=(const Point<T, S>& other)
{
	this->data = other.data;
	return (*this);
}

TEMPLATE
Point<T, S>::~Point()
{
	
}

TEMPLATE
T& Point<T, S>::operator[](const uint32_t i)
{
	if (i >= S) throw (std::out_of_range("Index out of bounds"));

	return (data[i]);
}

TEMPLATE
Point<T, S> Point<T, S>::operator+(Point<T, S> other)
{
	Point<T, S> result;
	result += *this;
	result += other;

	return (result);
}

TEMPLATE
Point<T, S> Point<T, S>::operator-(Point<T, S> other)
{
	Point<T, S> result;
	result -= *this;
	result -= other;

	return (result);
}

TEMPLATE
Point<T, S> Point<T, S>::operator*(Point<T, S> other)
{
	Point<T, S> result;
	result *= *this;
	result *= other;

	return (result);
}

TEMPLATE
Point<T, S> Point<T, S>::operator/(Point<T, S> other)
{
	Point<T, S> result;
	result /= *this;
	result /= other;

	return (result);
}

TEMPLATE
void Point<T, S>::operator+=(Point<T, S> other)
{
	for (int i = 0; i < S; i++) data[i] += other[i];
}

TEMPLATE
void Point<T, S>::operator-=(Point<T, S> other)
{
	for (int i = 0; i < S; i++) data[i] -= other[i];
}

TEMPLATE
void Point<T, S>::operator*=(Point<T, S> other)
{
	for (int i = 0; i < S; i++) data[i] *= other[i];
}

TEMPLATE
void Point<T, S>::operator/=(Point<T, S> other)
{
	for (int i = 0; i < S; i++) data[i] /= other[i];
}

//TEMPLATE
//std::ostream& operator<<(std::ostream& out, Point<T, S>& point)
//{
//	for (int i = 0; i < S; i++) out << point[i] << (i + 1 < S ? ", " : "");
//
//	return (out);
//}

TEMPLATE
std::ostream& operator<<(std::ostream& out, Point<T, S> point)
{
	for (int i = 0; i < S; i++)
	{
		out << static_cast<char>(119 + ((i + 1) % 4)) << ": " << point[i] << (i + 1 < S ? ", " : "");
	}

	return (out);
}