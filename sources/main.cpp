#include "manager.hpp"

#include "point.hpp"

#include <iostream>
#include <exception>
#include <string>

int main(int argc, char **argv)
{
	std::cout << "Program started" << std::endl << std::endl;

	point3D p1({1, 2, 3});
	std::cout << p1 << std::endl;
	point3D p2({0, 1, 2});
	std::cout << p2 << std::endl;
	p1 += p2;
	std::cout << p1 << std::endl;
	point3D p3(2);
	std::cout << p3 << std::endl;
	p1 *= p3;
	std::cout << p1 << std::endl;
	p1 /= point3D(3);
	std::cout << p1 << std::endl;
	point3D p4 = (p1 + p2);
	std::cout << p4 << std::endl;
	p4.x += 10;
	std::cout << p4 << std::endl;
	std::cout << p1 << std::endl;
	std::cout << (p1 + p2) << std::endl;
	point3D p5(100);
	std::cout << p5 << std::endl;
	p5 = p1;
	std::cout << p5 << std::endl;
	point3D p6(p2);
	std::cout << p6 << std::endl;

	point2D p2d(50);
	std::cout << p2d << std::endl;

	point4D p4d({1234, 5678, 9101112, 13141516});
	std::cout << p4d << std::endl;

	exit(EXIT_SUCCESS);

	try
	{
		Manager::Create();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Failed at creation: " << e.what() << std::endl;

		Manager::Destroy();

		exit(EXIT_FAILURE);
	}

	while (!Manager::ShouldClose())
	{
		Manager::Frame();
	}
	
	Manager::Destroy();

	std::cout << "Program ended" << std::endl << std::endl;

	exit(EXIT_SUCCESS);
}