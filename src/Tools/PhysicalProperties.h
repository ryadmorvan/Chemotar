#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <sstream>






class PhysicalProperties
{
	std::string filePath;
	std::unique_ptr<std::vector<std::array<std::string, 6>>> PhysicalPropertiesVector;
	void load_table(std::string file_name);
public:

	static void ShowPhysicalProperties(bool* p_open);
	PhysicalProperties();


	auto& returnTable() {return PhysicalPropertiesVector;}
};

