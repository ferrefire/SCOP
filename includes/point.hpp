#pragma once

#include <iostream>
#include <initializer_list>
#include <stdint.h>
#include <stdexcept>
#include <array>

#define TEMPLATE template <ValidType T, uint32_t S> \
	requires ValidRange<S>

template <typename T>
concept ValidType = (std::is_floating_point<T>().value || std::is_integral<T>().value);

template <uint32_t S>
concept ValidRange = (S >= 1 && S <= 4);

TEMPLATE
class Point
{
	protected:
		std::array<T, S> data{};

	public:
		Point();
		Point(const T init);
		Point(std::initializer_list<T> init);
		Point<T, S>& operator=(const Point<T, S>& other);
		~Point();

		T& x = data[0];
		T& y = data[1];
		T& z = (S > 2 ? data[2] : data[data.size() - 1]);
		T& w = (S > 3 ? data[3] : data[data.size() - 1]);

		T& operator[](const uint32_t i);
		Point<T, S> operator+(Point<T, S> other);
		Point<T, S> operator-(Point<T, S> other);
		Point<T, S> operator*(Point<T, S> other);
		Point<T, S> operator/(Point<T, S> other);
		void operator+=(Point<T, S> other);
		void operator-=(Point<T, S> other);
		void operator*=(Point<T, S> other);
		void operator/=(Point<T, S> other);
};

//TEMPLATE
//std::ostream& operator<<(std::ostream& out, Point<T, S>& point);

TEMPLATE
std::ostream& operator<<(std::ostream& out, Point<T, S> point);

#include "point.tpp"

typedef Point<float, 2> point2D;
typedef Point<float, 3> point3D;
typedef Point<float, 4> point4D;