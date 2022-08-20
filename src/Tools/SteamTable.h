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

template<size_t ArraySize>
auto load_steam_table(std::vector<std::array<std::string, ArraySize>>& steam_table, std::string file_name) -> void
{
	std::string word;
	std::fstream file;
	file.open(file_name, std::ios::in);
	if (file.is_open())
	{
		std::string line;
		int iterate_vec = 0;
		while (std::getline(file, line))
		{
			std::stringstream str(line);
			int iterate_arr = 0;
			steam_table.emplace_back();
			while (std::getline(str, word, ','))
			{
				steam_table.at(iterate_vec).at(iterate_arr) = word;
				iterate_arr++;
			}
			iterate_vec++;
		}
	}
	file.close();
}


template<size_t Size>
void PressureList(std::shared_ptr<boiler<FEED::DOUBLE>>& boil, std::unique_ptr<std::vector<std::array<std::string, Size>>>& Table)
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

		for (auto& i : pressure_list)
		{
			const bool is_selected = (current_pressure1 == i);
			if (ImGui::Selectable(i.c_str(), is_selected))
			{
				current_pressure1 = i + " MPa";
				boil->ReturnPressure1() = std::stof(i);
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
		for (auto& i : pressure_list)
		{
			const bool is_selected = (current_pressure2 == i);
			if (ImGui::Selectable(i.c_str(), is_selected))
			{
				current_pressure2 = i + " MPa";
				boil->ReturnPressure2() = std::stof(i);
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
		for (auto& i : pressure_list)
		{
			const bool is_selected = (current_pressure3 == i);
			if (ImGui::Selectable(i.c_str(), is_selected))
			{
				current_pressure3 = i + " MPa";
				boil->ReturnPressure3() = std::stof(i);
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

namespace SaturatedTable
{
	struct TEMPERATURE;
	struct PRESSURE;
}
template<size_t size>
inline auto InterpolaSteamValues(std::unique_ptr<std::vector<std::array<std::string, size>>>& SteamTable, float temperature, int i)
{
	float Den_Liq1 = std::stof(SteamTable->at(i).at(2)), Den_Vap1 = std::stof(SteamTable->at(i).at(3)), Internal_Liquid1 = std::stof(SteamTable->at(i).at(4)),
		Internal_Vapor1 = std::stof(SteamTable->at(i).at(5)), Enthalpy_Liquid1 = std::stof(SteamTable->at(i).at(7)), Enthalpy_Vap1 = std::stof(SteamTable->at(i).at(8)),
		Entropy_Liquid1 = std::stof(SteamTable->at(i).at(10)), Entropy_Vapor1 = std::stof(SteamTable->at(i).at(11)), Specific_Liquid1 = std::stof(SteamTable->at(i).at(13)), Specific_Vap1 = std::stof(SteamTable->at(i).at(14));

	float Den_Liq2 = std::stof(SteamTable->at(i + 1).at(2)), Den_Vap2 = std::stof(SteamTable->at(i + 1).at(3)), Internal_Liquid2 = std::stof(SteamTable->at(i + 1).at(4)),
		Internal_Vapor2 = std::stof(SteamTable->at(i + 1).at(5)), Enthalpy_Liquid2 = std::stof(SteamTable->at(i + 1).at(7)), Enthalpy_Vap2 = std::stof(SteamTable->at(i + 1).at(8)),
		Entropy_Liquid2 = std::stof(SteamTable->at(i + 1).at(10)), Entropy_Vapor2 = std::stof(SteamTable->at(i + 1).at(11)), Specific_Liquid2 = std::stof(SteamTable->at(i + 1).at(13)), Specific_Vap2 = std::stof(SteamTable->at(i + 1).at(14));

	//Percentage change
	
	float change = (temperature - std::stof(SteamTable->at(i).at(0))) / (std::stof(SteamTable->at(i + 1).at(0)) - std::stof(SteamTable->at(i).at(0)));
	float Den_Liquid_Real = Den_Liq1 + change * (Den_Liq2 - Den_Liq1);
	float Den_Vapor_Real = Den_Vap1 + change * (Den_Vap2 - Den_Vap1);
	float Internal_Liquid_Real = Internal_Liquid1 + change * (Internal_Liquid2 - Internal_Liquid1);
	float Internal_Vapor_Real = Internal_Vapor1 + change * (Internal_Vapor2 - Internal_Vapor1);
	float Enthalpy_Liquid_Real = Enthalpy_Liquid1 + change * (Enthalpy_Liquid2 - Enthalpy_Liquid1);
	float Enthalpy_Vapor_Real = Enthalpy_Vap1 + change * (Enthalpy_Vap2 - Enthalpy_Vap1);
	float Entropy_Liquid_Real = Entropy_Liquid1 + change * (Entropy_Liquid2 - Entropy_Liquid1);
	float Entropy_Vapor_Real = Entropy_Vapor1 + change * (Entropy_Vapor2 - Entropy_Vapor1);
	float Specific_Volume_Liquid_Real = Specific_Liquid1 + change * (Specific_Liquid2 - Specific_Liquid1);
	float Specific_Volume_Vapor_Real = Specific_Vap1 + change * (Specific_Vap2 - Specific_Vap1);

	float Real_Pressure = std::stof(SteamTable->at(i).at(1)) + change * (std::stof(SteamTable->at(i + 1).at(1)) - std::stof(SteamTable->at(i).at(1)));
	//std::cout << "Entropy Liquid: " << Entropy_Liquid1 << " Entropy Liquid2: " << Entropy_Liquid2 << std::endl;
	//std::cout << "Entropy Vap: " << Entropy_Vapor1 << " Entropy Vap2: " << Entropy_Vapor2 << std::endl;

	//std::cout << "Real Entropy Liquid: " << Entropy_Liquid_Real << " Real Entropy Vapor: " << Entropy_Vapor_Real << std::endl;
	return std::make_tuple(Den_Liquid_Real, Den_Vapor_Real, Internal_Liquid_Real, Internal_Vapor_Real, Enthalpy_Liquid_Real, Enthalpy_Vapor_Real, Entropy_Liquid_Real, Entropy_Vapor_Real,
		Specific_Volume_Liquid_Real, Specific_Volume_Vapor_Real, Real_Pressure);

}




template<typename CalculateFlag, size_t Size>

//std::tuple<quality, pressure, Enthalpy, Internal Energy, Entropy, Density, Specific Volume>
std::tuple<float, float, float, float, float, float, float> CalculateQuality(std::unique_ptr<std::vector<std::array<std::string, Size>>>& SteamTable, float* temperature, float* target)
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
			else if ((*temperature > std::stof(SteamTable->at(i).at(0)) && (*temperature < std::stof(SteamTable->at(i + 1).at(0)))))
			{
				//interpolation before finding the quality
				float lower_value_temp = std::stof(SteamTable->at(i).at(0)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(0));

				//std::tupe<Den_liq, Den_vap, Internal_liq, Internal_vap, Enthalpy_liquid, Enthalpy_vapor, Entropy_liq, Entropy_vap, specific_liquid, specific_vap
				auto tuple_result = InterpolaSteamValues(SteamTable, *temperature, i);
				float quality = (*target - std::get<4>(tuple_result)) / (std::get<5>(tuple_result) - std::get<4>(tuple_result));
				float enthalpy = *target;
				float InternalEnergy = std::get<2>(tuple_result) + quality * (std::get<3>(tuple_result) - std::get<2>(tuple_result));
				float Entropy = std::get<6>(tuple_result) + quality * (std::get<7>(tuple_result) - std::get<6>(tuple_result));
				float Density = std::get<0>(tuple_result) + quality * (std::get<1>(tuple_result) - std::get<0>(tuple_result));
				float SpecificVolume = std::get<8>(tuple_result) + quality * (std::get<9>(tuple_result) - std::get<8>(tuple_result));


				//std::cout << "Enthalpy Liquid: " << std::get<4>(tuple_result) << " Enthalpy Vapor: " << std::get<5>(tuple_result) << std::endl;
				//std::cout << "Quality: " << quality << std::endl;



				return std::make_tuple(quality, std::get<10>(tuple_result), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);
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
			else if ((*temperature > std::stof(SteamTable->at(i).at(0)) && (*temperature < std::stof(SteamTable->at(i + 1).at(0)))))
			{
				//interpolation before finding the quality
				float lower_value_temp = std::stof(SteamTable->at(i).at(0)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(0));

				//std::tupe<Den_liq, Den_vap, Internal_liq, Internal_vap, Enthalpy_liquid, Enthalpy_vapor, Entropy_liq, Entropy_vap, specific_liquid, specific_vap
				auto tuple_result = InterpolaSteamValues(SteamTable, *temperature, i);
				float quality = (*target - std::get<2>(tuple_result)) / (std::get<3>(tuple_result) - std::get<2>(tuple_result));
				float enthalpy = std::get<4>(tuple_result) + quality * (std::get<5>(tuple_result) - std::get<4>(tuple_result));
				float InternalEnergy = *target;
				float Entropy = std::get<6>(tuple_result) + quality * (std::get<7>(tuple_result) - std::get<6>(tuple_result));
				float Density = std::get<0>(tuple_result) + quality * (std::get<1>(tuple_result) - std::get<0>(tuple_result));
				float SpecificVolume = std::get<8>(tuple_result) + quality * (std::get<9>(tuple_result) - std::get<8>(tuple_result));

				return std::make_tuple(quality, std::get<10>(tuple_result), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);
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
			else if ((*temperature > std::stof(SteamTable->at(i).at(0)) && (*temperature < std::stof(SteamTable->at(i + 1).at(0)))))
			{
				//interpolation before finding the quality
				float lower_value_temp = std::stof(SteamTable->at(i).at(0)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(0));

				//std::tupe<Den_liq, Den_vap, Internal_liq, Internal_vap, Enthalpy_liquid, Enthalpy_vapor, Entropy_liq, Entropy_vap, specific_liquid, specific_vap
				auto tuple_result = InterpolaSteamValues(SteamTable, *temperature, i);
				float quality = (*target - std::get<6>(tuple_result)) / (std::get<7>(tuple_result) - std::get<6>(tuple_result));
				float enthalpy = std::get<4>(tuple_result) + quality * (std::get<5>(tuple_result) - std::get<4>(tuple_result));
				float InternalEnergy = std::get<2>(tuple_result) + quality * (std::get<3>(tuple_result) - std::get<2>(tuple_result));
				float Entropy = *target;
				float Density = std::get<0>(tuple_result) + quality * (std::get<1>(tuple_result) - std::get<0>(tuple_result));
				float SpecificVolume = std::get<8>(tuple_result) + quality * (std::get<9>(tuple_result) - std::get<8>(tuple_result));

				return std::make_tuple(quality, std::get<10>(tuple_result), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);
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
			else if ((*temperature > std::stof(SteamTable->at(i).at(0)) && (*temperature < std::stof(SteamTable->at(i + 1).at(0)))))
			{
				//interpolation before finding the quality
				float lower_value_temp = std::stof(SteamTable->at(i).at(0)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(0));

				//std::tupe<Den_liq, Den_vap, Internal_liq, Internal_vap, Enthalpy_liquid, Enthalpy_vapor, Entropy_liq, Entropy_vap, specific_liquid, specific_vap
				auto tuple_result = InterpolaSteamValues(SteamTable, *temperature, i);
				float quality = (*target - std::get<0>(tuple_result)) / (std::get<1>(tuple_result) - std::get<0>(tuple_result));
				float enthalpy = std::get<4>(tuple_result) + quality * (std::get<5>(tuple_result) - std::get<4>(tuple_result));
				float InternalEnergy = std::get<2>(tuple_result) + quality * (std::get<3>(tuple_result) - std::get<2>(tuple_result));
				float Entropy = std::get<6>(tuple_result) + quality * (std::get<7>(tuple_result) - std::get<6>(tuple_result));
				float Density = *target;;
				float SpecificVolume = std::get<8>(tuple_result) + quality * (std::get<9>(tuple_result) - std::get<8>(tuple_result));

				return std::make_tuple(quality, std::get<10>(tuple_result), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);
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
			else if ((*temperature > std::stof(SteamTable->at(i).at(0)) && (*temperature < std::stof(SteamTable->at(i + 1).at(0)))))
			{
				//interpolation before finding the quality
				float lower_value_temp = std::stof(SteamTable->at(i).at(0)); float upper_value_temp = std::stof(SteamTable->at(i + 1).at(0));

				//std::tupe<Den_liq, Den_vap, Internal_liq, Internal_vap, Enthalpy_liquid, Enthalpy_vapor, Entropy_liq, Entropy_vap, specific_liquid, specific_vap
				auto tuple_result = InterpolaSteamValues(SteamTable, *temperature, i);
				float quality = (*target - std::get<8>(tuple_result)) / (std::get<9>(tuple_result) - std::get<8>(tuple_result));
				float enthalpy = std::get<4>(tuple_result) + quality * (std::get<5>(tuple_result) - std::get<4>(tuple_result));
				float InternalEnergy = std::get<2>(tuple_result) + quality * (std::get<3>(tuple_result) - std::get<2>(tuple_result));
				float Entropy = std::get<6>(tuple_result) + quality * (std::get<7>(tuple_result) - std::get<6>(tuple_result));
				float Density = std::get<0>(tuple_result) + quality * (std::get<1>(tuple_result) - std::get<0>(tuple_result));
				float SpecificVolume = *target;

				return std::make_tuple(quality, std::get<10>(tuple_result), enthalpy, InternalEnergy, Entropy, Density, SpecificVolume);
			}
		}
	}

}


template<typename T, size_t Size>
inline void Quality_Calculator(std::unique_ptr<std::vector<std::array<std::string, Size>>> &SteamTable)
{
	const char* names[] = { "Enthalpy", "Internal Energy", "Entropy", "Density", "Specific Volume" };

	if (ImGui::TreeNode("Quality Calculator"))
	{
		static int selected_item = -1;
		static float target = 0;
		static std::tuple<float, float, float, float, float, float, float> result;
		static std::string selected = "Input Type";
																															//Value for temperature
		//Lambda function that will return the value for temperature and pressure at compile time
		auto RetrieveConstExprValue = []() constexpr -> float {if constexpr (std::is_same<T, SaturatedTable::TEMPERATURE>()) return 20;
		else
		{
			//Value for pressure
			return 1.0;
		}
		};

		static float temp_pressure[1] = { RetrieveConstExprValue()};

		//Compile time show if we passed either SaturatedTable::TEMPERATURE or SaturatedTable::Pressure template arguments to dictate which input we should put
		if constexpr (std::is_same<T, SaturatedTable::TEMPERATURE>())
		{

			ImGui::InputFloat("Temperature", &temp_pressure[0], 0.0f, 0.0f, "%.1f");
		}
		if constexpr(std::is_same<T, SaturatedTable::PRESSURE>())
		{

			ImGui::InputFloat("Pressure", &temp_pressure[0], 0.0f, 0.0f, "%.4f");
		}


		//select which text should be shown in button label when selecting operation
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
			const std::string format = "%0.1f";
			std::string placeholder;
			//Depending on which item, it will show the appropriate unit that will be used in the input value box
			if ((selected_item == 0) or (selected_item == 1)) placeholder = " kj"; if (selected_item == 2) placeholder = " kj/kg*k"; if (selected_item == 3) placeholder = " kg/m^3"; if (selected_item == 4) placeholder = " m^3/kg";

			ImGui::InputFloat("", &target, 0.0f, 0.0f, (format + placeholder).c_str());
			ImGui::SameLine();

		}

		if (ImGui::Button(selected.c_str()))
			ImGui::OpenPopup("Inputs");

		//Open the popup menu after pressing "Input Type"
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
				//Depending on which type is chosen, template argument will be passed to the function to call the appropriate code
				ImGui::OpenPopup("Result");
				if (selected == "Enthalpy")
					result = CalculateQuality<SteamTableCalculateFlags::ENTHALPY>(SteamTable, &temp_pressure[0], &target);
				if (selected == "Internal Energy")
					result = CalculateQuality<SteamTableCalculateFlags::INTERNAL_ENERGY>(SteamTable, &temp_pressure[0], &target);
				if (selected == "Entropy")
					result = CalculateQuality<SteamTableCalculateFlags::ENTROPY>(SteamTable, &temp_pressure[0], &target);
				if (selected == "Density")
					result = CalculateQuality<SteamTableCalculateFlags::DENSITY>(SteamTable, &temp_pressure[0], &target);
				if (selected == "Specific Volume")
					result = CalculateQuality<SteamTableCalculateFlags::SPECIFIC_VOLUME>(SteamTable, &temp_pressure[0], &target);
			}
			//Show the result we got from std::tuple
			if (ImGui::BeginPopup("Result"))
			{
				if (std::get<0>(result) > 1.0)
				{
					ImGui::TextColored(ImColor(59, 254, 225), "Phase: Vapor ");
					ImGui::TextColored(ImColor(59, 254, 225), ("Quality: " + _Format(std::get<0>(result), 3)).c_str());
					ImGui::Text(("Quality: NOT SATURATED"));
				}
				else if (std::get<0>(result) < 0.0)
				{
					ImGui::TextColored(ImColor(59, 154, 225), "Phase: Liquid ");
					ImGui::TextColored(ImColor(59, 154, 225), ("Quality: " + _Format(std::get<0>(result), 3)).c_str());
					ImGui::Text(("Quality: NOT SATURATED"));
				}
				else
				{
					ImGui::TextColored(ImColor(159, 254, 225), "Phase: Saturated ");
					ImGui::TextColored(ImColor(159, 254, 225), ("Quality: " + _Format(std::get<0>(result), 3)).c_str());
					if constexpr(std::is_same<T, SaturatedTable::PRESSURE>())
					{
						ImGui::TextColored(ImColor(159, 254, 225), ("Temperature: " + _Format(std::get<1>(result), 4) + " C").c_str());
						ImGui::TextColored(ImColor(159, 254, 225), ("Saturated Pressure: " + _Format(temp_pressure[0], 4) + " MPa").c_str());
					}
					else
					{
						ImGui::TextColored(ImColor(159, 254, 225), ("Temperature: " + _Format(temp_pressure[0], 4) + " C").c_str());
						ImGui::TextColored(ImColor(159, 254, 225), ("Saturated Pressure: " + _Format(std::get<1>(result), 4) + " MPa").c_str());
					}
					ImGui::Separator();
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
}



inline void SteamTableSimulation()
{
	enum contents_type { CT_Text, CT_FillButton };
	static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	static int contents_type = CT_Text;

	//Tableflags that will be used for the table
	flags |= ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
		| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
		| ImGuiTableFlags_SizingFixedFit;

	static bool show_window1 = false;
	static bool show_window2 = false;
	static bool show_window3 = false;
	ImGui::TextColored(ImColor(100, 100, 100, 250), "Press F1 when using a steam table to seperate it into a standlone window");
	if (show_window1 == true)
	{
		ImGui::Begin("Saturated Steam (Temperature)", &show_window1);
		static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 15>>>();
		static bool once = []() { load_steam_table(*SteamTable, "steam_tables/Saturated Steam Table (Temperature).csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();



		//CalculateQuality<SteamTableCalculateFlags::ENTHALPY>(SteamTable);
		Quality_Calculator<SaturatedTable::TEMPERATURE>(SteamTable);

		if (ImGui::BeginTable("table1", 15, flags))
		{
			//Freeze the first row
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
		ImGui::End();
	}

	if(show_window2 == true)
	{
		ImGui::Begin("Saturated Steam (Pressure)", &show_window2);
		static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 15>>>();
		static bool once = []() { load_steam_table(*SteamTable, "steam_tables/Saturated Steam Table (Pressure).csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();
		Quality_Calculator<SaturatedTable::PRESSURE>(SteamTable);

		if (ImGui::BeginTable("table2", 15, flags))
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
		ImGui::End();
	}

	if(show_window3 == true)
	{
		ImGui::Begin("Compressed and Superheated Table", &show_window3);
		static std::unique_ptr<std::vector<std::array<std::string, 8>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 8>>>();
		static bool once = []() { load_steam_table(*SteamTable, "steam_tables/Compressed Liquid and Superheated Steam Table.csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();
		struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array

		if (ImGui::BeginTable("table3", 8, flags))
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
			for (int row = 0; row < 100; row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 8; column++)
				{
					ImGui::TableSetColumnIndex(column);
					if ((value2 == row) && (value2 != 0))
					{

						ImGui::TextColored(ImVec4(0.3, 0.9, 0.8, 0.9), SteamTable->at(row).at(column).c_str());
					}
					else if (ImGui::Selectable(SteamTable->at(row).at(column).c_str(), placeholder.at(row), ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
					{
						std::cout << "value: " << value2 << std::endl;
						placeholder.at(row) = true;
						value2 = row;
					}

				}

			}
			ImGui::EndTable();
		}
		ImGui::End();
	}


	if ((show_window1 != true) && ImGui::CollapsingHeader("Saturated Steam Table (Temperature)"))
	{
		static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 15>>>();
		static bool once = []() { load_steam_table(*SteamTable, "steam_tables/Saturated Steam Table (Temperature).csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();

		struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
		for (ImGuiKey key = 0; key < ImGuiKey_COUNT; key++) {
			if (funcs::IsLegacyNativeDupe(key)) continue; if (ImGui::IsKeyDown(key))
			{
				if (ImGui::GetKeyName((key)) == "F1")
				{
					show_window1 = true;

				}
			}
		}

		//CalculateQuality<SteamTableCalculateFlags::ENTHALPY>(SteamTable);
		Quality_Calculator<SaturatedTable::TEMPERATURE>(SteamTable);

		if (ImGui::BeginTable("table1", 15, flags))
		{
			//Freeze the first row
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


	if ((show_window2 != true) && ImGui::CollapsingHeader("Saturated Steam Table (Pressure)"))
	{
		static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 15>>>();
		static bool once = []() { load_steam_table(*SteamTable, "steam_tables/Saturated Steam Table (Pressure).csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();


		struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
		for (ImGuiKey key = 0; key < ImGuiKey_COUNT; key++) {
			if (funcs::IsLegacyNativeDupe(key)) continue; if (ImGui::IsKeyDown(key))
			{
				if (ImGui::GetKeyName((key)) == "F1")
				{
					show_window2 = true;

				}
			}
		}

		Quality_Calculator<SaturatedTable::PRESSURE>(SteamTable);

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

	if ((show_window3 != true) && ImGui::CollapsingHeader("Compressed Liquid and Superheated Steam Table"))
	{
		static std::unique_ptr<std::vector<std::array<std::string, 8>>> SteamTable = std::make_unique<std::vector<std::array<std::string, 8>>>();
		static bool once = []() { load_steam_table(*SteamTable, "steam_tables/Compressed Liquid and Superheated Steam Table.csv"); std::cout << "Done Loading Table" << std::endl;  return true; } ();

		struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
		for (ImGuiKey key = 0; key < ImGuiKey_COUNT; key++) {
			if (funcs::IsLegacyNativeDupe(key)) continue; if (ImGui::IsKeyDown(key))
			{
				if (ImGui::GetKeyName((key)) == "F1")
				{
					show_window3 = true;

				}
			}
		}

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
			for (int row = 0; row < 100; row++)
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
						std::cout << "value: " << value2 << std::endl;
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
float CalculateFromSteamTable(std::unique_ptr<std::vector<std::array<std::string, size>>>& SteamTable, SteamTableFlag table, SteamTableCalculate calculate, float& pressure, float& temperature, float& enthalpy_target, Phase& phase)
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
						else if ((temperature == std::stof(SteamTable->at(i).at(1))) && ((phase.phase == " Saturated Liquid") or (phase.phase == " Saturated Vapor")))
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