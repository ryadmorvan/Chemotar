#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "ChemicalCalculations.h"




std::vector<std::string> loadTable(std::string fname, std::fstream &file)
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
bool CheckBoxUI(bool* CheckBox, std::string& fname, std::fstream &file, std::string &specie)
{
	ImGui::Checkbox("Elliot Liquid", &CheckBox[0]);
	ImGui::Checkbox("Gas Enthalpy", &CheckBox[1]);
	ImGui::Checkbox("Liquid Enthalpy", &CheckBox[2]);
	static std::vector<std::string> ChemicalsVector;
	const int vectorSize = ChemicalsVector.size();
	if (CheckBox[0] && ((CheckBox[1] == 0) && (CheckBox[2] == 0)))
	{
		fname = "ElliotLiquid.csv";
		static int ChemicalSpecie = 0;

		if (TabeLoaded == 0)
		{
			ChemicalsVector = loadTable(fname, file);
			TabeLoaded = 1;
		}
		static const char* Chemicals[16] = {};

		for (int i = 0; i < 16; ++i)
		{
			Chemicals[i] = ChemicalsVector.at(i).c_str();
		}

		ImGui::Combo("Chemical Species", &ChemicalSpecie, Chemicals, IM_ARRAYSIZE(Chemicals), 8);
		specie = ChemicalsVector.at(ChemicalSpecie);
		return 1;
	}
	else if (CheckBox[1] && ((CheckBox[0] == 0) && (CheckBox[2] == 0)))
	{
		fname = "GasEnthalpy.csv";
		static int ChemicalSpecie = 0;
		if (TabeLoaded == 0)
		{
			ChemicalsVector = loadTable(fname, file);
			TabeLoaded = 1;
		}
		static const char* Chemicals[73] = {};

		for (int i = 0; i < 73; ++i)
		{
			Chemicals[i] = ChemicalsVector.at(i).c_str();
		}

		ImGui::Combo("Chemical Species", &ChemicalSpecie, Chemicals, IM_ARRAYSIZE(Chemicals), 8);
		specie = ChemicalsVector.at(ChemicalSpecie);
		return 1;
	}
	else if (CheckBox[2] && ((CheckBox[1] == 0) && (CheckBox[0] == 0)))
	{
		fname = "LiquidEnthalpy.csv";
		static int ChemicalSpecie = 0;
		if (TabeLoaded == 0)
		{
			ChemicalsVector = loadTable(fname, file);
			TabeLoaded = 1;
		}
		static const char* Chemicals[74] = {};

		for (int i = 0; i < 74; ++i)
		{
			Chemicals[i] = ChemicalsVector.at(i).c_str();
		}

		ImGui::Combo("Chemical Species", &ChemicalSpecie, Chemicals, IM_ARRAYSIZE(Chemicals), 8);
		specie = ChemicalsVector.at(ChemicalSpecie);
		return 1;
	}
	else
	{
		TabeLoaded = 0;
		specie = "NULL";
		return 0;
	}
}