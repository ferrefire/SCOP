#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#define CI(a) static_cast<int>(a)
#define CUI(a) static_cast<uint32_t>(a)

class Utilities
{
	private:
		

	public:
		static std::string GetPath();
		static std::vector<char> FileToBinary(std::string path);
};