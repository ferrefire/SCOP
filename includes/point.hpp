#pragma once

#include <iostream>
#include <initializer_list>

template <typename T, unsigned int S> 
class Point
{
	private:
		T data[S]{};

	public:
		Point();
		Point(std::initializer_list<T> init);
		~Point();

		T& x = data[0];
		T& operator[](unsigned int i);
};

template <typename T, unsigned int S>
std::ostream& operator<<(std::ostream& out, Point<T, S>& point);

#include "point.tpp"

typedef Point<float, 2> point2D;
typedef Point<float, 3> point3D;