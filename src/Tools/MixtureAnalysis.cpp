#include "MixtureAnalysis.h"

void MixtureAnalysis::MixtureAnalyisTool(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Mixture Analysis", p_open))
	{
		return;
	}

	static MixtureAnalysis mixtures;
	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if(ImGui::BeginTabBar("Mixture Analysis", tab_bar_flags))
	{

		if(ImGui::BeginTabItem("Binary Mixture"))
		{
			mixtures.VaporPressureCalculationMethod();
			if(!(std::get<1>(mixtures.VaporPressureMethod()) == MixtureAnalysis::VaporPressure::NONE))
			{
				mixtures.SpeciesList();
				if(ImGui::Button("Add Specie"))
				{
					mixtures.AddCurrentSpecie();
				}
				ImGui::SameLine();
				if(ImGui::Button("Reset Components"))
				{
					mixtures.ResetCurrentComponents();
				}
				mixtures.InputTempPressure();
				mixtures.ChooseMoleFraction();
				mixtures.ShowCurrentComponents();
				mixtures.RunMixtureAnalysis();
			}
			ImGui::EndTabItem();
		}
		if(ImGui::BeginTabItem("Multi-Component Mixture"))
		{
			ImGui::EndTabItem();
		}
	}
}

void MixtureAnalysis::InputMolarFractions(std::tuple<bool, unsigned int> &change_molefraction)
{
	if(std::get<0>(change_molefraction))
	{
		if(current_state == State::BUBBLEPOINT)
		{
			ImGui::OpenPopup("Input Mole Fraction");
			if(ImGui::BeginPopup("Input Mole Fraction"))
			{
				ImGui::TextColored(ImColor(59, 254, 225), "Input Mole Fraction");
				ImGui::InputFloat("", &liquidFraction_Components.at(std::get<1>(change_molefraction)), 0.01f, 0.02f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				if(ImGui::Button("Set")) {			change_molefraction = std::make_pair(false, 0);}
				ImGui::EndPopup();
			}

		}
	}
}

void MixtureAnalysis::SpeciesList()
{
	if(std::get<1>(Method) == VaporPressure::SHORTCUT)
	{
		if(ImGui::BeginCombo("Specie", current_specie, ImGuiComboFlags_HeightLarge))
		{
			for(unsigned n = 0; n < m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->size(); n++)
			{
				bool is_selected = (current_specie == m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str());
				if(ImGui::Selectable(m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str(), is_selected))
				{
					current_specie = m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0).c_str();
				}
			}
		ImGui::EndCombo();
		}
	}
	if(std::get<1>(Method) == VaporPressure::ANTOINE)
	{
		
		if(ImGui::BeginCombo("Specie", current_specie, ImGuiComboFlags_HeightLarge))
		{
			for(unsigned n = 0; n < AntoineCalculator.AntoineConstants->size(); n++)
			{
				bool is_selected = (current_specie == AntoineCalculator.AntoineConstants->at(n).at(0).c_str()); //Check if we have chose it
				if(ImGui::Selectable(AntoineCalculator.AntoineConstants->at(n).at(0).c_str(), is_selected))
				{
					current_specie = AntoineCalculator.AntoineConstants->at(n).at(0).c_str();
				}
			}
			ImGui::EndCombo();
		}
	}
}

void MixtureAnalysis::AddCurrentSpecie()
{
	if(std::get<1>(Method) == VaporPressure::SHORTCUT)
	{
		for(unsigned n = 0; n < m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->size(); n++)
		{
			if(current_specie == m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0))
			{
				Components.push_back(m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(0));
				m_CriticalTemperatures.push_back(std::stof(m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(1)));
				m_CriticalPressures.push_back(std::stof(m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(2)));
				m_AcentricFactors.push_back(std::stof(m_properties.returnTable(PhysicalProperties::type::PhysicalProperty)->at(n).at(3)));
				m_VaporPressures.push_back(0);	liquidFraction_Components.push_back(0); vaporFraction_Components.push_back(0);
			}
		}
		CalculateVaporPressures();
	}
}

void MixtureAnalysis::ShowCurrentComponents()
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

		static std::tuple<bool, unsigned int> changevalue;
		static bool once = []() { changevalue = std::make_tuple<bool, unsigned>(false, 0); return true; } ();


		//////////////////////////////////////////////////////////////////////////
		if(std::get<1>(Method) == VaporPressure::SHORTCUT)
		{
			if(ImGui::BeginTable("Components", 5, flags, ImVec2(1000, 200)))
			{
				ImGui::TableSetupScrollFreeze(false, true);
				static bool display_headers = false;
				static int contents_type = CT_Text;
				ImGui::TableSetupColumn("Component");
				ImGui::TableSetupColumn("Name");
				//ImGui::TableSetupColumn("Temperature Critical (K)");
				//ImGui::TableSetupColumn("Pressure Critical (MPa)");
				//ImGui::TableSetupColumn("Acentric Factor");

				std::string VaporPressure = "Vapor Pressure @" + _Format(m_temperature, 4) + "K (Mpa)";
				ImGui::TableSetupColumn(VaporPressure.c_str());

				//It will change according to what the user chooses
				std::string fraction_type = "Mole Fraction";
				std::string fraction_type2 = "Mole Fraction";
				fraction_type += (molefraction_type == FRACTIONS::LIQUID) ? " (Liquid)" : " (Vapor)";
				fraction_type2 += (molefraction_type != FRACTIONS::LIQUID) ? " (Liquid)" : " (Vapor)";
				ImGui::TableSetupColumn(fraction_type.c_str(), ImGuiTableColumnFlags_WidthFixed, 0.0f);
				ImGui::TableSetupColumn(fraction_type2.c_str(), ImGuiTableColumnFlags_WidthFixed, 0.0f);

				ImGui::TableHeadersRow();
				if(Components.size() > 0)
				{
					for(unsigned n = 0; n < Components.size(); n++)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text(std::to_string((n+1)).c_str());
						ImGui::TableSetColumnIndex(1); ImGui::Text(Components.at(n).c_str());
						//ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(m_CriticalTemperatures.at(n), 4).c_str());
						//ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(m_CriticalPressures.at(n), 4).c_str());
						//ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(m_AcentricFactors.at(n), 4).c_str());
						ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(m_VaporPressures.at(n), 5).c_str());
						ImGui::TableSetColumnIndex(3); if(ImGui::SmallButton((liquidFraction_Components.at(n) == 0) ? "Input Mole Fraction" : _Format(liquidFraction_Components.at(n), 4).c_str())) changevalue = std::make_pair(true, n);
						ImGui::TableSetColumnIndex(4); if(current_state == State::BUBBLEPOINT) {ImGui::Text(_Format(vaporFraction_Components.at(n), 4).c_str());}
					}
				}
			ImGui::EndTable();
			}
			InputMolarFractions(changevalue);
		}

}

void MixtureAnalysis::ResetCurrentComponents()
{
	Components.clear();
	m_CriticalTemperatures.clear();
	m_CriticalPressures.clear();
	m_AcentricFactors.clear();
	m_VaporPressures.clear();
	liquidFraction_Components.clear();
	vaporFraction_Components.clear();
}

void MixtureAnalysis::VaporPressureCalculationMethod()
{
	if(ImGui::BeginCombo("Methods", std::get<0>(Method).c_str(), ImGuiComboFlags_HeightLarge))
	{
		for(unsigned n = 0; n < methods.size(); n++)
		{
			bool is_selected = (std::get<0>(Method).c_str() == methods.at(n));
			if(ImGui::Selectable(methods.at(n).c_str(), is_selected))
			{
				std::get<0>(Method) = methods.at(n);
				if(n == 0) {std::get<1>(Method) = VaporPressure::ANTOINE;} else if(n == 1) {std::get<1>(Method) = VaporPressure::SHORTCUT;}
				else {std::get<1>(Method) = VaporPressure::NONE;}
				current_specie = "Choose";
				ResetCurrentComponents();
			}
		}
		ImGui::EndCombo();
	}
}

MixtureAnalysis::FRACTIONS MixtureAnalysis::ChooseMoleFraction()
{
	static int selection = 1;
	//prompts the user to choose which fraction type they going to use
	ImGui::RadioButton("Liquid Fraction", &selection, 1); ImGui::SameLine();
	ImGui::RadioButton("Vapor Fraction", &selection, 2);
	molefraction_type = (selection == 1) ? FRACTIONS::LIQUID : FRACTIONS::VAPOR;
	current_state = (selection == 1) ? State::BUBBLEPOINT : State::DEWPOINT; //We choose either bubble or dewpoint
	//Chooses which fraction type they will use
	return (selection == 1) ? FRACTIONS::LIQUID : FRACTIONS::VAPOR;
}

void MixtureAnalysis::CalculateVaporPressures()
{
	for(unsigned n = 0; n < Components.size(); n++)
	{
		float VaporPressure = m_CriticalPressures.at(n) * pow(10, 7.0f/3.0f*(1+ m_AcentricFactors.at(n)) * (1- (m_CriticalTemperatures.at(n)/m_temperature)));
		m_VaporPressures.at(n) = VaporPressure;
	}
}

void MixtureAnalysis::RunMixtureAnalysis()
{
	if(ImGui::Button("Run Mixture Analysis"))
	{
		//We check if all the fractions add to 1
		if(current_state == State::BUBBLEPOINT)
		{
			//float total_fraction = 0;
			//for(auto fraction : liquidFraction_Components)
			//{
			//	total_fraction += fraction;
			//}
			//if(total_fraction != 1.00)
			//{
			//	ImGui::OpenPopup("Error"); if(ImGui::BeginPopup("Error")) {ImGui::TextColored(ImColor(204, 51, 0), "All fractions must add to 1"); if(ImGui::Button("OK")) {ImGui::EndPopup(); return;}}
			//}
			m_pressure = 0;
			for(unsigned n = 0; n < Components.size(); n++)
			{
				m_pressure += liquidFraction_Components.at(n)*m_VaporPressures.at(n);
			}
			for(unsigned n = 0; n < Components.size(); n++)
			{
				vaporFraction_Components.at(n) = (liquidFraction_Components.at(n)*m_VaporPressures.at(n))/m_pressure;
			}
		}
	}
}

void MixtureAnalysis::InputTempPressure()
{
	if(ImGui::InputFloat("Input Temperature", &m_temperature, 5, 10, "%.2f K", ImGuiSliderFlags_AlwaysClamp))
	{
		CalculateVaporPressures();
	}
	if(ImGui::InputFloat("Input Pressure", &m_pressure, 5, 10, "%.2f MPa", ImGuiSliderFlags_AlwaysClamp))
	{
	}
}
