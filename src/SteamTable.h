#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/imgui-SFML.h>


template<size_t array_size>
void LoadSteamTable(std::vector<std::array<std::string, array_size>>& SteamTable, std::string fileName)
{
	std::string word, line;
	std::fstream file;
	file.open(fileName, std::ios::in);
	if (file.is_open())
	{
		int iterateVec = 0;
		while (std::getline(file, line))
		{
			std::stringstream str(line);
			int iterateArr = 0;
			SteamTable.emplace_back();
			while (std::getline(str, word, ','))
			{
				SteamTable.at(iterateVec).at(iterateArr) = word;
				iterateArr++;
			}
			iterateVec++;
		}
	}
	file.close();
}


template<size_t size>
void PressureList(std::shared_ptr<boiler<FEED::DOUBLE>> &boil, std::unique_ptr<std::vector<std::array<std::string, size>>> &Table)
{



	static std::vector<std::string> pressure_list;
	static bool once = [&Table]() {
		pressure_list.emplace_back("0.01");
		std::string previous_value = "0.01";
		for (int row = 1; row < Table->size(); row++)
		{
			if (Table->at(row).at(0) != previous_value)
			{
				pressure_list.emplace_back(Table->at(row).at(0));
				previous_value = Table->at(row).at(0);
			}
		}
		return true;
	}();

	//bool once2 = []()
	//	{
	//		for (int i = 0; i < pressure_list.size(); i++)
	//		{
	//			std::cout << pressure_list.at(i) << std::endl;
	//		}
	//		return true;
	//	} ();
	static std::string current_pressure2 = boil->ReturnPressure1Copy() + " MPa";
	static std::string current_pressure3 = boil->ReturnPressure2Copy() + " MPa";

	static std::string current_pressure1 = boil->ReturnPressure3Copy() + " MPa";
	//std::cout << "After: " << current_pressure1 << std::endl;

	if (ImGui::BeginCombo("Pressure Feed (1)", current_pressure1.c_str()))
	{

		for (int i = 0; i < pressure_list.size(); i++)
		{
			bool is_selected = (current_pressure1 == pressure_list.at(i));
			if (ImGui::Selectable(pressure_list.at(i).c_str(), is_selected))
			{
				current_pressure1 = pressure_list.at(i) + " MPa";
				boil->ReturnPressure1() = std::stof(pressure_list.at(i));
				return;
			}
			if (is_selected)
			{

			}
		}
		ImGui::EndCombo();
		return;
	}
	if (ImGui::BeginCombo("Pressure Feed (2)", current_pressure2.c_str()))
	{
		for (int i = 0; i < pressure_list.size(); i++)
		{
			bool is_selected = (current_pressure2 == pressure_list.at(i));
			if (ImGui::Selectable(pressure_list.at(i).c_str(), is_selected))
			{
				current_pressure2 = pressure_list.at(i) + " MPa";
				boil->ReturnPressure2() = std::stof(pressure_list.at(i));
				return;
			}
			if (is_selected)
			{

			}
		}
		ImGui::EndCombo();
		return;
	}
	if (ImGui::BeginCombo("Pressure Outlet", current_pressure3.c_str()))
	{
		for (int i = 0; i < pressure_list.size(); i++)
		{
			bool is_selected = (current_pressure3 == pressure_list.at(i));
			if (ImGui::Selectable(pressure_list.at(i).c_str(), is_selected))
			{
				current_pressure3 = pressure_list.at(i) + " MPa";
				boil->ReturnPressure3() = std::stof(pressure_list.at(i));
				return;
			}
			if (is_selected)
			{

			}
		}
		ImGui::EndCombo();
		return;
	}

	//std::cout << current_pressure1 << std::endl;
	//system("pause");
	return;

}





void SteamTableSimulation()
{
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






	if (ImGui::CollapsingHeader("Saturated Steam Table (Temperature)"))
	{
		static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 15>>>();
		static bool once = []() { LoadSteamTable(*SteamTable, "steam_tables/Saturated Steam Table (Temperature).csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();
		if (ImGui::BeginTable("table1", 15, flags))
		{
			ImGui::TableSetupScrollFreeze(false, true);

			// Display headers so we can inspect their interaction with borders.
			// (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)
			static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
			static bool display_headers = false;
			static int contents_type = CT_Text;

			enum ContentsType { CT_Text, CT_Button, CT_SmallButton, CT_FillButton, CT_Selectable, CT_SelectableSpanRow };


			ImGui::TableSetupColumn("T (C)");
			ImGui::TableSetupColumn("P (MPa)");
			ImGui::TableSetupColumn("Density Liquid (kg/m^3)");
			ImGui::TableSetupColumn("Density Vapor (kg/m^3)");
			ImGui::TableSetupColumn("Internal Energy Liquid (kJ/kg)");
			ImGui::TableSetupColumn("Internal Energy Vapor (kJ/kg)");
			ImGui::TableSetupColumn("Internal Energy of Vaporization (kJ/kg");
			ImGui::TableSetupColumn("Enthalpy Liquid (kj/kg)");
			ImGui::TableSetupColumn("Enthalpy Vapor (kJ/kg)");
			ImGui::TableSetupColumn("Enthalpy of Vaporization (kj/kg)");
			ImGui::TableSetupColumn("Entropy Liquid [kJ/(kg*K]");
			ImGui::TableSetupColumn("Entropy Vapor [kJ/(kg*k]");
			ImGui::TableSetupColumn("Entropy of Vaporization [kJ/(kg*k)]");
			ImGui::TableSetupColumn("Specific Volume Liquid (m^3/kg)");
			ImGui::TableSetupColumn("Specific Volume Vapor (m^3/kg)");
			ImGui::TableHeadersRow();

			static int value = 0;
			std::vector<bool> placeholder; placeholder.resize(SteamTable->size());
			for (int row = 0; row < SteamTable->size(); row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 15; column++)
				{
					ImGui::TableSetColumnIndex(column);
					if ((value == row) && (value != 0))
					{

						/*ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), column_color = 0);*/
						ImGui::TextColored(ImVec4(0.3, 0.9, 0.8, 0.9), SteamTable->at(row).at(column).c_str());
					}
					else if (ImGui::Selectable(SteamTable->at(row).at(column).c_str(), placeholder.at(row), ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
					{
						placeholder.at(row) = true;
						value = row;
					}

				}

			}
			ImGui::EndTable();
		}
	}

	if (ImGui::CollapsingHeader("Saturated Steam Table (Pressure)"))
	{
		static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 15>>>();
		static bool once = []() { LoadSteamTable(*SteamTable, "steam_tables/Saturated Steam Table (Pressure).csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();
		if (ImGui::BeginTable("table1", 15, flags))
		{
			ImGui::TableSetupScrollFreeze(false, true);

			// Display headers so we can inspect their interaction with borders.
			// (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)
			static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
			static bool display_headers = false;
			static int contents_type = CT_Text;

			enum ContentsType { CT_Text, CT_Button, CT_SmallButton, CT_FillButton, CT_Selectable, CT_SelectableSpanRow };

			ImGui::TableSetupColumn("P (MPa)");
			ImGui::TableSetupColumn("T (C)");
			ImGui::TableSetupColumn("Density Liquid (kg/m^3)");
			ImGui::TableSetupColumn("Density Vapor (kg/m^3)");
			ImGui::TableSetupColumn("Internal Energy Liquid (kJ/kg)");
			ImGui::TableSetupColumn("Internal Energy Vapor (kJ/kg)");
			ImGui::TableSetupColumn("Internal Energy of Vaporization (kJ/kg");
			ImGui::TableSetupColumn("Enthalpy Liquid (kj/kg)");
			ImGui::TableSetupColumn("Enthalpy Vapor (kJ/kg)");
			ImGui::TableSetupColumn("Enthalpy of Vaporization (kj/kg)");
			ImGui::TableSetupColumn("Entropy Liquid [kJ/(kg*K]");
			ImGui::TableSetupColumn("Entropy Vapor [kJ/(kg*k]");
			ImGui::TableSetupColumn("Entropy of Vaporization [kJ/(kg*k)]");
			ImGui::TableSetupColumn("Specific Volume Liquid (m^3/kg)");
			ImGui::TableSetupColumn("Specific Volume Vapor (m^3/kg)");
			ImGui::TableHeadersRow();

			static int value = 0;
			std::vector<bool> placeholder; placeholder.resize(SteamTable->size());
			for (int row = 0; row < SteamTable->size(); row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 15; column++)
				{
					ImGui::TableSetColumnIndex(column);
					if ((value == row) && (value != 0))
					{

						/*ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), column_color = 0);*/
						ImGui::TextColored(ImVec4(0.3, 0.9, 0.8, 0.9), SteamTable->at(row).at(column).c_str());
					}
					else if (ImGui::Selectable(SteamTable->at(row).at(column).c_str(), placeholder.at(row), ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
					{
						placeholder.at(row) = true;
						value = row;
					}

				}

			}
			ImGui::EndTable();
		}
	}

	if (ImGui::CollapsingHeader("Compressed Liquid and Superheated Steam Table"))
	{
		static std::unique_ptr<std::vector<std::array<std::string, 8>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 8>>>();
		static bool once = []() { LoadSteamTable(*SteamTable, "steam_tables/Compressed Liquid and Superheated Steam Table.csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();
		if (ImGui::BeginTable("table1", 8, flags))
		{
			ImGui::TableSetupScrollFreeze(false, true);

			// Display headers so we can inspect their interaction with borders.
			// (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)
			static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
			static bool display_headers = false;
			static int contents_type = CT_Text;

			enum ContentsType { CT_Text, CT_Button, CT_SmallButton, CT_FillButton, CT_Selectable, CT_SelectableSpanRow };

			ImGui::TableSetupColumn("P (MPa)");
			ImGui::TableSetupColumn("T (C)");
			ImGui::TableSetupColumn("Specific Volume (m^3/kg)");
			ImGui::TableSetupColumn("Density (kg/m^3)");
			ImGui::TableSetupColumn("Specific Internal Energy (kJ/kg");
			ImGui::TableSetupColumn("Specific Enthalpy (kJ/kg");
			ImGui::TableSetupColumn("Specific Entropy [kJ/(kg*K]");
			ImGui::TableSetupColumn("Phase");

			ImGui::TableHeadersRow();

			static int value2 = 0;
			std::vector<bool> placeholder; placeholder.resize(SteamTable->size());
			for (int row = 0; row < SteamTable->size(); row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 8; column++)
				{
					ImGui::TableSetColumnIndex(column);
					if ((value2 == row) && (value2 != 0))
					{

						/*ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), column_color = 0);*/
						ImGui::TextColored(ImVec4(0.3, 0.9, 0.8, 0.9), SteamTable->at(row).at(column).c_str());
					}
					else if (ImGui::Selectable(SteamTable->at(row).at(column).c_str(), placeholder.at(row), ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
					{
						placeholder.at(row) = true;
						value2 = row;
					}

				}

			}
			ImGui::EndTable();
		}
	}


}


enum class SteamTableFlag
{
	COMPRESSED_SUPERHEATED_TABLE = 0x01, SATURATED_TABLE = 0x02
};

enum class SteamTableCalculate
{
	PRESSURE = 0X01, TEMPERATURE = 0X02, DENSITY = 0X03, INTERNAL_ENERGY = 0x04, ENTHALPY = 0X05, ENTROPY = 0X06, SPECIFIC_VOLUME = 0x07
};

template<size_t size>
float CalculateFromSteamTable(std::unique_ptr<std::vector<std::array<std::string, size>>>& SteamTable, SteamTableFlag table, SteamTableCalculate calculate, float &pressure, float &temperature, float &enthalpy_target, Phase &phase)
{
	//float pressure = 0.11; //In MPA
	//float temperature = 1760; //in Celsius
	float SaturatedTemperature = 0;

	if (table == SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE)
	{
		for (int i = 1; i < SteamTable->size(); i++)
		{
			for (int j = 0; j < size - 1; j++)
			{
				if (pressure == std::stof(SteamTable->at(i).at(j)))
				{
					if (calculate == SteamTableCalculate::DENSITY)
					{
						if (temperature == std::stof(SteamTable->at(i).at(1)))
						{
							return std::stof(SteamTable->at(i).at(3));
						}
						else if ((temperature > std::stof(SteamTable->at(i).at(1)) && (temperature < std::stof(SteamTable->at(i + 1).at(1)))))
						{
							float lower_value_temp = std::stof(SteamTable->at(i).at(1)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(1));
							float lower_value_den = std::stof(SteamTable->at(i).at(3)); float upper_value_den = std::stof(SteamTable->at(i + 1).at(3));

							float real_den = lower_value_den + ((temperature - lower_value_temp) / (upper_value_temp - lower_value_temp)) * (upper_value_den - lower_value_den);
							phase.phase = SteamTable->at(i + 1).at(7);
							return real_den;

						}
					}
					if (calculate == SteamTableCalculate::ENTHALPY)
					{
						if ((temperature == std::stof(SteamTable->at(i).at(1)) && ((phase.phase != " Saturated Liquid") && (phase.phase != " Saturated Vapor"))))
						{
							return std::stof(SteamTable->at(i).at(5));
						}
						else if ((temperature == std::stof(SteamTable->at(i).at(1)))  && ((phase.phase == " Saturated Liquid") or (phase.phase == " Saturated Vapor")))
						{
							float quality = 0, lower_enthalpy = std::stof(SteamTable->at(i).at(5)), higher_enthalpy = std::stof(SteamTable->at(i + 1).at(5));
							quality = (enthalpy_target - lower_enthalpy) / (higher_enthalpy - lower_enthalpy);
							phase.quality = quality;
							return  enthalpy_target;
						}
						else if ((temperature > std::stof(SteamTable->at(i).at(1)) && (temperature < std::stof(SteamTable->at(i + 1).at(1)))))
						{

							float lower_value_temp = std::stof(SteamTable->at(i).at(1)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(1));
							float lower_value_target = std::stof(SteamTable->at(i).at(5)); float upper_value_target = std::stof(SteamTable->at(i + 1).at(5));

							float real_den = lower_value_target + ((temperature - lower_value_temp) / (upper_value_temp - lower_value_temp)) * (upper_value_target - lower_value_target);


							phase.phase = SteamTable->at(i).at(7);

							if (((phase.phase == " Saturated Vapor") or (phase.phase == " Saturated Liquid")) && (temperature > std::stof(SteamTable->at(i).at(1))))
							{
								phase.phase = SteamTable->at(i + 1).at(7);
							}

							return real_den;
						}

					}
					if (calculate == SteamTableCalculate::TEMPERATURE)
					{
						if (enthalpy_target == std::stof(SteamTable->at(i).at(5)))
						{
							return std::stof(SteamTable->at(i).at(1));
						}
						else if ((enthalpy_target > std::stof(SteamTable->at(i).at(5)) && (enthalpy_target < std::stof(SteamTable->at(i + 1).at(5)))))
						{
							float lower_value_temp = std::stof(SteamTable->at(i).at(5)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(5));
							float lower_value_target = std::stof(SteamTable->at(i).at(1)); float upper_value_target = std::stof(SteamTable->at(i + 1).at(1));


							float real_den = lower_value_target + ((enthalpy_target - lower_value_temp) / (upper_value_temp - lower_value_temp)) * (upper_value_target - lower_value_target);
							if ((phase.phase == " Saturated Vapor") && (enthalpy_target > std::stof(SteamTable->at(i).at(5))))
							{
								phase.phase = SteamTable->at(i + 1).at(7);
							}
							else
								phase.phase = SteamTable->at(i).at(7);
							return real_den;

						}
					}
					if (calculate == SteamTableCalculate::SPECIFIC_VOLUME)
					{
						if ((temperature == std::stof(SteamTable->at(i).at(1)) && ((phase.phase != " Saturated Liquid") && (phase.phase != " Saturated Vapor"))))
						{
							return std::stof(SteamTable->at(i).at(2));
						}
						else if ((temperature == std::stof(SteamTable->at(i).at(1))) && ((phase.phase == " Saturated Liquid") or (phase.phase == " Saturated Vapor")))
						{
							float lower_vol = std::stof(SteamTable->at(i).at(2)), higher_vol = std::stof(SteamTable->at(i + 1).at(2));
							float real_vol = lower_vol + phase.quality * (higher_vol - lower_vol);
							return  real_vol;
						}
						else if ((temperature > std::stof(SteamTable->at(i).at(1)) && (temperature < std::stof(SteamTable->at(i + 1).at(1)))))
						{
							float lower_value_temp = std::stof(SteamTable->at(i).at(1)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(1));
							float lower_value_den = std::stof(SteamTable->at(i).at(2)); float upper_value_den = std::stof(SteamTable->at(i + 1).at(2));

							float real_vol = lower_value_den + ((temperature - lower_value_temp) / (upper_value_temp - lower_value_temp)) * (upper_value_den - lower_value_den);
							return real_vol;

						}
					}
				}
			}
		}
	}



}