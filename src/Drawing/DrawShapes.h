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
protected:
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
	//Flags that will be used to dictate which shape to use
	enum shapes
	{
		NON = 0x00, ARROW = 0x01, BOILER = 0x02, TURBINE = 0x03, RESISTOR = 0x04
	};
	shapes shape;
	

	DrawShapes(float x_posistion, float y_posistion, float Length, float Thickness, DrawShapes::shapes import_shape);

	DrawShapes() : x_pos(0), y_pos(0), length(1), thickness(1),
		col32(ImColor(0, 255, 0, 255)), TextColor(ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.8f))), text("NULL"), shape(shapes::NON),
		start_position(ImVec2(x_pos, y_pos)), end_position(ImVec2(x_pos + 10, y_pos + 10))
	{

	}

	void TurbineResize(float size) { length = size; }

	void Draw(ImDrawList* draw_list);

	virtual float returnX() { return x_pos; }
	virtual float returnY() { return y_pos; }
	virtual float returnLength() { return length; }
	ImU32 returnColor() { return col32; }
	ImU32 returnColorText() { return TextColor; }

};

namespace Shapes
{
	class Pipes : public DrawShapes
	{
	private:
		float Inlet_Diameter;
		float Outlet_Diameter;
		float height;
		float Shape_Length;
		ImU32 PipeColor;
		ImVec2 Position;
		float length;
		float diameter;
	public:
		Pipes() : DrawShapes()
		{
			diameter = 10;
			Position.x = 300; Position.y = 550; length = 150;
			height = 50;
			PipeColor = ImColor(76, 91, 92, 200);
		}

		void Draw(ImDrawList* draw_list);
		ImU32 returnColor() { return col32; }
		ImU32 returnColorText() { return TextColor; }
		float& ReturnInlet() {return Inlet_Diameter;}
		float& ReturnOutlet() {return Outlet_Diameter;}
		float& ReturnHeight() {return height;}

	};
}

