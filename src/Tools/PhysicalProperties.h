#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>






template<size_t ArraySize>
class PhysicalProperties
{
	std::vector<std::array<std::string, ArraySize>> PhysicalPropertiesVector;
	inline void load_table(std::string file_name);
	static void ShowPhysicalProperties();

};

