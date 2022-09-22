#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <math.h>

template<typename T1, typename T2>
std::string _Format(T1 f, T2 n);


class VaporPressureCalculator
{
	std::string filePath;
	std::unique_ptr<std::vector<std::array<std::string, 6>>> AntoineConstants;
public:
	static void ShowVaporCalculator(bool *p_open);
	float CalculateVaporPressure(float temperature, unsigned specie_number, float Antoine_A, float Antoine_B, float Antoine_C);
	VaporPressureCalculator();
};

