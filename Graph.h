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
#include <math.h>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "Piston.h"




class Graph
{
public:


	enum state
	{
		constant_pressure = 5, constant_volume = 6, constant_temperature = 7, constant_heat = 8
	};

	double x;
	double y;
	double thickness;
	ImVec2 start_pos_x;
	ImVec2 End_pos_x;
	ImVec2 start_pos_y;
	ImVec2 End_pos_y;

	ImVec2 PressureText_pos;
	ImVec2 VolumeText_pos;
	bool Constant_Pressure_Setup = 0;

	class LineGraph
	{
	public:
		ImVec2 start_cord;
		ImVec2 end_cord;
		void setcord(double x, double y, ImVec2 start_pos_x, ImVec2 start_pos_y)
		{
			start_cord = ImVec2(start_pos_x.x + x, start_pos_y.y - y);
			end_cord = ImVec2(start_pos_x.x + x + 1, start_pos_y.y - y);
		}
		void setcord_point(double x, double y, ImVec2 start_pos_x, ImVec2 start_pos_y)
		{
			start_cord = ImVec2(start_pos_x.x + x, start_pos_y.y - y);
			end_cord = ImVec2(start_pos_x.x + x + 5, start_pos_y.y - y);
		}
	};



	ImVec4 Text_col = ImVec4(0.902f, 0.902f, 0.98f , 0.65f);
	std::vector<LineGraph> VectorOfLines;
	std::vector<LineGraph> VectorOfPoints;


	Graph(double x_start_pos, double y_start_pos, double x_end_pos, double y_end_pos, double Thickness)
	{
		x = 0, y = 0;
		start_pos_x = ImVec2(x_start_pos, y_start_pos);
		End_pos_x = ImVec2(x_end_pos, y_start_pos);
		start_pos_y = ImVec2(x_start_pos, y_start_pos);
		End_pos_y = ImVec2(x_start_pos, y_end_pos);
		
		PressureText_pos = ImVec2(x_start_pos - 15, y_end_pos + 5);
		VolumeText_pos = ImVec2(x_end_pos - 10, y_start_pos +5);
		thickness = Thickness;
	}




	void BuildLinesVector(Piston piston, Graph::state state)
	{
		if (state == Graph::state::constant_temperature)
		{
			LineGraph linegraph;
			VectorOfLines = std::vector<LineGraph>();
			for (float pressure = 1.0; pressure <= piston.pressure; pressure += 0.01)
			{
				//piston.PressureChange(pressure, Piston::process::constant_temperature);
				linegraph.setcord_point((piston.returnVolume() - 22.4) * 3.0 + 30, piston.pressure * 50, start_pos_x, start_pos_y);
				VectorOfPoints.push_back(linegraph);
			}
			for (float pressure = 1.0; pressure <= 5; pressure += 0.01)
			{
				piston.PressureChange(pressure, Piston::process::constant_temperature);
				linegraph.setcord((piston.returnVolume() - 22.4) * 3.0 + 30, piston.pressure * 50, start_pos_x, start_pos_y);
				VectorOfLines.push_back(linegraph);
			}

		}
		if (state == Graph::state::constant_heat)
		{
			LineGraph linegraph;
			VectorOfLines = std::vector<LineGraph>();
			for (float pressure = 1.0; pressure <= piston.pressure; pressure += 0.01)
			{
				//piston.PressureChange(pressure, Piston::process::constant_temperature);
				linegraph.setcord_point((piston.returnVolume() - 22.4) * 3.0 + 30, piston.pressure * 50, start_pos_x, start_pos_y);
				VectorOfPoints.push_back(linegraph);
			}
			for (float pressure = 1.0; pressure <= 5.0; pressure += 0.01)
			{
				piston.PressureChange(pressure, Piston::process::constant_heat);
				linegraph.setcord((piston.returnVolume() - 22.4) * 3.0 + 30, piston.pressure * 50, start_pos_x, start_pos_y);
				VectorOfLines.push_back(linegraph);
			}
		}
		if (state == Graph::state::constant_pressure)
		{
			LineGraph linegraph;
			VectorOfLines = std::vector<LineGraph>();

			for (float volume = 0; volume <= 84.3; volume += 0.01) //Max Value of Volume 106.7 - 22.4 = 84.3
			{
				//piston.AddHeat(heat, Piston::process::constant_pressure);
				linegraph.setcord((volume + 22.4) * 2, piston.pressure * 50, start_pos_x, start_pos_y);
				VectorOfLines.push_back(linegraph);
			}


			for (float heat = 0; heat <= 30; heat += 0.01)
			{
				/*piston.AddHeat(heat, Piston::process::constant_pressure);*/
				linegraph.setcord_point((piston.returnVolume()) * 2 , piston.pressure * 50, start_pos_x, start_pos_y);
				VectorOfPoints.push_back(linegraph);
			}
		}
		if (state == Graph::state::constant_volume)
		{
			LineGraph linegraph;
			VectorOfLines = std::vector<LineGraph>();

			for (float pressure = 0; pressure <= 5.27; pressure += 0.01) //Max value of pressure 6.27 - 1 = 5.27
			{
				//piston.AddHeat(heat, Piston::process::constant_pressure);
				linegraph.setcord((22.4) * 2 + 2, pressure * 50 + 50, start_pos_x, start_pos_y);
				VectorOfLines.push_back(linegraph);
			}


			for (float heat = 0; heat <= 30; heat += 0.01)
			{
				/*piston.AddHeat(heat, Piston::process::constant_pressure);*/
				linegraph.setcord_point((piston.returnVolume()) * 2, piston.pressure * 50, start_pos_x, start_pos_y);
				VectorOfPoints.push_back(linegraph);
			}
		}
	}
	
	void Draw(ImDrawList* draw_list, const ImU32 &Graph_col32)
	{
		ImU32 Text_col32 = ImColor(Text_col);
		draw_list->AddLine(start_pos_x, End_pos_x, Graph_col32, thickness); //Draws X_Axis
		draw_list->AddLine(start_pos_y, End_pos_y, Graph_col32, thickness); //Draws Y_Axis
		draw_list->AddText(PressureText_pos, Text_col32, "P");
		draw_list->AddText(VolumeText_pos, Text_col32, "V");
	}

	void DrawLines(ImDrawList* draw_list, Piston &piston)
	{

		ImVec4 Text_col2 = ImVec4(0.2f, 0.902f, 0.98f, 0.3f);
		ImU32 Text_col32 = ImColor(Text_col);
		ImU32 Text_col32_2 = ImColor(Text_col2);


		for (auto &linegraphs : VectorOfLines)
		{
			draw_list->AddLine(linegraphs.start_cord, linegraphs.end_cord, Text_col32, 2);
		}
		for (auto& linegraphs : VectorOfPoints)
		{
			draw_list->AddLine(linegraphs.start_cord, linegraphs.end_cord, Text_col32_2, 5);
		}

	}
};

