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





bool _AddTable(std::string &filePath, TABLES_SAVE_DATA DATA)
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
		std::fstream outfile;
		if (DATA == TABLES_SAVE_DATA::HEAT_CAPACITY)
		{
			outfile.open("table_data/Heat Capacity Tables.ini", std::fstream::in | std::fstream::out | std::fstream::app);
		}
		if (DATA == TABLES_SAVE_DATA::VISCOSITY)
		{
			outfile.open("table_data/Viscosity Tables.ini", std::fstream::in | std::fstream::out | std::fstream::app);
		}

		if (_Find_File(sSelectedFile, DATA))
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

std::vector<std::string> loadTable(std::string fname, std::fstream &file, std::map<int, TemperatureRange> &TempRange, int &vectorSize, TABLES_SAVE_DATA DATA)
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
			float min = 0;
			float max = 0;
			while (std::getline(str, word, ','))
			{
				std::string placeholder = word;
				if (DATA == TABLES_SAVE_DATA::HEAT_CAPACITY)
				{
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
				}
				if (DATA == TABLES_SAVE_DATA::VISCOSITY)
				{
					switch (i)
					{
					case 0:
						Chemicals.push_back(placeholder);
						break;
					case 5:
						min = std::stod(placeholder);
						break;
					case 6:
						max = std::stod(placeholder);
						break;
					}
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

template<typename FLOATING>//accepts floats, doubles... etc
bool CheckBoxUI(bool* CheckBox, std::string& fname, std::fstream& file, std::string& specie, FLOATING& tempMin, FLOATING& tempMax, std::string& filePath, FileInfo &TableInfo)
{
	static bool _TabeLoaded = 0;
	static int _vectorSize = 0; //Vector Size for different files
	std::vector<std::string> TableNames;
	std::vector<std::string> TablePaths;
	std::fstream SaveFile;
	SaveFile.open("table_data/Heat Capacity Tables.ini", std::ios::in);
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
				ChemicalsVector = loadTable(fname, file, TempRange, _vectorSize, TABLES_SAVE_DATA::HEAT_CAPACITY);
				_TabeLoaded = 1;
				current_item = "Choose";
			}
			//Set up the fileInfo
			TableInfo.fileName = TableNames.at(i) + ".csv";
			TableInfo.filePath = TablePaths.at(i);

			

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
}




template<typename FLOATING> //accepts floats, doubles... etc
bool CheckBoxUI(bool* CheckBox, std::string& fname, std::fstream& file, std::string& specie, FLOATING& tempMin, FLOATING& tempMax, FLOATING& tempCrit,  std::string& filePath, FileInfo& TableInfo)
{
	static bool _TabeLoaded = 0;
	static int _vectorSize = 0; //Vector Size for different files
	std::vector<std::string> TableNames;
	std::vector<std::string> TablePaths;
	std::fstream SaveFile;
	SaveFile.open("table_data/Viscosity Tables.ini", std::ios::in);
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
				ChemicalsVector = loadTable(fname, file, TempRange, _vectorSize, TABLES_SAVE_DATA::VISCOSITY);
				_TabeLoaded = 1;
				current_item = "Choose";
			}
			//Set up the fileInfo
			TableInfo.fileName = TableNames.at(i) + ".csv";
			TableInfo.filePath = TablePaths.at(i);



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
}




void EnthalpyClaculatorInfo()
{
	if (ImGui::BeginPopup("Info Enthalpy"))
	{
		ImGui::Text("USER GUIDE: ");
		ImGui::Text("Addition of Table");
		ImGui::BulletText("Click on (File) from the main menu and hover on (Add)");
		ImGui::BulletText("Select the appropriate table to be added.");
		ImGui::BulletText("Tables file must be converted from .xlsx to .csv ");
		ImGui::BulletText("If there is no value for a coefficent, the appropriate cell must be set to 0 ");
		ImGui::BulletText("Heat Capacity tables must follow this format");

		enum ContentsType { CT_Text, CT_FillButton };
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
		static int contents_type = CT_Text;
		//Table Flags
		//flags |= ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH
		//	| ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner;
		//flags |= ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable;

		flags |= ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
			| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_SizingFixedFit;

		if (ImGui::BeginTable("Demonstration", 9, flags))
		{
			ImGui::TableSetupScrollFreeze(false, true);
			static bool display_headers = false;
			static int contents_type = CT_Text;
			ImGui::TableSetupColumn("Chemical");
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("A");
			ImGui::TableSetupColumn("B");
			ImGui::TableSetupColumn("C");
			ImGui::TableSetupColumn("D");
			ImGui::TableSetupColumn("E");
			ImGui::TableSetupColumn("Minimum Temperature (K)");
			ImGui::TableSetupColumn("Maximum Temperature (K)");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); ImGui::Text("C3H6O (Acetone)");
			ImGui::TableSetColumnIndex(1); ImGui::Text("Acetone");
			ImGui::TableSetColumnIndex(2); ImGui::Text("135.600");
			ImGui::TableSetColumnIndex(3); ImGui::Text("-0.177");
			ImGui::TableSetColumnIndex(4); ImGui::Text("0.000283");
			ImGui::TableSetColumnIndex(5); ImGui::Text("0.000000689");
			ImGui::TableSetColumnIndex(6); ImGui::Text("0");
			ImGui::TableSetColumnIndex(7); ImGui::Text("273.15");
			ImGui::TableSetColumnIndex(8); ImGui::Text("373.15");



			ImGui::EndTable();
		}

		ImGui::EndPopup();
	}
}

void ViscosityClaculatorInfo()
{
	if (ImGui::BeginPopup("Info Viscosity"))
	{
		ImGui::Text("USER GUIDE: ");
		ImGui::Text("Addition of Table");
		ImGui::BulletText("Click on (File) from the main menu and hover on (Add)");
		ImGui::BulletText("Select the appropriate table to be added.");
		ImGui::BulletText("Tables file must be converted from .xlsx to .csv ");
		ImGui::BulletText("If there is no value for a coefficent, the appropriate cell must be set to 0 ");
		ImGui::BulletText("Viscosity tables must follow this format");

		enum ContentsType { CT_Text, CT_FillButton };
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
		static int contents_type = CT_Text;
		//Table Flags
		//flags |= ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH
		//	| ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner;
		//flags |= ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable;

		flags |= ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
			| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_SizingFixedFit;

		if (ImGui::BeginTable("Demonstration", 7, flags))
		{
			ImGui::TableSetupScrollFreeze(false, true);
			static bool display_headers = false;
			static int contents_type = CT_Text;
			ImGui::TableSetupColumn("Chemical");
			ImGui::TableSetupColumn("A");
			ImGui::TableSetupColumn("B");
			ImGui::TableSetupColumn("C");
			ImGui::TableSetupColumn("D");
			ImGui::TableSetupColumn("Minimum Temperature (K)");
			ImGui::TableSetupColumn("Maximum Temperature (K)");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); ImGui::Text("Acetic acid (C2H4O2)");
			ImGui::TableSetColumnIndex(1); ImGui::Text("-28.66");
			ImGui::TableSetColumnIndex(2); ImGui::Text("0.2351");
			ImGui::TableSetColumnIndex(3); ImGui::Text("0.0002208");
			ImGui::TableSetColumnIndex(4); ImGui::Text("0");
			ImGui::TableSetColumnIndex(5); ImGui::Text("366");
			ImGui::TableSetColumnIndex(6); ImGui::Text("523");




			ImGui::EndTable();
		}

		ImGui::EndPopup();
	}
}

//Code to execute the window
inline void EnthalpyCalculator(bool &ShowPropertiesCalculator)
{
	static std::string fname = "";
	static std::string species;
	static float temperature1 = 0;
	static float temperature2 = 0;
	static float MinTemp = 0;
	static float MaxTemp = 0;
	static float CritTemp = 0;
	static std::string line, word;
	static std::fstream file;
	static FileInfo TableName;
	static bool tableLoaded = 0;
	static bool CheckBox[500];



	static ImVec4 TextColor = ImVec4(0.898f, 0.845f, 0.710f, 0.95f);

	//clean checkBoxes initally
	static bool clean = []()
	{
		memset(CheckBox, 0, sizeof(CheckBox));
		return true;
	} ();

	static std::stringstream results;
	static std::string finalResult = "";
	static std::string filePath = " ";




	if (ImGui::Button("INFO")) ImGui::OpenPopup("Info Enthalpy");
	EnthalpyClaculatorInfo();






	if (ShowPropertiesCalculator == TRUE)
	{
		if (CheckBoxUI(CheckBox, fname, file, species, MinTemp, MaxTemp, filePath, TableName))
		{
			ImGui::InputFloat("Input Inital Temperature", &temperature1);
			ImGui::InputFloat("Input Final Temperature", &temperature2);
			ImGui::SliderFloat("Inital Temperature", &temperature1, MinTemp, MaxTemp);
			ImGui::SliderFloat("Final Temperature", &temperature2, MinTemp, MaxTemp);

			if (ImGui::Button("Calculate"))
			{
				finalResult.clear();
				results = CalculateEnthalpy(species, temperature1, temperature2, file, line, word, fname);
				insertInfo(results, finalResult);
			}
			if (ImGui::Button("Remove Table"))
			{
				memset(CheckBox, 0, sizeof(CheckBox));
				_Find_File_Delete(TableName, TABLES_SAVE_DATA::HEAT_CAPACITY);
			}
		}
		else
		{
			finalResult.clear(); species = "";
		}
		ImGui::TextColored(TextColor, finalResult.c_str());
	}
	else
	{
		temperature1 = 0, temperature2 = 0, memset(CheckBox, FALSE, sizeof(CheckBox));
		finalResult = "";
	}
}

void ViscosityCalculator(bool& ShowViscosityCalculator)
{
	static std::string fname = "";
	static std::string species;
	static float temperature1 = 0;
	static float temperature2 = 0;
	static float MinTemp = 0;
	static float MaxTemp = 0;
	static float CritTemp = 0;
	static std::string line, word;
	static std::fstream file;
	static FileInfo TableName;
	static bool tableLoaded = 0;
	static bool CheckBox[500];
	

	//Text Color
	static ImVec4 TextColorNotif = ImVec4(0.725f, 0.292f, 0.376f, 0.95f);
	static ImVec4 TextColorStat = ImVec4(0.898f, 0.845f, 0.710f, 0.95f);
	//clean checkBoxes initally
	static bool clean = []()
	{
		memset(CheckBox, 0, sizeof(CheckBox));
		return true;
	} ();

	static std::stringstream results;
	static std::string finalResult = "";
	static std::string filePath = " ";



	if (ImGui::Button("INFO")) ImGui::OpenPopup("Info Viscosity");
	ViscosityClaculatorInfo();


	if (ShowViscosityCalculator == TRUE)
	{
		if (CheckBoxUI(CheckBox, fname, file, species, MinTemp, MaxTemp, CritTemp,  filePath, TableName))
		{
			ImGui::InputFloat("Input Inital Temperature", &temperature1);
			//ImGui::InputFloat("Input Final Temperature", &temperature2);
			ImGui::SliderFloat("Inital Temperature", &temperature1, MinTemp, MaxTemp);
			//ImGui::SliderFloat("Final Temperature", &temperature2, MinTemp, MaxTemp);

			if (ImGui::Button("Calculate"))
			{
				finalResult.clear();
				results = CalculateViscosity(species, temperature1, file, line, word, fname);
				insertInfo(results, finalResult);
			}
			if (ImGui::Button("Remove Table"))
			{
				memset(CheckBox, 0, sizeof(CheckBox));
				_Find_File_Delete(TableName, TABLES_SAVE_DATA::VISCOSITY);
			}
			ImGui::TextColored(TextColorNotif, "cP: CentiPoise\n");
		}
		else
		{
			finalResult.clear(); species = "";
		}
		ImGui::TextColored(TextColorStat, finalResult.c_str());
	}
	else
	{
		temperature1 = 0, temperature2 = 0, memset(CheckBox, FALSE, sizeof(CheckBox));
		finalResult = "";
	}
}



