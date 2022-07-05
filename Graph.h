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
		constant_pressure = 0x01, constant_volume = 0x02, constant_temperature = 0x03, constant_heat = 0x04
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


	Graph(double x_start_pos, double y_start_pos, double x_end_pos, double y_end_pos, double Thickness);




	void BuildLinesVector(Piston piston, Graph::state state);
	
	void Draw(ImDrawList* draw_list, const ImU32& Graph_col32);

	void DrawLines(ImDrawList* draw_list, Piston& piston);
};

