#include "manager.hpp"

#include "point.hpp"

#include <iostream>
#include <exception>

int main(int argc, char **argv)
{
	std::cout << "Program started" << std::endl << std::endl;

	Point<int, 3> point;
	std::cout << point << std::endl;
	point[0] = 3;
	std::cout << point << std::endl;
	point[1] = 10;
	point[2] = 55;
	std::cout << point << std::endl;
	point[2] /= 2;
	std::cout << point << std::endl;

	std::cout << std::endl;

	Point<float, 3> fpoint;
	std::cout << fpoint << std::endl;
	fpoint[0] = 3.0f;
	std::cout << fpoint << std::endl;
	fpoint[1] = 10.0f;
	fpoint[2] = 55.0f;
	std::cout << fpoint << std::endl;
	fpoint[2] /= 2;
	std::cout << fpoint << std::endl;

	std::cout << std::endl;

	Point<double, 3> dpoint;
	std::cout << dpoint << std::endl;
	dpoint[0] = 3.0;
	std::cout << dpoint << std::endl;
	dpoint[1] = 10.0;
	dpoint[2] = 55.0;
	std::cout << dpoint << std::endl;
	dpoint[2] /= 2;
	std::cout << dpoint << std::endl;

	std::cout << std::endl;

	point3D initPoint({1, 2, 3});
	std::cout << initPoint << std::endl;
	initPoint.x = 5;
	std::cout << initPoint << std::endl;

	std::cout << std::endl;

	//Point<float, 3> initArrPoint();
	//std::cout << initPoint << std::endl;

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