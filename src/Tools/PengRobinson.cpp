#include "PengRobinson.h"


void PengRobinson::PengRobinsonCalculator(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Peng-Robinson EOS Calculator", p_open))
	{
		return;
	}

	//We create our calculator object which also has our "PhysicalProperties" object class
	//We access our tables through accessing our m_properties object
	//After, we call "returnTable" function with the arguments "PhysicalProperties::type::PhysicalProperty" or "PhysicalProperties::type::HeatCapacity" for our heat capacity table
	//EXAMPLE
	 ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
if(ImGui::BeginTabBar("PengRobinSon",tab_bar_flags))
{
	static PengRobinson PengRobinsonCalculator;
	static PengRobinson PengRobinsonCalculatorRef;
	static bool once = []() { PengRobinsonCalculatorRef.setPressure(0.1f); PengRobinsonCalculatorRef.setTemperature(273.15); return true; } ();
	static const char* current_item = "Choose";

	if(ImGui::BeginTabItem("Compressibility Factor"))
	{
		//PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty))->at(row).at(column).c_str()
		//This will return to us the value from our m_properties.returnTable at certain table type (type::PhysicalProperty) at certain row and colums
		static bool ShowResults = false;
		static bool ShowExtraDetails = false;
		static std::string final_result = "";
		ImGui::TextColored(ImColor(120, 240, 80, 220), "Compounds");
		if(ImGui::BeginCombo("Specie", current_item, ImGuiComboFlags_HeightLarge)) // The second parameter is the label previewed before opening the combo.
			{
				for(unsigned n = 0; n < PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->size(); n++)
				{
					//Checks if the item is selected or not
					bool is_selected = (current_item == PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str()); // You can store your selection however you want, outside or inside your objects
					if(ImGui::Selectable(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str(), is_selected))
					{
						//After selecting the item, it will store all of its data into these variables that will later on be used to display it to the user
						current_item = PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str();
						PengRobinsonCalculator.setCriticalTemperature(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(1)));
						PengRobinsonCalculator.setCriticalPressure(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(2)));
						PengRobinsonCalculator.setAcentricFactor(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(3)));
						PengRobinsonCalculator.setSpecieName(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0));

						//It will set everything for our reference
						PengRobinsonCalculatorRef.setCriticalTemperature(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(1)));
						PengRobinsonCalculatorRef.setCriticalPressure(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(2)));
						PengRobinsonCalculatorRef.setAcentricFactor(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(3)));
						PengRobinsonCalculatorRef.setSpecieName(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0));

						bool success = false;
						for(int i = 0; i < PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->size(); ++i)
						{
							if(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(0) == current_item)
							{
								PengRobinsonCalculator.HeatCapacityCoefficients[0] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(1));
								PengRobinsonCalculator.HeatCapacityCoefficients[1] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(2));
								PengRobinsonCalculator.HeatCapacityCoefficients[2] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(3));
								PengRobinsonCalculator.HeatCapacityCoefficients[3] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(4));
								PengRobinsonCalculator.HeatCapacityCoefficients[4] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(5));

								//for our reference
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[0] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(1));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[1] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(2));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[2] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(3));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[3] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(4));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[4] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(5));
								success = true;
							}
							if(success == true)
							{
								return;
							}
							else 
							{PengRobinsonCalculator.HeatCapacityCoefficients[0] = 0; PengRobinsonCalculator.HeatCapacityCoefficients[1] = 0;
								PengRobinsonCalculator.HeatCapacityCoefficients[2] = 0; PengRobinsonCalculator.HeatCapacityCoefficients[3] = 0;
								PengRobinsonCalculator.HeatCapacityCoefficients[4] = 0;


								PengRobinsonCalculatorRef.HeatCapacityCoefficients[0] = 0; PengRobinsonCalculatorRef.HeatCapacityCoefficients[1] = 0;
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[2] = 0; PengRobinsonCalculatorRef.HeatCapacityCoefficients[3] = 0;
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[4] = 0;
							}
						}
						return;
					}
					if(is_selected)
					ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

		ImGui::SameLine();
		if(ImGui::Button("Details"))
		{
			ImGui::OpenPopup("Details");
		}
		if(ImGui::BeginPopup("Details"))
		{
			ImGui::TextColored(ImColor(59, 254, 225), PengRobinsonCalculator.returnDetails().c_str());
			ImGui::Separator();
			ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)), PengRobinsonCalculator.returnHeatCapacityDetails().c_str());
			ImGui::EndPopup();
		}
	if(!(PengRobinsonCalculator.HeatCapacityCoefficients[0] == 0))
	{
		static float specie_temperature = 273.15;
		static float specie_pressure = 1.5;
		ImGui::InputFloat("Input Temperature", &specie_temperature, 1.0f, 5.0f, "%.2f K",ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Temperature", &specie_temperature, 120, 550, "%.2f K", ImGuiSliderFlags_AlwaysClamp);
		ImGui::InputFloat("Input Pressure", &specie_pressure, 1.0f, 5.0f, "%.2f Mpa",ImGuiSliderFlags_AlwaysClamp );
		ImGui::SliderFloat("Pressure", &specie_pressure, 0.1f, 30.0f, "%.2f Mpa", ImGuiSliderFlags_AlwaysClamp);


		if(ImGui::Checkbox("High Accuracy", PengRobinsonCalculator.solver_flag()))
		{
		}
		ImGui::SameLine();
		if(ImGui::Checkbox("Show Extra Details", &ShowExtraDetails))
		{
		}

		if(ImGui::Button("Calculate"))
		{
			PengRobinsonCalculator.setTemperature(specie_temperature);
			PengRobinsonCalculator.setPressure(specie_pressure);
			PengRobinsonCalculator.Calculate();
			ShowResults = true;
		}

		//Show Results
		if(ShowResults)
		{
			ImGui::Separator();
			PengRobinsonCalculator.ShowResults();
			if(ShowExtraDetails)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)),PengRobinsonCalculator.returnCoefficients().c_str());
			}
		}
		//ImGui::Separator();
		if(ShowResults)
			ImGui::TextColored(ImColor(100, 160, 100, 150), "Stable Root has a lower fugacity");
	
	
	}
		ImGui::EndTabItem();
	}





	if(ImGui::BeginTabItem("Departure Function"))
	{
		//PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty))->at(row).at(column).c_str()
		//This will return to us the value from our m_properties.returnTable at certain table type (type::PhysicalProperty) at certain row and colums
		static bool ShowResults = false;
		static bool ShowExtraDetails = false;
		static bool ShowIdealGasChange = false;
		static std::string final_result = "";
		ImGui::TextColored(ImColor(120, 240, 80, 220), "Compounds");
		if(ImGui::BeginCombo("Specie", current_item, ImGuiComboFlags_HeightLarge)) // The second parameter is the label previewed before opening the combo.
			{
				for(unsigned n = 0; n < PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->size(); n++)
				{
					//Checks if the item is selected or not
					bool is_selected = (current_item == PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str()); // You can store your selection however you want, outside or inside your objects
					if(ImGui::Selectable(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str(), is_selected))
					{
						//After selecting the item, it will store all of its data into these variables that will later on be used to display it to the user
						current_item = PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str();
						PengRobinsonCalculator.setCriticalTemperature(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(1)));
						PengRobinsonCalculator.setCriticalPressure(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(2)));
						PengRobinsonCalculator.setAcentricFactor(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(3)));
						PengRobinsonCalculator.setSpecieName(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0));

						//will setup everything for our reference
						PengRobinsonCalculatorRef.setCriticalTemperature(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(1)));
						PengRobinsonCalculatorRef.setCriticalPressure(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(2)));
						PengRobinsonCalculatorRef.setAcentricFactor(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(3)));
						PengRobinsonCalculatorRef.setSpecieName(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0));
						bool success = false;
						for(int i = 0; i < PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->size(); ++i)
						{
							if(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(0) == current_item)
							{
								PengRobinsonCalculator.HeatCapacityCoefficients[0] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(1));
								PengRobinsonCalculator.HeatCapacityCoefficients[1] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(2));
								PengRobinsonCalculator.HeatCapacityCoefficients[2] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(3));
								PengRobinsonCalculator.HeatCapacityCoefficients[3] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(4));
								PengRobinsonCalculator.HeatCapacityCoefficients[4] = std::stod(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(5));


								//for our reference
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[0] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(1));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[1] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(2));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[2] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(3));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[3] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(4));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[4] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(5));
								success = true;
							}
							if(success == true)
							{
								return;
							}
							else 
							{PengRobinsonCalculator.HeatCapacityCoefficients[0] = 0; PengRobinsonCalculator.HeatCapacityCoefficients[1] = 0;
								PengRobinsonCalculator.HeatCapacityCoefficients[2] = 0; PengRobinsonCalculator.HeatCapacityCoefficients[3] = 0;
								PengRobinsonCalculator.HeatCapacityCoefficients[4] = 0;

								PengRobinsonCalculatorRef.HeatCapacityCoefficients[0] = 0; PengRobinsonCalculatorRef.HeatCapacityCoefficients[1] = 0;
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[2] = 0; PengRobinsonCalculatorRef.HeatCapacityCoefficients[3] = 0;
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[4] = 0;
							}
						}
						return;
					}
					if(is_selected)
					ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

		ImGui::SameLine();
		if(ImGui::Button("Details"))
		{
			ImGui::OpenPopup("Details");
		}
		if(ImGui::BeginPopup("Details"))
		{
			ImGui::TextColored(ImColor(59, 254, 225), PengRobinsonCalculator.returnDetails().c_str());
			ImGui::Separator();
			ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)), PengRobinsonCalculator.returnHeatCapacityDetails().c_str());
			ImGui::EndPopup();
		}
	if(!(PengRobinsonCalculator.HeatCapacityCoefficients[0] == 0))
	{
		static float specie_temperature = 273.15;
		static float specie_pressure = 1.5;
		ImGui::InputFloat("Input Temperature", &specie_temperature, 1.0f, 5.0f, "%.2f K",ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Temperature", &specie_temperature, 120, 550, "%.2f K", ImGuiSliderFlags_AlwaysClamp);
		ImGui::InputFloat("Input Pressure", &specie_pressure, 1.0f, 5.0f, "%.2f Mpa",ImGuiSliderFlags_AlwaysClamp );
		ImGui::SliderFloat("Pressure", &specie_pressure, 0.1f, 30.0f, "%.2f Mpa", ImGuiSliderFlags_AlwaysClamp);


		if(ImGui::Checkbox("High Accuracy", PengRobinsonCalculator.solver_flag()))
		{
		}
		ImGui::SameLine();
		if(ImGui::Checkbox("Show Extra Details", &ShowExtraDetails))
		{
		}
		ImGui::SameLine();
		if(ImGui::Checkbox("Show Ideal Gas Change", &ShowIdealGasChange))
		{
		}

		if(ImGui::Button("Calculate"))
		{
			PengRobinsonCalculator.setTemperature(specie_temperature);
			PengRobinsonCalculator.setPressure(specie_pressure);
			PengRobinsonCalculator.Calculate();
			PengRobinsonCalculator.CalculateChange(PengRobinsonCalculatorRef);
			ShowResults = true;
		}

		//Show Results
		if(ShowResults)
		{
			ImGui::Separator();
			PengRobinsonCalculator.ShowResultsDeparture();
			if(ShowExtraDetails)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)),PengRobinsonCalculator.returnCoefficients().c_str());
			}
		}
		//ImGui::Separator();
		if(ShowResults)
			ImGui::TextColored(ImColor(100, 160, 100, 150), "Stable Root has a lower fugacity");
	
		//Checks if our reference has roots or not
		if((PengRobinsonCalculatorRef.returnZref() != 0) && ShowResults)
		{
			//if it does it will calculate the changes with respect to departure functions, ideal gas and reference state
			PengRobinsonCalculator.ShowResultsChange(PengRobinsonCalculatorRef);
		}
		else
		{
			ImGui::TextColored(ImColor(100, 160, 100, 150), "You need to choose a reference root from the (Reference) Tab");
		}

		//Show Ideal Gas Changes
		if(ShowIdealGasChange)
			PengRobinsonCalculator.ShowIdealGasChange(PengRobinsonCalculatorRef);

	}
		ImGui::EndTabItem();
	}
	if(ImGui::BeginTabItem("Reference State"))
	{
		static const char* current_root = "Choose";
		static std::string root = "0.0";

		static bool ShowResults = false;
		static bool ShowExtraDetails = false;
		static std::string final_result = "";
		ImGui::TextColored(ImColor(120, 240, 80, 220), "Compounds");
		*PengRobinsonCalculatorRef.solver_flag() = true;

		if(ImGui::BeginCombo("Specie", current_item, ImGuiComboFlags_HeightLarge)) // The second parameter is the label previewed before opening the combo.
			{
				for(unsigned n = 0; n < PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->size(); n++)
				{
					//Checks if the item is selected or not
					bool is_selected = (current_item == PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str()); // You can store your selection however you want, outside or inside your objects
					if(ImGui::Selectable(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str(), is_selected))
					{
						//After selecting the item, it will store all of its data into these variables that will later on be used to display it to the user
						current_item = PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str();
						PengRobinsonCalculatorRef.setCriticalTemperature(std::stof(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(1)));
						PengRobinsonCalculatorRef.setCriticalPressure(std::stof(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(2)));
						PengRobinsonCalculatorRef.setAcentricFactor(std::stof(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(3)));
						PengRobinsonCalculatorRef.setSpecieName(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0));

						//It will setup everything for our main function
						PengRobinsonCalculator.setCriticalTemperature(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(1)));
						PengRobinsonCalculator.setCriticalPressure(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(2)));
						PengRobinsonCalculator.setAcentricFactor(std::stof(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(3)));
						PengRobinsonCalculator.setSpecieName(PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0));

						bool success = false;
						for(int i = 0; i < PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->size(); ++i)
						{
							if(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(0) == current_item)
							{
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[0] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(1));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[1] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(2));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[2] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(3));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[3] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(4));
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[4] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(5));

								//for our main function
								PengRobinsonCalculator.HeatCapacityCoefficients[0] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(1));
								PengRobinsonCalculator.HeatCapacityCoefficients[1] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(2));
								PengRobinsonCalculator.HeatCapacityCoefficients[2] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(3));
								PengRobinsonCalculator.HeatCapacityCoefficients[3] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(4));
								PengRobinsonCalculator.HeatCapacityCoefficients[4] = std::stod(PengRobinsonCalculatorRef.m_properties.returnTable(PhysicalProperties::type::HeatCapaicty)->at(i).at(5));
								success = true;
							}
							if(success == true)
							{
								return;
							}
							else 
							{PengRobinsonCalculatorRef.HeatCapacityCoefficients[0] = 0; PengRobinsonCalculatorRef.HeatCapacityCoefficients[1] = 0;
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[2] = 0; PengRobinsonCalculatorRef.HeatCapacityCoefficients[3] = 0;
								PengRobinsonCalculatorRef.HeatCapacityCoefficients[4] = 0;

								//for our main function
								PengRobinsonCalculator.HeatCapacityCoefficients[0] = 0; PengRobinsonCalculator.HeatCapacityCoefficients[1] = 0;
								PengRobinsonCalculator.HeatCapacityCoefficients[2] = 0; PengRobinsonCalculator.HeatCapacityCoefficients[3] = 0;
								PengRobinsonCalculator.HeatCapacityCoefficients[4] = 0;


							}
						}
						return;
					}
					if(is_selected)
					ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

		ImGui::SameLine();
		if(ImGui::Button("Details"))
		{
			ImGui::OpenPopup("Details");
		}
		if(ImGui::BeginPopup("Details"))
		{
			ImGui::TextColored(ImColor(59, 254, 225), PengRobinsonCalculatorRef.returnDetails().c_str());
			ImGui::Separator();
			ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)), PengRobinsonCalculatorRef.returnHeatCapacityDetails().c_str());
			ImGui::EndPopup();
		}
	if(!(PengRobinsonCalculatorRef.HeatCapacityCoefficients[0] == 0))
	{
		static float specie_temperature = 273.15;
		static float specie_pressure = 0.1;
		ImGui::InputFloat("Input Temperature", &specie_temperature, 1.0f, 5.0f, "%.2f K",ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Temperature", &specie_temperature, 120, 550, "%.2f K", ImGuiSliderFlags_AlwaysClamp);
		ImGui::InputFloat("Input Pressure", &specie_pressure, 1.0f, 5.0f, "%.2f Mpa",ImGuiSliderFlags_AlwaysClamp );
		ImGui::SliderFloat("Pressure", &specie_pressure, 0.1f, 30.0f, "%.2f Mpa", ImGuiSliderFlags_AlwaysClamp);


		//if(ImGui::Checkbox("High Accuracy", PengRobinsonCalculatorRef.solver_flag()));
		//ImGui::SameLine();

		if(ImGui::Checkbox("Show Extra Details", &ShowExtraDetails));

		if(ImGui::Button("Calculate"))
		{
			PengRobinsonCalculatorRef.setTemperature(specie_temperature);
			PengRobinsonCalculatorRef.setPressure(specie_pressure);
			PengRobinsonCalculatorRef.Calculate();
			ShowResults = true;
		}

		//Show Results
		if(ShowResults)
		{
			ImGui::Separator();
			PengRobinsonCalculatorRef.ShowResultsReference();
			if(ShowExtraDetails)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)),PengRobinsonCalculatorRef.returnCoefficients().c_str());
			}
		}
		//ImGui::Separator();
		if(ShowResults)
		{
			//Prompts user to choose the appropriate roots to use for calculations
			ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)), "Choose a reference root");
			ImGui::TextColored(ImColor(100, 160, 100, 150), "Stable Root has a lower fugacity");
			if(ImGui::BeginCombo("Root", current_root, ImGuiComboFlags_HeightLarge))
			{ // You can store your selection however you want, outside or inside your objects
				for(float Z : *PengRobinsonCalculatorRef.returnSolutions())
				{
					bool is_selected = (current_root == _Format(Z, 5));
					if(ImGui::Selectable(_Format(Z, 5).c_str(), is_selected))
					{
						root = _Format(Z, 5);
						current_root = root.c_str();

						PengRobinsonCalculatorRef.set_Z_Ref(Z);
					}
				}
			}
		}
	
	
	}
		ImGui::EndTabItem();
	}
}

}

std::vector<float> PengRobinson::CubicEquationSolver()
{
	//Peng Robin Song Equation
	// Z^3 + (-1+B)Z^2 + (A-3B^2-2B)Z + (-AB + B^2 + B^3) = 0
	std::vector<float> solutions;
	if(solver_flag_accuracy == false)
{
	float coeff1 = -1 + m_B;
	float coeff2 = m_A - 3*pow(m_B,2) - 2*m_B;
	float coeff3 = -m_A*m_B + pow(m_B, 2) + pow(m_B, 3);


		float Z = 0.00000f;
	jump:
		Z+= 0.000001f;
		float value;
		if(Z >= 1.0f)
		{
			goto end_process;
		}
		value = pow(Z, 3) + coeff1*pow(Z, 2) + coeff2*pow(Z, 1) + coeff3;
		//std::cout << value << std::endl;
		if((0.000001 >= value) && (value >= -0.000001))
		{
			if(solutions.empty())
			{
				solutions.push_back(Z);
			}
			if(!solutions.empty())
			{
				for(auto solution : solutions)
				{
					if(((Z < solution + 0.001f) && (Z > solution -0.001f)))
					{
						goto jump;
					}
				}
				solutions.push_back(Z);
			}
		}
		goto jump;
}

if(solver_flag_accuracy == true)
{
	float coeff1 = -1 + m_B;
	float coeff2 = m_A - 3*pow(m_B,2) - 2*m_B;
	float coeff3 = -m_A*m_B + pow(m_B, 2) + pow(m_B, 3);


		float Z = 0.00000f;
	jump1:
		Z+= 0.0000001f;
		float value;
		if(Z >= 1.0f)
		{
			goto end_process;
		}
		value = pow(Z, 3) + coeff1*pow(Z, 2) + coeff2*pow(Z, 1) + coeff3;
		//std::cout << value << std::endl;
		if((0.0000001 >= value) && (value >= -0.0000001))
		{
			if(solutions.empty())
			{
				solutions.push_back(Z);
			}
			if(!solutions.empty())
			{
				for(auto solution : solutions)
				{
					if(((Z < solution + 0.001f) && (Z > solution -0.001f)))
					{
						goto jump1;
					}
				}
				solutions.push_back(Z);
			}
		}
		goto jump1;
}


	end_process:
	return solutions;
}

//Calculates the change for our departure function
void PengRobinson::CalculateChange(PengRobinson& reference)
{
	IdealGasEnthalpyChange(reference.returnTemperature(), m_temperature);
	IdealGasEntropyChange(reference.returnTemperature(), m_temperature, reference.returnPressure(), m_pressure);
	IdealGasInternalEnergyChange(reference.returnTemperature(), m_temperature);
}





void PengRobinson::ShowResults()
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
			| ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY;
		//////////////////////////////////////////////////////////////////////////
		if (ImGui::BeginTable("Roots Results", 4, flags, ImVec2(650, 100)))
		{
			ImGui::TableSetupScrollFreeze(false, true);
			static bool display_headers = false;
			static int contents_type = CT_Text;
			ImGui::TableSetupColumn("Roots");
			ImGui::TableSetupColumn("Molar Volume (cm^3/mol)");
			ImGui::TableSetupColumn("Density (mol/m^3)");
			ImGui::TableSetupColumn("Fugacity (MPa)");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			if(solutions.size() > 0)
			{
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(0), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(1/CalculateMolarVolume(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateFugacity(solutions.at(0)), 5).c_str());
			}

			if(solutions.size() > 1)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(1), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(1/CalculateMolarVolume(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateFugacity(solutions.at(1)), 5).c_str());
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(2), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(1/CalculateMolarVolume(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateFugacity(solutions.at(2)), 5).c_str());
			}
			ImGui::EndTable();
		}

		//if(solutions.size() == 0)
		//{
		//	ImGui::TextColored(ImColor(59, 254, 225), "No Root Exist");
		//}
}

void PengRobinson::ShowResultsReference()
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
			| ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY;
		//////////////////////////////////////////////////////////////////////////
		if (ImGui::BeginTable("Roots Results", 6, flags, ImVec2(900, 100)))
		{
			ImGui::TableSetupScrollFreeze(false, true);
			static bool display_headers = false;
			static int contents_type = CT_Text;
			ImGui::TableSetupColumn("Roots");
			ImGui::TableSetupColumn("Molar Volume (cm^3/mol)");
			ImGui::TableSetupColumn("Fugacity (MPa)");
			ImGui::TableSetupColumn("H - H(ig) (J/mol)");
			ImGui::TableSetupColumn("U - U(ig) (J/mol)");
			ImGui::TableSetupColumn("S - S(ig) (J/k*mol)");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			if(solutions.size() > 0)
			{
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(0), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(CalculateFugacity(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateEnthalpyDeparture(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(CalculateInternalDeparture(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(5); ImGui::Text(_Format(CalculateEntropyDeparture(solutions.at(0)), 5).c_str());
			}

			if(solutions.size() > 1)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(1), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(CalculateFugacity(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateEnthalpyDeparture(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(CalculateInternalDeparture(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(5); ImGui::Text(_Format(CalculateEntropyDeparture(solutions.at(1)), 5).c_str());
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(2), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(CalculateFugacity(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateEnthalpyDeparture(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(CalculateInternalDeparture(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(5); ImGui::Text(_Format(CalculateEntropyDeparture(solutions.at(2)), 5).c_str());
			}
			ImGui::EndTable();
		}
}


void PengRobinson::ShowResultsDeparture()
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
			| ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY;
		//////////////////////////////////////////////////////////////////////////
		if (ImGui::BeginTable("Roots Results", 6, flags, ImVec2(900, 100)))
		{
			ImGui::TableSetupScrollFreeze(false, true);
			static bool display_headers = false;
			static int contents_type = CT_Text;
			ImGui::TableSetupColumn("Roots");
			ImGui::TableSetupColumn("Molar Volume (cm^3/mol)");
			ImGui::TableSetupColumn("Fugacity (MPa)");
			ImGui::TableSetupColumn("H - H(ig) (J/mol)");
			ImGui::TableSetupColumn("U - U(ig) (J/mol)");
			ImGui::TableSetupColumn("S - S(ig) (J/k*mol)");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			if(solutions.size() > 0)
			{
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(0), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(CalculateFugacity(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateEnthalpyDeparture(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(CalculateInternalDeparture(solutions.at(0)), 5).c_str());
				ImGui::TableSetColumnIndex(5); ImGui::Text(_Format(CalculateEntropyDeparture(solutions.at(0)), 5).c_str());
			}

			if(solutions.size() > 1)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(1), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(CalculateFugacity(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateEnthalpyDeparture(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(CalculateInternalDeparture(solutions.at(1)), 5).c_str());
				ImGui::TableSetColumnIndex(5); ImGui::Text(_Format(CalculateEntropyDeparture(solutions.at(1)), 5).c_str());
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(2), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(CalculateMolarVolume(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(CalculateFugacity(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(CalculateEnthalpyDeparture(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(CalculateInternalDeparture(solutions.at(2)), 5).c_str());
				ImGui::TableSetColumnIndex(5); ImGui::Text(_Format(CalculateEntropyDeparture(solutions.at(2)), 5).c_str());
			}
			ImGui::EndTable();
		}
}



void PengRobinson::ShowIdealGasChange(PengRobinson &reference)
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
			| ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY;
		//////////////////////////////////////////////////////////////////////////
		ImGui::Separator();
		ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)),"Ideal Gas Changes");
		if (ImGui::BeginTable("Ideal Gas Change", 5, flags, ImVec2(800, 50)))
		{
			ImGui::TableSetupScrollFreeze(false, true);
			static bool display_headers = false;
			static int contents_type = CT_Text;
			ImGui::TableSetupColumn("Reference Temperature");
			ImGui::TableSetupColumn("Final Temperature");
			ImGui::TableSetupColumn("H - H(R) (J/mol)");
			ImGui::TableSetupColumn("U - U(R) (J/mol)");
			ImGui::TableSetupColumn("S - S(R) (J/k*mol)");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(reference.returnTemperature(), 5).c_str());
			ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(m_temperature, 5).c_str());
			ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(m_EnthalpyChangeIdeal, 5).c_str());
			ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(m_InternalEnergyChangeIdeal, 5).c_str());
			ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(m_EntropyChangeIdeal, 5).c_str());
			ImGui::EndTable();
		}
}



void PengRobinson::ShowResultsChange(PengRobinson &reference)
{
	enum ContentsType { CT_Text, CT_FillButton };
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
		static int contents_type = CT_Text;
		//Table Flags
		//flags |= ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH
		//	| ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner;
		//flags |= ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable;

		auto calculateChange = [](float Departure1, float Departure2, float IdealGasChange)
		{
			return Departure2 + IdealGasChange - Departure1;
		};


		flags |= ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
			| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY;
		//////////////////////////////////////////////////////////////////////////
		ImGui::Separator();
		ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)),"Fluid changes from state 1 to state 2");
		if (ImGui::BeginTable("Changes", 4, flags, ImVec2(600, 100)))
		{
			ImGui::TableSetupScrollFreeze(false, true);
			static bool display_headers = false;
			static int contents_type = CT_Text;
			ImGui::TableSetupColumn("Roots");
			ImGui::TableSetupColumn("H2 - H1 (J/mol)");
			ImGui::TableSetupColumn("U2 - U1 (J/mol)");
			ImGui::TableSetupColumn("S2 - S1 (J/k*mol)");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			if(solutions.size() > 0)
			{
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(0), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(calculateChange(reference.CalculateEnthalpyDeparture(reference.returnZref()), CalculateEnthalpyDeparture(solutions.at(0)), m_EnthalpyChangeIdeal), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(calculateChange(reference.CalculateInternalDeparture(reference.returnZref()), CalculateInternalDeparture(solutions.at(0)), m_InternalEnergyChangeIdeal), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(calculateChange(reference.CalculateEntropyDeparture(reference.returnZref()), CalculateEntropyDeparture(solutions.at(0)), m_EntropyChangeIdeal), 5).c_str());
			}

			if(solutions.size() > 1)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(1), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(calculateChange(reference.CalculateEnthalpyDeparture(reference.returnZref()), CalculateEnthalpyDeparture(solutions.at(1)), m_EnthalpyChangeIdeal), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(calculateChange(reference.CalculateInternalDeparture(reference.returnZref()), CalculateInternalDeparture(solutions.at(1)), m_InternalEnergyChangeIdeal), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(calculateChange(reference.CalculateEntropyDeparture(reference.returnZref()), CalculateEntropyDeparture(solutions.at(1)), m_EntropyChangeIdeal), 5).c_str());
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); ImGui::Text(_Format(solutions.at(2), 5).c_str());
				ImGui::TableSetColumnIndex(1); ImGui::Text(_Format(calculateChange(reference.CalculateEnthalpyDeparture(reference.returnZref()), CalculateEnthalpyDeparture(solutions.at(2)), m_EnthalpyChangeIdeal), 5).c_str());
				ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(calculateChange(reference.CalculateInternalDeparture(reference.returnZref()), CalculateInternalDeparture(solutions.at(2)), m_InternalEnergyChangeIdeal), 5).c_str());
				ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(calculateChange(reference.CalculateEntropyDeparture(reference.returnZref()), CalculateEntropyDeparture(solutions.at(2)), m_EntropyChangeIdeal), 5).c_str());
			}
		ImGui::EndTable();
		}
}
