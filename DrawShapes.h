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









class DrawShapes
{
private:
	ImVec2 start_position;
	ImVec2 end_position;
	float length;
	float thickness;
	float x_pos;
	float y_pos;
	ImVec4 Arrow_Col;
	ImU32 col32;
public:
	enum shapes
	{
		ARROW = 0x01, BOILER = 0x02, TURBINE = 0x03, 
	};
	shapes shape;
	

	DrawShapes(float x_posistion, float y_posistion, float Length, float Thickness, DrawShapes::shapes import_shape)
	{
		x_pos = x_posistion;
		y_pos = y_posistion;

		if (import_shape == DrawShapes::ARROW)
		{
			length = Length;
			thickness = Thickness;
			shape = import_shape;
			start_position = ImVec2(x_pos, y_pos);
			end_position = ImVec2(x_pos + length, y_pos);
			Arrow_Col = ImVec4(0.1f, 0.9f, 0.3f, 0.65f);

			col32 = ImColor(Arrow_Col);
		}
	}


	void Draw(ImDrawList* draw_list)
	{
		if (shape == ARROW)
		{
			draw_list->AddLine(start_position, end_position, col32, thickness);
			draw_list->AddTriangleFilled(ImVec2(x_pos + length, y_pos - 2 + thickness/2 ), ImVec2(x_pos + length, y_pos + 2 + thickness/2), ImVec2(x_pos + length + 6, y_pos + thickness/2), ImColor(ImVec4(0.6f, 0.9f, 0.3f, 0.85f)));
		}
	}


};

