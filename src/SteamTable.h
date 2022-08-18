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



enum class SteamTableFlag
{
	COMPRESSED_SUPERHEATED_TABLE = 0x01, SATURATED_TABLE = 0x02
};

enum class SteamTableCalculate
{
	PRESSURE = 0X01, TEMPERATURE = 0X02, DENSITY = 0X03, INTERNAL_ENERGY = 0x04, ENTHALPY = 0X05, ENTROPY = 0X06, SPECIFIC_VOLUME = 0x07
};

namespace SteamTableCalculateFlags
{
	struct ENTHALPY;
	struct INTERNAL_ENERGY;
	struct ENTROPY;
	struct DENSITY;
	struct SPECIFIC_VOLUME;
}


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
	static std::string current_pressure1 = boil->ReturnPressure1Copy() + " MPa";
	static std::string current_pressure2 = boil->ReturnPressure2Copy() + " MPa";

	static std::string current_pressure3 = boil->ReturnPressure3Copy() + " MPa";
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


template<typename CalculateFlag, size_t size>

//std::tuple<quality, pressure, Enthalpy, Internal Energy, Entropy, Density, Specific Volume>
std::tuple<float, float, float, float, float, float, float> CalculateQuality(std::unique_ptr<std::vector<std::array<std::string, size>>> &SteamTable, int* temperature, float* target)
{
	if constexpr (std::is_same<CalculateFlag, SteamTableCalculateFlags::ENTHALPY>())
	{
		for (int i = 1; i < SteamTable->size(); i++)
		{
			if ((std::stof(SteamTable->at(i).at(0)) == *temperature))
			{
				float lower_value = std::stof(SteamTable->at(i).at(7)); float upper_value = std::stof(SteamTable->at(i).at(8));
				float quality = (*target - lower_value) / (upper_value - lower_value);
				float enthalpy = *target;
				float InternalEnergy = std::stof(SteamTable->at(i).at(4)) + quality * (std::stof(SteamTable->at(i).at(5)) - std::stof(SteamTable->at(i).at(4)));
				float Entropy = std::stof(SteamTable->at(i).at(10)) + quality * (std::stof(SteamTable->at(i).at(11)) - std::stof(SteamTable->at(i).at(10)));
				float Density = std::stof(SteamTable->at(i).at(2)) + quality * (std::stof(SteamTable->at(i).at(3)) - std::stof(SteamTable->at(i).at(2)));
				float SpecificVolume = std::stof(SteamTable->at(i).at(13)) + quality * (std::stof(SteamTable->at(i).at(14)) - std::stof(SteamTable->at(i).at(13)));
				return std::make_tuple(quality, std::stof(SteamTable->at(i).at(1)), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);

			}
		}
	}
	if constexpr (std::is_same<CalculateFlag, SteamTableCalculateFlags::INTERNAL_ENERGY>())
	{
		for (int i = 1; i < SteamTable->size(); i++)
		{
			if ((std::stof(SteamTable->at(i).at(0)) == *temperature))
			{
				float lower_value = std::stof(SteamTable->at(i).at(4)); float upper_value = std::stof(SteamTable->at(i).at(5));
				float quality = (*target - lower_value) / (upper_value - lower_value);
				float enthalpy = std::stof(SteamTable->at(i).at(7)) + quality * (std::stof(SteamTable->at(i).at(8)) - std::stof(SteamTable->at(i).at(7)));
				float InternalEnergy = *target;
				float Entropy = std::stof(SteamTable->at(i).at(10)) + quality * (std::stof(SteamTable->at(i).at(11)) - std::stof(SteamTable->at(i).at(10)));
				float Density = std::stof(SteamTable->at(i).at(2)) + quality * (std::stof(SteamTable->at(i).at(3)) - std::stof(SteamTable->at(i).at(2)));
				float SpecificVolume = std::stof(SteamTable->at(i).at(13)) + quality * (std::stof(SteamTable->at(i).at(14)) - std::stof(SteamTable->at(i).at(13)));
				return std::make_tuple(quality, std::stof(SteamTable->at(i).at(1)), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);

			}
		}
	}
	if constexpr (std::is_same<CalculateFlag, SteamTableCalculateFlags::ENTROPY>())
	{
		for (int i = 1; i < SteamTable->size(); i++)
		{
			if ((std::stof(SteamTable->at(i).at(0)) == *temperature))
			{
				float lower_value = std::stof(SteamTable->at(i).at(10)); float upper_value = std::stof(SteamTable->at(i).at(11));
				float quality = (*target - lower_value) / (upper_value - lower_value);
				float enthalpy = std::stof(SteamTable->at(i).at(7)) + quality * (std::stof(SteamTable->at(i).at(8)) - std::stof(SteamTable->at(i).at(7)));
				float InternalEnergy = std::stof(SteamTable->at(i).at(4)) + quality * (std::stof(SteamTable->at(i).at(5)) - std::stof(SteamTable->at(i).at(4)));
				float Entropy = *target;
				float Density = std::stof(SteamTable->at(i).at(2)) + quality * (std::stof(SteamTable->at(i).at(3)) - std::stof(SteamTable->at(i).at(2)));
				float SpecificVolume = std::stof(SteamTable->at(i).at(13)) + quality * (std::stof(SteamTable->at(i).at(14)) - std::stof(SteamTable->at(i).at(13)));
				return std::make_tuple(quality, std::stof(SteamTable->at(i).at(1)), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);

			}
		}
	}
	if constexpr (std::is_same < CalculateFlag, SteamTableCalculateFlags::DENSITY>())
	{
		for (int i = 1; i < SteamTable->size(); i++)
		{
			if ((std::stof(SteamTable->at(i).at(0)) == *temperature))
			{
				float lower_value = std::stof(SteamTable->at(i).at(2)); float upper_value = std::stof(SteamTable->at(i).at(3));
				float quality = (*target - lower_value) / (upper_value - lower_value);
				float enthalpy = std::stof(SteamTable->at(i).at(7)) + quality * (std::stof(SteamTable->at(i).at(8)) - std::stof(SteamTable->at(i).at(7)));
				float InternalEnergy = std::stof(SteamTable->at(i).at(4)) + quality * (std::stof(SteamTable->at(i).at(5)) - std::stof(SteamTable->at(i).at(4)));
				float Entropy = std::stof(SteamTable->at(i).at(10)) + quality * (std::stof(SteamTable->at(i).at(11)) - std::stof(SteamTable->at(i).at(10)));
				float Density = *target;
				float SpecificVolume = std::stof(SteamTable->at(i).at(13)) + quality * (std::stof(SteamTable->at(i).at(14)) - std::stof(SteamTable->at(i).at(13)));
				return std::make_tuple(quality, std::stof(SteamTable->at(i).at(1)), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);

			}
		}
	}
	if constexpr (std::is_same < CalculateFlag, SteamTableCalculateFlags::SPECIFIC_VOLUME>())
	{
		for (int i = 1; i < SteamTable->size(); i++)
		{
			if ((std::stof(SteamTable->at(i).at(0)) == *temperature))
			{
				float lower_value = std::stof(SteamTable->at(i).at(13)); float upper_value = std::stof(SteamTable->at(i).at(14));
				float quality = (*target - lower_value) / (upper_value - lower_value);
				float enthalpy = std::stof(SteamTable->at(i).at(7)) + quality * (std::stof(SteamTable->at(i).at(8)) - std::stof(SteamTable->at(i).at(7)));
				float InternalEnergy = std::stof(SteamTable->at(i).at(4)) + quality * (std::stof(SteamTable->at(i).at(5)) - std::stof(SteamTable->at(i).at(4)));
				float Entropy = std::stof(SteamTable->at(i).at(10)) + quality * (std::stof(SteamTable->at(i).at(11)) - std::stof(SteamTable->at(i).at(10)));
				float Density = std::stof(SteamTable->at(i).at(2)) + quality * (std::stof(SteamTable->at(i).at(3)) - std::stof(SteamTable->at(i).at(2)));
				float SpecificVolume = *target;
				return std::make_tuple(quality, std::stof(SteamTable->at(i).at(1)), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);

			}
		}
	}

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

	const char* names[] = { "Enthalpy", "Internal Energy", "Entropy", "Density", "Specific Volume" };
	


	if (ImGui::CollapsingHeader("Saturated Steam Table (Temperature)"))
	{
		static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 15>>>();
		static bool once = []() { LoadSteamTable(*SteamTable, "steam_tables/Saturated Steam Table (Temperature).csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();
		


		//CalculateQuality<SteamTableCalculateFlags::ENTHALPY>(SteamTable);

		if (ImGui::TreeNode("Quality Calculator"))
		{
			static int temp_pressure[1] = { 20};
			static int selected_item = -1;
			static float target = 0;
			static std::tuple<float, float, float, float, float, float, float> result;
			static std::string selected = "Input Type";
			ImGui::InputInt("Temperature", &temp_pressure[0]);

			if (selected_item != -1)
			{
				switch (selected_item)
				{
				case 0:
					selected = "Enthalpy";
					break;
				case 1:
					selected = "Internal Energy";
					break;
				case 2:
					selected = "Entropy";
					break;
				case 3:
					selected = "Density";
					break;
				case 4:
					selected = "Specific Volume";
					break;
				}
			}

			if (selected_item != -1)
			{
				std::string format = "%0.1f";
				std::string placeholder;
				if ((selected_item == 0) or (selected_item == 1)) placeholder = " kj"; if (selected_item == 2) placeholder = " kj/kg*k"; if (selected_item == 3) placeholder = " kg/m^3"; if (selected_item == 4) placeholder = " m^3/kg";

				ImGui::InputFloat("", &target, 0.0f, 0.0f, (format + placeholder).c_str());
				ImGui::SameLine();

			}

			if (ImGui::Button(selected.c_str()))
				ImGui::OpenPopup("Inputs");
			//ImGui::SameLine();

			//ImGui::TextUnformatted(selected_item == -1 ? "Not Selected" : names[selected_item]);
			if (ImGui::BeginPopup("Inputs"))
			{
				ImGui::Text("Inputs");
				ImGui::Separator();
				for (int i = 0; i < IM_ARRAYSIZE(names); i++)
					if (ImGui::Selectable(names[i]))
						selected_item = i;
				ImGui::EndPopup();
			}
			if (selected_item != -1)
			{
				ImGui::SameLine();
				if (ImGui::Button("Calculate"))
				{
					ImGui::OpenPopup("Result");
					if(selected == "Enthalpy")
						result = CalculateQuality<SteamTableCalculateFlags::ENTHALPY>(SteamTable, &temp_pressure[0], &target);
					if(selected == "Internal Energy")
						result = CalculateQuality<SteamTableCalculateFlags::INTERNAL_ENERGY>(SteamTable, &temp_pressure[0], &target);
					if(selected == "Entropy")
						result = CalculateQuality<SteamTableCalculateFlags::ENTROPY>(SteamTable, &temp_pressure[0], &target);
					if(selected == "Density")
						result = CalculateQuality<SteamTableCalculateFlags::DENSITY>(SteamTable, &temp_pressure[0], &target);
					if(selected == "Specific Volume")
						result = CalculateQuality<SteamTableCalculateFlags::SPECIFIC_VOLUME>(SteamTable, &temp_pressure[0], &target);
				}
				if (ImGui::BeginPopup("Result"))
				{
					if (std::get<0>(result) > 1.0)
					{
						ImGui::TextColored(ImColor(59, 254, 225), "Phase: Vapor ");
						ImGui::Text(("Quality: NOT SATURATED"));
					}
					else if (std::get<0>(result) < 0.0)
					{
						ImGui::TextColored(ImColor(59, 154, 225), "Phase: Liquid ");
						ImGui::Text(("Quality: NOT SATURATED"));
					}
					else
					{
						ImGui::TextColored(ImColor(159, 254, 225), "Phase: Saturated ");
						ImGui::TextColored(ImColor(159, 254, 225), ("Quality: " + _Format(std::get<0>(result), 3)).c_str());
						ImGui::Separator();
						ImGui::Text(("Saturated Pressure: " + _Format(std::get<1>(result), 3) + " MPa").c_str());
						ImGui::Text(("Enthalpy: " + _Format(std::get<2>(result), 4) + " kj/kg").c_str());
						ImGui::Text(("Internal Energy: " + _Format(std::get<3>(result), 4) + " kj/kg").c_str());
						ImGui::Text(("Entropy: " + _Format(std::get<4>(result), 3) + " kj/kg*k").c_str());
						ImGui::Text(("Density: " + _Format(std::get<5>(result), 4) + " kg/m^3").c_str());
						ImGui::Text(("Specific Volume: " + _Format(std::get<6>(result), 4) + " m^3/kg").c_str());

					}
					ImGui::EndPopup();
				}

			}

			ImGui::TreePop();

		}

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