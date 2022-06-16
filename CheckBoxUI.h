#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <map>
#include "ChemicalCalculations.h"


struct TemperatureRange
{
	float min = 0;
	float max = 0;
};

std::vector<std::string> loadTable(std::string fname, std::fstream &file, std::map<int, TemperatureRange> &TempRange)
{
	std::string word, line;
	std::vector<std::string> Chemicals;
	file.open(fname, std::ios::in);
	if (file.is_open())
	{
		int iterateMap = 0;
		while (std::getline(file, line))
		{
			std::stringstream str(line);
			int i = 0;
			double min = 0;
			double max = 0;
			while (std::getline(str, word, ','))
			{
				std::string placeholder = word;
				switch (i)
				{
				case 0:
					Chemicals.push_back(placeholder);
					break;
				case 7:
					min = std::stod(placeholder);
					break;
				case 8:
					max = std::stod(placeholder);
					break;
				}
				++i;
			}
			TempRange[iterateMap].max = max;
			TempRange[iterateMap].min = min;
			++iterateMap;
		}
		//for (int i = 0; i < Chemicals.size(); ++i)
		//{
		//	std::cout << Chemicals.at(i) << std::endl;
		//}
	}
	else
		std::cout << "Failed to Open" << std::endl;
	file.close();
	return Chemicals;
}
std::vector<std::string> loadTableNames(std::string fname, std::fstream& file)
{
	std::string word, line;
	std::vector<std::string> Chemicals;
	file.open(fname, std::ios::in);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::stringstream str(line);
			int i = 0;
			while (std::getline(str, word, ','))
			{
				std::string placeholder = word;
				switch (i)
				{
				case 0:
					Chemicals.push_back(placeholder);
				}
				++i;
			}

		}
		//for (int i = 0; i < Chemicals.size(); ++i)
		//{
		//	std::cout << Chemicals.at(i) << std::endl;
		//}
	}
	else
		std::cout << "Failed to Open" << std::endl;
	file.close();
	return Chemicals;
}

bool TabeLoaded = 0;
bool CheckBoxUI(bool* CheckBox, std::string& fname, std::fstream &file, std::string &specie, float &tempMin, float &tempMax)
{
	ImGui::Checkbox("Elliot Liquid", &CheckBox[0]);
	ImGui::Checkbox("Gas Enthalpy", &CheckBox[1]);
	ImGui::Checkbox("Liquid Enthalpy", &CheckBox[2]);
	static std::vector<std::string> ChemicalsVector;
	static std::map<int, TemperatureRange> TempRange;
	const int vectorSize = ChemicalsVector.size();
	if (CheckBox[0] && ((CheckBox[1] == 0) && (CheckBox[2] == 0)))
	{
		fname = "Empirical Tables\\ElliotLiquid.csv";
		static int ChemicalSpecie = 0;

		if (TabeLoaded == 0)
		{
			ChemicalsVector = loadTable(fname, file, TempRange);
			TabeLoaded = 1;
		}
		static const char* Chemicals[16] = {};

		for (int i = 0; i < 16; ++i)
		{
			Chemicals[i] = ChemicalsVector.at(i).c_str();
		}

		ImGui::Combo("Chemical Species", &ChemicalSpecie, Chemicals, IM_ARRAYSIZE(Chemicals), 8);
		specie = ChemicalsVector.at(ChemicalSpecie);
		tempMin = TempRange.at(ChemicalSpecie).min;
		tempMax = TempRange.at(ChemicalSpecie).max;
		return 1;
	}
	else if (CheckBox[1] && ((CheckBox[0] == 0) && (CheckBox[2] == 0)))
	{
		fname = "Empirical Tables\\GasEnthalpy.csv";
		static int ChemicalSpecie = 0;
		if (TabeLoaded == 0)
		{
			ChemicalsVector = loadTable(fname, file, TempRange);
			TabeLoaded = 1;
		}
		static const char* Chemicals[75] = {};

		for (int i = 0; i < 75; ++i)
		{
			Chemicals[i] = ChemicalsVector.at(i).c_str();
		}

		ImGui::Combo("Chemical Species", &ChemicalSpecie, Chemicals, IM_ARRAYSIZE(Chemicals), 8);
		specie = ChemicalsVector.at(ChemicalSpecie);
		tempMin = TempRange.at(ChemicalSpecie).min;
		tempMax = TempRange.at(ChemicalSpecie).max;
		return 1;
	}
	else if (CheckBox[2] && ((CheckBox[1] == 0) && (CheckBox[0] == 0)))
	{
		fname = "Empirical Tables\\LiquidEnthalpy.csv";
		static int ChemicalSpecie = 0;
		if (TabeLoaded == 0)
		{
			ChemicalsVector = loadTable(fname, file, TempRange);
			TabeLoaded = 1;
		}
		static const char* Chemicals[76] = {};

		for (int i = 0; i < 76; ++i)
		{
			Chemicals[i] = ChemicalsVector.at(i).c_str();
		}

		ImGui::Combo("Chemical Species", &ChemicalSpecie, Chemicals, IM_ARRAYSIZE(Chemicals), 8);
		specie = ChemicalsVector.at(ChemicalSpecie);
		tempMin = TempRange.at(ChemicalSpecie).min;
		tempMax = TempRange.at(ChemicalSpecie).max;
		return 1;
	}
	else
	{
		TabeLoaded = 0;
		specie = "";
		tempMin = NULL;
		tempMax = NULL;

		return 0;
	}
	tempMin = NULL;
	tempMax = NULL;
}