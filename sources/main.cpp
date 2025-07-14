#include "manager.hpp"

#include <iostream>
#include <exception>
#include <string>

int main(int argc, char **argv)
{
	try
	{
		Manager::ParseArguments(argv, argc);
		Manager::Create();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Failed at creation: " << e.what() << std::endl;

		Manager::Destroy();

		exit(EXIT_FAILURE);
	}

	Manager::Start();

	while (!Manager::ShouldClose())
	{
		Manager::Frame();
	}
	
	Manager::Destroy();

	std::cout << "Program ended" << std::endl << std::endl;

	exit(EXIT_SUCCESS);
}