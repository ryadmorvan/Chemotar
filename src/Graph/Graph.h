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
#include "../src/Simulations/Piston.h"




class Graph
{
public:

	//Enum used to indicate which type of build will be performed
	enum state
	{
		constant_pressure = 0x01, constant_volume = 0x02, constant_temperature = 0x03, constant_heat = 0x04
	};

	float x;
	float y;
	float thickness;
	ImVec2 start_pos_x;
	ImVec2 end_pos_x;
	ImVec2 start_pos_y;
	ImVec2 end_pos_y;

	ImVec2 pressure_text_pos;
	ImVec2 volume_text_pos;
	bool constant_pressure_setup = 0;

	//Class within the Graph class
	//This class will be used to indicate the Infinitesimal small dots that will be combined all together to draw the graph
	class LineGraph
	{
	public:
		ImVec2 start_cord;
		ImVec2 end_cord;
		void setcord(float x, float y, ImVec2 start_pos_x, ImVec2 start_pos_y)
		{
			start_cord = ImVec2(start_pos_x.x + x, start_pos_y.y - y);
			end_cord = ImVec2(start_pos_x.x + x + 1, start_pos_y.y - y);
		}
		void setcord_point(float x, float y, ImVec2 start_pos_x, ImVec2 start_pos_y)
		{
			start_cord = ImVec2(start_pos_x.x + x, start_pos_y.y - y);
			end_cord = ImVec2(start_pos_x.x + x + 5, start_pos_y.y - y);
		}
	};



	ImVec4 Text_col = ImVec4(0.902f, 0.902f, 0.98f , 0.65f);
	//Vectors that will store the dots and be used to simulate the graph
	std::vector<LineGraph> VectorOfLines;
	std::vector<LineGraph> VectorOfPoints;


	Graph(double x_start_pos, double y_start_pos, double x_end_pos, double y_end_pos, double Thickness);



	//Builds all the lines on the graph
	void BuildLinesVector(Piston piston, Graph::state state);
	//Draw the actual graph
	void Draw(ImDrawList* draw_list, const ImU32& Graph_col32);
	//Draws the actual lines on top of the graph render
	void DrawLines(ImDrawList* draw_list, Piston& piston);
};

