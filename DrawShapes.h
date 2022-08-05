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
	std::string text;
	ImVec2 start_position;
	ImVec2 end_position;
	float length;
	float thickness;
	float x_pos;
	float y_pos;
	ImU32 col32;
	ImU32 TextColor;
public:
	enum shapes
	{
		ARROW = 0x01, BOILER = 0x02, TURBINE = 0x03, RESISTOR = 0x04
	};
	shapes shape;
	

	DrawShapes(float x_posistion, float y_posistion, float Length, float Thickness, DrawShapes::shapes import_shape);
	void TurbineResize(float size) { length = size; }

	void Draw(ImDrawList* draw_list);


};

