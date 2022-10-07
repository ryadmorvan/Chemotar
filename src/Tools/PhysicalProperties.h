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
	std::unique_ptr<std::vector<std::array<std::string, 6>>> HeatCapacitiesVector;
	void load_table(std::string file_name);
public:
	enum class type {Default = 0x01, Elliot = 0x02, PhysicalProperty = 0x03, HeatCapaicty = 0x04};
	static void ShowPhysicalProperties(bool* p_open);
	PhysicalProperties(type property = type::Default);

	//template<size_t PhysicalPropertiesSize, size_t HeatCapacitySize>
	void load_table_custom(std::string file_name1, std::string file_name2);
	auto& returnTable(type table) {return (table == type::PhysicalProperty) ? PhysicalPropertiesVector : HeatCapacitiesVector;}
};

