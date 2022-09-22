#include "VaporPressureCalculator.h"





void VaporPressureCalculator::ShowVaporCalculator(bool *p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Vapor Pressure Calculator", p_open))
	{
		return;
	}
	static VaporPressureCalculator application;
	static const char* current_item = "Choose";

	static float min_temp = 0;
	static float max_temp = 0;

	//user input
	static float temperature = 285;
	static float pressure = 0;
	static float specie_number = 0;

	//our antoine constants
	static float Antoine_A = 0, Antoine_B = 0, Antoine_C = 0;

	static float VaporPressure = 0;
	static float VaporTemperature = 0; //will be used to calculate the vapor temperature
	static bool ShowResult = false;

	static int selection = 1;
	ImGui::RadioButton("Pressure Calculator", &selection, 1); ImGui::SameLine();
	ImGui::RadioButton("Temperature Calculator", &selection, 2);
	if(selection == 1)
	{
		static std::string final_result = "";
		ImGui::TextColored(ImColor(120, 240, 80, 220), "Compounds");
		if(ImGui::BeginCombo("Specie", current_item, ImGuiComboFlags_HeightLarge)) // The second parameter is the label previewed before opening the combo.
	{
		for(unsigned n = 0; n < application.AntoineConstants->size(); n++)
		{
			//Checks if the item is selected or not
			bool is_selected = (current_item == application.AntoineConstants->at(n).at(0).c_str()); // You can store your selection however you want, outside or inside your objects
			if(ImGui::Selectable(application.AntoineConstants->at(n).at(0).c_str(), is_selected))
			{
				//After selecting the item, it will store all of its data into these variables that will later on be used to display it to the user
				current_item = application.AntoineConstants->at(n).at(0).c_str();
				min_temp = 273.15 + std::stof(application.AntoineConstants->at(n).at(4));
				max_temp = 273.15 + std::stof(application.AntoineConstants->at(n).at(5));
				Antoine_A = std::stof(application.AntoineConstants->at(n).at(1));
				Antoine_B = std::stof(application.AntoineConstants->at(n).at(2));
				Antoine_C = std::stof(application.AntoineConstants->at(n).at(3));
				specie_number = n;
				return;
			}
			if(is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::InputFloat("Input Temperature", &temperature, 1.0f, 5.0f, "%.2f K", ImGuiSliderFlags_AlwaysClamp); temperature = (temperature > max_temp) ? max_temp : temperature; temperature = (temperature < min_temp) ? min_temp : temperature;
	ImGui::SliderFloat("Temperature", &temperature, min_temp, max_temp, "%.2f K", ImGuiSliderFlags_AlwaysClamp);
	ImGui::TextColored(ImColor(220, 110, 90, 230), ("Temperature Range: " + _Format(min_temp, 4) + " K to " + _Format(max_temp, 4) + " K").c_str());
		if(ImGui::Button("Calculate"))
	{
		VaporPressure = application.CalculateVaporPressure<VaporPressureCalculator::VaporPressureCalculate>(temperature, specie_number, Antoine_A, Antoine_B, Antoine_C);
		final_result = "Vapor Pressure: " + _Format(VaporPressure, 6) + " mmHg\nVapor Pressure: " + _Format(VaporPressure/760.0f, 6) + " atm\nVapor Pressure: " + _Format(VaporPressure/7.50062f, 6) + " kPa\nA = " + _Format(Antoine_A, 6) + "\nB = " + _Format(Antoine_B, 6) + "\nC = " + _Format(Antoine_C, 6);
		ShowResult = true;
	}
		if(ShowResult)
	ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)), final_result.c_str());
	}

	if(selection == 2)
	{
		static std::string final_result = "";
		ImGui::TextColored(ImColor(120, 240, 80, 220), "Compounds");
		if(ImGui::BeginCombo("Specie", current_item, ImGuiComboFlags_HeightLarge)) // The second parameter is the label previewed before opening the combo.
		{
		for(unsigned n = 0; n < application.AntoineConstants->size(); n++)
		{
			//Checks if the item is selected or not
			bool is_selected = (current_item == application.AntoineConstants->at(n).at(0).c_str()); // You can store your selection however you want, outside or inside your objects
			if(ImGui::Selectable(application.AntoineConstants->at(n).at(0).c_str(), is_selected))
			{
				//After selecting the item, it will store all of its data into these variables that will later on be used to display it to the user
				current_item = application.AntoineConstants->at(n).at(0).c_str();
				min_temp = 273.15 + std::stof(application.AntoineConstants->at(n).at(4));
				max_temp = 273.15 + std::stof(application.AntoineConstants->at(n).at(5));
				Antoine_A = std::stof(application.AntoineConstants->at(n).at(1));
				Antoine_B = std::stof(application.AntoineConstants->at(n).at(2));
				Antoine_C = std::stof(application.AntoineConstants->at(n).at(3));
				specie_number = n;
				return;
			}
			if(is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
		}
		ImGui::InputFloat("Input Pressure", &pressure, 1.0f, 5.0f, "%.2f mmHg", ImGuiSliderFlags_AlwaysClamp); pressure = (pressure > 800) ? 800 : pressure; pressure = (pressure < 10) ? 10 : pressure;
		ImGui::SliderFloat("Pressure", &pressure, 10, 800, "%.2f mmHg", ImGuiSliderFlags_AlwaysClamp);
		ImGui::TextColored(ImColor(220, 110, 90, 230), ("Vapor Pressure Range: " + _Format(10.0f, 4) + " mmHg to " + _Format(800.0f, 4) + " mmHg").c_str());
		if(ImGui::Button("Calculate"))
		{
			VaporTemperature = application.CalculateVaporPressure<VaporPressureCalculator::VaporTemperatureCalculate>(pressure, specie_number, Antoine_A, Antoine_B, Antoine_C) + 273.15;
			final_result = "Vapor Temperature: " + _Format(VaporTemperature, 6) + " K\nA = " + _Format(Antoine_A, 6) + "\nB = " + _Format(Antoine_B, 6) + "\nC = " + _Format(Antoine_C, 6);
			ShowResult = true;
		}
		if(ShowResult)
			ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)), final_result.c_str());
	}

	return;
}

template<typename T>
float VaporPressureCalculator::CalculateVaporPressure(float value, unsigned specie_number, float Antoine_A, float Antoine_B, float Antoine_C)
{
	if constexpr (std::is_same<T, VaporPressureCalculate>())
	{
		std::cout << Antoine_A << std::endl << Antoine_B << std::endl << Antoine_C << std::endl;
		return (pow(10, Antoine_A - Antoine_B/( (value - 273.15) + Antoine_C)));
	}
	if constexpr (std::is_same<T, VaporTemperatureCalculate>())
	{
		return Antoine_B/(Antoine_A - log10f(value)) - Antoine_C;
	}
}

VaporPressureCalculator::VaporPressureCalculator()
{
	filePath = "Empirical Tables/Antoine Constants/Antoine Equation Constants.csv";
	std::fstream file(filePath, std::ios::in);
	AntoineConstants = std::make_unique<std::vector<std::array<std::string, 6>>>();
	std::string word;
	if(file.is_open())
	{
		std::cout << "Successfully opened" << std::endl;
		std::string line;
		unsigned iterate_vec = 0;
		while(std::getline(file, line))
		{
			std::stringstream str(line);
			unsigned iterate_arr = 0;
			AntoineConstants->emplace_back();
			while(std::getline(str, word, ','))
			{
				AntoineConstants->at(iterate_vec).at(iterate_arr) = word;
				iterate_arr++;
			}
			iterate_vec++;
		}
	}
	else
		std::cout << "Failed to Open" << std::endl;

	file.close();
}
