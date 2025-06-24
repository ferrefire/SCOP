#include "manager.hpp"

#include <iostream>
#include <exception>

int main(int argc, char **argv)
{
	std::cout << "Program started" << std::endl << std::endl;

	try
	{
		Manager::Create();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Failed at creation: " << e.what() << std::endl;
	}

	while (!Manager::ShouldClose())
	{
		Manager::Frame();
	}
	
	Manager::Destroy();

	std::cout << "Program ended" << std::endl << std::endl;

	return (0);
}