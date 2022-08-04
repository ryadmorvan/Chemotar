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


static void BoilerSimulation(bool *p_open)
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

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	static float sz = 220.0f;
	static float thickness = 3.0f;
	static ImVec4 col = ImVec4(0.9f, 0.9f, 0.9f, 0.8f);
	const ImVec2 p = ImGui::GetCursorScreenPos();
	const ImU32 col32 = ImColor(col);
	float x = p.x + 100.0f, y = p.y + 200.0f, spacing = 15.0f;
	float th = thickness;


	draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz + 100, y + sz + 100), col32, 10.0f, ImDrawCornerFlags_All, th);

	/*draw_list->AddTriangleFilled(ImVec2(x + 50, y+ 50), ImVec2(x+ 50, y + 60), ImVec2(x+ 60, y + 55), ImColor(255, 0, 0));*/


	DrawShapes arrow1 = DrawShapes(50.0f + x, 250.0f + y, 200.0f, 2.0f, DrawShapes::ARROW);
	DrawShapes arrow2 = DrawShapes(50.0f + x, 350.0f + y, 200.0f, 2.0f, DrawShapes::ARROW);

	arrow1.Draw(draw_list);
	arrow2.Draw(draw_list);

}