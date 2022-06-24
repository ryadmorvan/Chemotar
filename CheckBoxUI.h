#pragma once

#include <iostream>
#include <Windows.h>
#include <ShObjIdl.h>
#include <vector>
#include <fstream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <map>
#include <algorithm>
#include "ChemicalCalculations.h"
#include "Windows_FileHandling.h"



bool _AddTable(std::string &filePath)
{
	bool result = FALSE;
	std::string sSelectedFile; //Storing the file name
	std::string sFilePath; //Stores the path
	result = _openFile(sSelectedFile, sFilePath); //After opening the file the it will return boolean
	std::replace(sFilePath.begin(), sFilePath.end(), '\\', '/');
	filePath = sFilePath;
	if (result)
	{
		std::cout << "File name:" << sSelectedFile << " File Path: " << sFilePath << " " << std::endl;
		std::fstream outfile("save.ini", std::fstream::in | std::fstream::out | std::fstream::app);
		if (_Find_File(sSelectedFile))
		{
			//Append changes to the file after opening it 
			sSelectedFile.append("\n");
			sFilePath.append("\n");
			outfile.write(sSelectedFile.data(), sSelectedFile.size());
			outfile.write(sFilePath.data(), sFilePath.size());
		}
		outfile.close();
	}
	else
		std::cout << "Failed to Open" << std::endl;

}












struct TemperatureRange
{
	float min = 0;
	float max = 0;
};

std::vector<std::string> loadTable(std::string fname, std::fstream &file, std::map<int, TemperatureRange> &TempRange, int &vectorSize)
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
		vectorSize = iterateMap;
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



void _LoadTables(std::vector<std::string>& TableNames, std::vector<std::string> &TablePaths, std::fstream& SaveFile)
{
	int count = 0;
	std::string line;
	while (std::getline(SaveFile, line))
	{
		if (count % 2 == 0)
		{
			TableNames.push_back(line);
		}
		else
			TablePaths.push_back(line);
		++count;
	}
}




bool _TabeLoaded = 0;
int _vectorSize = 0; //Vector Size for different files
bool CheckBoxUI(bool* CheckBox, std::string& fname, std::fstream& file, std::string& specie, float& tempMin, float& tempMax, std::string& filePath)
{
	std::vector<std::string> TableNames;
	std::vector<std::string> TablePaths;
	std::fstream SaveFile;
	SaveFile.open("save.ini", std::ios::in);
	_LoadTables(TableNames, TablePaths, SaveFile);
	_TableNamesCorrection(TableNames);

	int _countNumberofBools = 0;
	for (int i = 0; i < TableNames.size(); ++i)
	{
		ImGui::Checkbox(TableNames.at(i).c_str(), &CheckBox[i]);
		if (CheckBox[i] == 0)
		{
			_countNumberofBools++;
		}
	}
	if (_countNumberofBools == TableNames.size())
	{
		_TabeLoaded = 0;
	}
	SaveFile.close();

	static std::vector<std::string> ChemicalsVector;
	static std::map<int, TemperatureRange> TempRange;



	for (int i = 0; i < TableNames.size(); ++i)
	{
		static const char* current_item = "Choose";
		if (CheckBox[i])
		{
			fname = TablePaths.at(i);
			static int ChemicalSpecie = 0;
			if (_TabeLoaded == 0)
			{
				ChemicalsVector = loadTable(fname, file, TempRange, _vectorSize);
				_TabeLoaded = 1;
				current_item = "Choose";
			}
				
			

			if (ImGui::BeginCombo("Chemicals", current_item)) // The second parameter is the label previewed before opening the combo.
			{
				for (int n = 0; n < _vectorSize; n++)
				{
					bool is_selected = (current_item == ChemicalsVector.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(ChemicalsVector.at(n).c_str(), is_selected))
					{
						current_item = ChemicalsVector.at(n).c_str();
						specie = ChemicalsVector.at(n);
						tempMax = TempRange.at(n).max;
						tempMin = TempRange.at(n).min;
						return 1;
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
				ImGui::EndCombo();
			}
			return 1;
		}
		else
		{
			//_TabeLoaded = 0;
			//break;
			//memset(CheckBox, NULL, sizeof(CheckBox));
			//std::cout << "Table didn't load properly" << std::endl;
		}
	}

	return 0;








	/*if (CheckBox[0] && ((CheckBox[1] == 0) && (CheckBox[2] == 0)))
	{
		fname = "C:/Users/riyad/source/repos/Chemicals/ChemicalsSfml/Empirical Tables/ElliotLiquid.csv";
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
		fname = "C:\\Users\\riyad\\source\\repos\\Chemicals\\ChemicalsSfml\\Empirical Tables\\GasEnthalpy.csv";
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
		fname = "C:\\Users\\riyad\\source\\repos\\Chemicals\\ChemicalsSfml\\Empirical Tables\\LiquidEnthalpy.csv";
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
	tempMax = NULL;*/
}












