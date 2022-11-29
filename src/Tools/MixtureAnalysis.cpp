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
				mixtures.ShowCurrentComponents();
			}
			ImGui::EndTabItem();
		}
		if(ImGui::BeginTabItem("Multi-Component Mixture"))
		{
			ImGui::EndTabItem();
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
				m_VaporPressures.push_back(0);
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
		//////////////////////////////////////////////////////////////////////////
		if(std::get<1>(Method) == VaporPressure::SHORTCUT)
		{
			if(ImGui::BeginTable("Components", 6, flags, ImVec2(1000, 120)))
			{
				ImGui::TableSetupScrollFreeze(false, true);
				static bool display_headers = false;
				static int contents_type = CT_Text;
				ImGui::TableSetupColumn("Component");
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Temperature Critical (K)");
				ImGui::TableSetupColumn("Pressure Critical (MPa)");
				ImGui::TableSetupColumn("Acentric Factor");
				std::string VaporPressure = "Vapor Pressure @" + _Format(m_temperature, 4) + "K (Mpa)";
				ImGui::TableSetupColumn(VaporPressure.c_str());
				ImGui::TableHeadersRow();
				if(Components.size() > 0)
				{
					for(unsigned n = 0; n < Components.size(); n++)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); ImGui::Text(std::to_string((n+1)).c_str());
						ImGui::TableSetColumnIndex(1); ImGui::Text(Components.at(n).c_str());
						ImGui::TableSetColumnIndex(2); ImGui::Text(_Format(m_CriticalTemperatures.at(n), 4).c_str());
						ImGui::TableSetColumnIndex(3); ImGui::Text(_Format(m_CriticalPressures.at(n), 4).c_str());
						ImGui::TableSetColumnIndex(4); ImGui::Text(_Format(m_AcentricFactors.at(n), 4).c_str());
						ImGui::TableSetColumnIndex(5); ImGui::Text(_Format(m_VaporPressures.at(n), 5).c_str());
					}
				}
			ImGui::EndTable();
			}	
		}

}

void MixtureAnalysis::ResetCurrentComponents()
{
	Components.clear();
	m_CriticalTemperatures.clear();
	m_CriticalPressures.clear();
	m_AcentricFactors.clear();
	m_VaporPressures.clear();
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

void MixtureAnalysis::CalculateVaporPressures()
{
	for(unsigned n = 0; n < Components.size(); n++)
	{
		float VaporPressure = m_CriticalPressures.at(n) * pow(10, 7.0f/3.0f*(1+ m_AcentricFactors.at(n)) * (1- (m_CriticalTemperatures.at(n)/m_temperature)));
		m_VaporPressures.at(n) = VaporPressure;
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
