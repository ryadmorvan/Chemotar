#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../src/Utility/Windows_FileHandling.h"
#include "../src//Drawing/DrawShapes.h"
#include "Boiler.h"
#include "../src/Tools/SteamTable.h"







static void BoilerSimulation(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Boiler Simulation", p_open))
	{
		return;
	}
	// Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc. 
	// Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4. 
	// ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types) 
	// In this example we are not using the maths operators! 


	//ImDrawListSharedData *draw_list_data = ImGui::GetDrawListSharedData();


	static ImDrawList* draw_list = ImGui::GetWindowDrawList();

	static float sz = 220.0f;
	static float thickness = 3.0f;
	static ImVec4 col = ImVec4(0.9f, 0.9f, 0.9f, 0.8f);
	const ImVec2 p = ImGui::GetCursorScreenPos();
	const ImU32 col32 = ImColor(col);
	float x = p.x + 100.0f, y = p.y + 200.0f, spacing = 15.0f;
	float th = thickness;

	static bool VelocityProfile = FALSE;


	DrawShapes Boiler = DrawShapes(x + 260, y + 190, 270, 5.0f, DrawShapes::BOILER);
	DrawShapes arrow3_out = DrawShapes(Boiler.returnX() + Boiler.returnLength() + 2, 325.0f + y, 200.0f, 2.0, DrawShapes::ARROW);
	DrawShapes arrow1 = DrawShapes(Boiler.returnX() - 210.0f, 225.0f + y, 200.0f, 2.0f, DrawShapes::ARROW);
	DrawShapes arrow2 = DrawShapes(Boiler.returnX() - 210.0f, 405.0f + y, 200.0f, 2.0f, DrawShapes::ARROW);

	//DrawShapes Turbine = DrawShapes(x + 660, y + 190, 250, 2.0f, DrawShapes::TURBINE);

	DrawShapes Resistor = DrawShapes(Boiler.returnX() + Boiler.returnLength() / 3 + 5, y + 420, 25, 2.5f, DrawShapes::RESISTOR);


	//Load steam table
	static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable_Saturated = std::make_unique<std::vector<std::array<std::string, 15>>>();
	static bool once = []() { LoadSteamTable(*SteamTable_Saturated, "steam_tables/Saturated Steam Table (Temperature).csv"); std::cout << "Done Loading Table Steam" << std::endl;  return true; } ();
	//Load compressed liquid and superheated
	static std::unique_ptr<std::vector<std::array<std::string, 8>>> SteamTable_Compressed = std::make_unique<std::vector<std::array<std::string, 8>>>();
	static bool once1 = []() { LoadSteamTable(*SteamTable_Compressed, "steam_tables/Compressed Liquid and Superheated Steam Table.csv"); std::cout << "Done Loading Table Compressed" << std::endl;  return true; } ();






	static std::shared_ptr<boiler<FEED::DOUBLE>> boil = std::make_shared<boiler<FEED::DOUBLE>>(120, 175, 30, 65, 0.11, 0.11, 1.8, 204, SteamTable_Compressed);
	boil->setShape(Boiler);

	//float density = CalculateFromSteamTable(SteamTable_Compressed, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::DENSITY, boil->ReturnPhases().at(0));
	//std::cout << "Density: " << density << " Phase: " << boil->ReturnPhases().at(0) << std::endl;
	//boil->CalculateEnthalpy(*SteamTable_Saturated);

	//Feeds Flow Rates
	static float* feeds[3] = { &boil->ReturnFeed1(), &boil->ReturnFeed2(), &boil->ReturnFeed3() };

	static float* FeedTemps[3] = { &boil->ReturnTemp1(), &boil->ReturnTemp2(), &boil->ReturnTemp3() };
	static float* FeedEnthalpies[3] = { &boil->ReturnEnthalpy1(), &boil->ReturnEnthalpy2(), &boil->ReturnEnthalpy3() };

	static float feed_pressure = 0.11;



	static float FeedDiameters[3] = { 10, 10, 10 };

	ImGui::TextColored(ImColor(100, 100, 100, 150), "Ctrl + Left Click to input value inside the slider");
	ImGui::TextColored(ImColor(100, 200, 100, 200), "Feeds Flow Rates");
	ImGui::SliderFloat2(" In Kg/min", *feeds, 0.0f, 400.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::TextColored(ImColor(200, 100, 100, 200), "Feeds Temperatures");
	if (ImGui::SliderFloat3(" In Celsius", *FeedTemps, 5.0f, 500.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
	{
		boil->Update(SteamTable_Compressed, feed_pressure, boiler<FEED::DOUBLE>::UPDATING::ENTHALPY);
	}

	ImGui::TextColored(ImColor(100, 100, 200, 200), "Feeds Enthalpies");
	if (ImGui::SliderFloat3(" In kJ/kg", *FeedEnthalpies, 100.0f, 3500.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
	{
		boil->Update(SteamTable_Compressed, feed_pressure, boiler<FEED::DOUBLE>::UPDATING::TEMPERATURE);
	}
	PressureList(boil, SteamTable_Compressed);

	ImGui::Checkbox("Enable Velocity Profile", &VelocityProfile);
	if (VelocityProfile)
	{
		boil->EnableVelocityProfile();
		boil->CalculateVelocity(SteamTable_Compressed);

		if (ImGui::SliderFloat3(" Diameter in Cm", FeedDiameters, 5, 200, "%.2f", ImGuiSliderFlags_AlwaysClamp))
		{
			boil->ReturnDiameter1Ref() = FeedDiameters[0];
			boil->ReturnDiameter2Ref() = FeedDiameters[1];
			boil->ReturnDiameter3Ref() = FeedDiameters[2];
		}
	}
	if (VelocityProfile == false) boil->DisabeVelocityProfile();




	boil->Update(SteamTable_Compressed, feed_pressure, boiler<FEED::DOUBLE>::UPDATING::HEAT);




	arrow1.Draw(draw_list);
	arrow2.Draw(draw_list);
	arrow3_out.Draw(draw_list);
	//Boiler.Draw(draw_list);
	boil->Draw(draw_list);
	boil->DrawInfo(draw_list, arrow1, arrow2, arrow3_out);
	//Turbine.Draw(draw_list);

	Resistor.Draw(draw_list);
}