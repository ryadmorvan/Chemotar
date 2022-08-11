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
#include "Windows_FileHandling.h"
#include "DrawShapes.h"
#include "Boiler.h"
#include "SteamTable.h"




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


	DrawShapes Boiler = DrawShapes(x + 260, y + 190, 220, 5.0f, DrawShapes::BOILER);
	DrawShapes arrow3_out = DrawShapes(Boiler.returnX() + Boiler.returnLength() + 2, 300.0f + y, 200.0f, 2.0, DrawShapes::ARROW);
	DrawShapes arrow1 = DrawShapes(Boiler.returnX() - 210.0f, 240.0f + y, 200.0f, 2.0f, DrawShapes::ARROW);
	DrawShapes arrow2 = DrawShapes(Boiler.returnX() - 210.0f, 370.0f + y, 200.0f, 2.0f, DrawShapes::ARROW);

	//DrawShapes Turbine = DrawShapes(x + 660, y + 190, 250, 2.0f, DrawShapes::TURBINE);

	DrawShapes Resistor = DrawShapes(Boiler.returnX() + Boiler.returnLength() / 3 - 5, y + 370, 25, 2.5f, DrawShapes::RESISTOR);


	//Load steam table
	static std::unique_ptr<std::vector<std::array<std::string, 15>>> SteamTable_Saturated = std::make_unique<std::vector<std::array<std::string, 15>>>();
	static bool once = []() { LoadSteamTable(*SteamTable_Saturated, "steam_tables/Saturated Steam Table (Temperature).csv"); std::cout << "Done Loading Table Steam" << std::endl;  return true; } ();
	//Load compressed liquid and superheated
	static std::unique_ptr<std::vector<std::array<std::string, 8>>> SteamTable_Compressed = std::make_unique<std::vector<std::array<std::string, 8>>>();
	static bool once1 = []() { LoadSteamTable(*SteamTable_Compressed, "steam_tables/Compressed Liquid and Superheated Steam Table.csv"); std::cout << "Done Loading Table Compressed" << std::endl;  return true; } ();






	//static boiler<FEED::DOUBLE> boil(120, 175, 295, 303.15, 338.15, 125.7, 271.9, 2793, 6, 477.15, true);
	static std::shared_ptr<boiler<FEED::DOUBLE>> boil = std::make_shared<boiler<FEED::DOUBLE>>(120, 175, 30, 65, 0.11, 0.11, 1.8, 6, 204, SteamTable_Compressed);
	boil->setShape(Boiler);

	//float density = CalculateFromSteamTable(SteamTable_Compressed, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::DENSITY, boil->ReturnPhases().at(0));
	//std::cout << "Density: " << density << " Phase: " << boil->ReturnPhases().at(0) << std::endl;
	//boil->CalculateEnthalpy(*SteamTable_Saturated);

	//Feeds Flow Rates
	static float* feeds[3] = { &boil->ReturnFeed1(), &boil->ReturnFeed2(), &boil->ReturnFeed3() };

	static float* FeedTemps[3] = { &boil->ReturnTemp1(), &boil->ReturnTemp2(), &boil->ReturnTemp3() };
	static float* FeedEnthalpies[3] = { &boil->ReturnEnthalpy1(), &boil->ReturnEnthalpy2(), &boil->ReturnEnthalpy3() };

	static float feed_pressure = 0.11;




	ImGui::TextColored(ImColor(100, 200, 100, 200), "Feeds Flow Rates");
	ImGui::SliderFloat2(" In Kg/min", *feeds, 0.0f, 400.0f);
	ImGui::TextColored(ImColor(200, 100, 100, 200), "Feeds Temperatures");
	if (ImGui::SliderFloat3(" In Celsius", *FeedTemps, 5.0f, 300.0f))
	{
		boil->Update(SteamTable_Compressed, feed_pressure, boiler<FEED::DOUBLE>::UPDATING::ENTHALPY);
	}
	ImGui::TextColored(ImColor(100, 100, 200, 200), "Feeds Enthalpies");
	if (ImGui::SliderFloat3(" In kJ/kg", *FeedEnthalpies, 100.0f, 3500.0f))
	{
		boil->Update(SteamTable_Compressed, feed_pressure, boiler<FEED::DOUBLE>::UPDATING::TEMPERATURE);
	}
	//ImGui::SliderFloat("Feed1", &boil->ReturnFeed1(), 0, 200); ImGui::SameLine();
	//ImGui::SliderFloat("Feed2", &boil->ReturnFeed2(), 0, 300); ImGui::SameLine();
	//ImGui::SliderFloat("Feed3", &boil->ReturnFeed3(), 0, 400);

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