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
	static PengRobinson PengRobinsonCalculator;
	//PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty))->at(row).at(column).c_str()
	//This will return to us the value from our m_properties.returnTable at certain table type (type::PhysicalProperty) at certain row and colums
	static const char* current_item = "Choose";

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
						success = true;
					}
					if(success == true)
					{
						return;
					}
					else {PengRobinsonCalculator.HeatCapacityCoefficients[0] = 0; PengRobinsonCalculator.HeatCapacityCoefficients[1] = 0;
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
		ImGui::TextColored(ImColor(59, 254, 225), PengRobinsonCalculator.returnDetails().c_str());
		ImGui::Separator();
		ImGui::TextColored(ImColor(ImVec4(0.898f, 0.845f, 0.710f, 0.95f)), PengRobinsonCalculator.returnHeatCapacityDetails().c_str());
		ImGui::EndPopup();
	}
	

	ImGui::InputFloat("Input Temperature", PengRobinsonCalculator.returnTemperaturePointer(), 1.0f, 5.0f, "%.2f K",ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderFloat("Temperature", PengRobinsonCalculator.returnTemperaturePointer(), 120, 550, "%.2f K", ImGuiSliderFlags_AlwaysClamp);
	ImGui::InputFloat("Input Pressure", PengRobinsonCalculator.returnPressurePointer(), 1.0f, 5.0f, "%.2f Mpa",ImGuiSliderFlags_AlwaysClamp );
	ImGui::SliderFloat("Pressure", PengRobinsonCalculator.returnPressurePointer(), 0.01f, 80.0f, "%.2f Mpa", ImGuiSliderFlags_AlwaysClamp);

	if(ImGui::Button("Calculate"))
	{
		PengRobinsonCalculator.Calculate();
	}

	ImGui::Text(PengRobinsonCalculator.returnCoefficients().c_str());
	//static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	//	flags |= ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
	//	| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
	//	| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
	//	| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
	//	| ImGuiTableFlags_SizingFixedFit;

	//if(ImGui::BeginTable("Physical Properties", 6, flags))
	//{
	//	ImGui::TableSetupScrollFreeze(false, true);
	//	static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	//	static bool display_headers = false;
	//	//ImGui::TableSetupColumn(" ");
	//	ImGui::TableSetupColumn("Compound");
	//	ImGui::TableSetupColumn("Molecular Weight (g/mol)");
	//	ImGui::TableSetupColumn("Melting Temperature (C)");
	//	ImGui::TableSetupColumn("Boiling Temperature (C)");
	//	ImGui::TableSetupColumn("Critical Temperature (K)");
	//	ImGui::TableSetupColumn("Critical Pressure (atm)");
	//	ImGui::TableHeadersRow();

	//	static int value = 0;
	//		std::vector<bool> placeholder;
	//		//We access our tables through accessing our m_properties object
	//		//After, we call "returnTable" function with the arguments "PhysicalProperties::type::PhysicalProperty" or "PhysicalProperties::type::HeatCapacity" for our heat capacity table
	//		placeholder.resize((PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty))->size()); //APPROPRIATE RESIZE OF OUR VECTOR
	//		for (int row = 0; row < (PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty))->size(); row++)
	//		{
	//			ImGui::TableNextRow();
	//			for (int column = 0; column < 6; column++)
	//			{
	//				ImGui::TableSetColumnIndex(column);
	//				if ((value == row) && (value != 0))
	//				{
	//					//When the row is selected, we highlight it
	//					ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.8f, 0.9f), (PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty))->at(row).at(column).c_str());
	//				}
	//				else if (ImGui::Selectable((PengRobinsonCalculator.m_properties.returnTable(PhysicalProperties::type::PhysicalProperty))->at(row).at(column).c_str(), placeholder.at(row),
	//				                           ImGuiSelectableFlags_SpanAllColumns |
	//				                           ImGuiSelectableFlags_AllowItemOverlap))
	//				{
	//					placeholder.at(row) = true;
	//					value = row;
	//				}
	//			}
	//		}
	//		ImGui::EndTable();
	//}



}
