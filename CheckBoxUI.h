#pragma once

#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "ChemicalCalculations.h"




void loadTable(std::string fname)
{

}


void CheckBoxUI(bool* CheckBox, std::string& fname)
{
	ImGui::Checkbox("Elliot Liquid", &CheckBox[0]);
	ImGui::Checkbox("Gas Enthalpy", &CheckBox[1]);
	ImGui::Checkbox("Liquid Enthalpy", &CheckBox[2]);
	if (CheckBox[0])
	{
		fname = "ElliotLiquid.csv";
		static int ChemicalSpecie = 0;
		static const char* Chemicals[] = { "CH4", "H2O" };
		ImGui::ListBox("Chemical Species", &ChemicalSpecie, Chemicals, IM_ARRAYSIZE(Chemicals), 4);

	}
	else if (CheckBox[1])
		fname = "GasEnthalpy.csv";
	else if (CheckBox[2])
		fname = "LiquidEnthalpy.csv";
}