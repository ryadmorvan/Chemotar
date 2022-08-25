#include "DrawShapes.h"


DrawShapes::DrawShapes(float x_posistion, float y_posistion, float Length, float Thickness, DrawShapes::shapes import_shape) : length(1), thickness(1), col32(ImColor(ImVec4(0.1f, 0.9f, 0.3f, 0.65f))), text("NULL"), TextColor(ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.8f)))
{
	x_pos = x_posistion;
	y_pos = y_posistion;
	//Construct and draw the units that will be used to render them
	if (import_shape == DrawShapes::ARROW)
	{
		length = Length;
		thickness = Thickness;
		shape = import_shape;
		start_position = ImVec2(x_pos, y_pos);
		end_position = ImVec2(x_pos + length, y_pos);
		col32 = ImColor(ImVec4(0.1f, 0.9f, 0.3f, 0.65f));
	}

	if (import_shape == DrawShapes::BOILER)
	{
		text = "Boiler";
		TextColor = ImColor(255, 100, 100, 200);
		length = Length;
		thickness = Thickness;
		shape = import_shape;
		start_position = ImVec2(x_pos, y_pos);
		end_position = ImVec2(x_pos + length, y_pos + length);
		col32 = ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.8f));
	}
	if (import_shape == DrawShapes::TURBINE)
	{
		text = "Steam Turbine";
		TextColor = ImColor(255, 100, 100, 200);
		length = Length;
		thickness = Thickness;
		shape = import_shape;
		x_pos = x_posistion;
		y_pos = y_posistion;
		col32 = ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.8f));
	}
	if (import_shape == DrawShapes::RESISTOR)
	{
		text = "Heating Element";
		length = Length;
		thickness = Thickness;
		shape = import_shape;
		x_pos = x_posistion;
		y_pos = y_posistion;
		col32 = ImColor(96, 95, 99, 250);
	}
}








void DrawShapes::Draw(ImDrawList* draw_list)
{
	//Draw the units depending on the shapes enum
	if (shape == shapes::ARROW)
	{
		draw_list->AddLine(start_position, end_position, col32, thickness);
		draw_list->AddTriangleFilled(ImVec2(x_pos + length, y_pos - 2 + thickness / 2), ImVec2(x_pos + length, y_pos + 2 + thickness / 2), ImVec2(x_pos + length + 6, y_pos + thickness / 2), ImColor(ImVec4(0.6f, 0.9f, 0.3f, 0.85f)));
	}
	if (shape == shapes::BOILER)
	{
		draw_list->AddRect(start_position, end_position, col32, thickness);
		draw_list->AddRectFilled(ImVec2(x_pos + thickness - 3, y_pos + thickness - 3), ImVec2(x_pos + length - thickness + 2, y_pos + length - thickness + 2), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.15f)));
		draw_list->AddText(ImVec2(x_pos + length / 2 - 20, y_pos + length / 2), TextColor, text.c_str());
		
	}
	if (shape == shapes::TURBINE)
	{
		draw_list->AddLine(ImVec2(x_pos + length, y_pos), ImVec2(x_pos + length, y_pos + length), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos, y_pos + length *1/4), ImVec2(x_pos, y_pos + length * 3/4), col32, thickness);


		draw_list->AddLine(ImVec2(x_pos, y_pos + length * 1 / 4), ImVec2(x_pos + length, y_pos), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos, y_pos + length * 3 / 4), ImVec2(x_pos + length, y_pos + length), col32, thickness);


		//Filled Rect and Triangle
		draw_list->AddRectFilled(ImVec2(x_pos, y_pos + length * 1 / 4), ImVec2(x_pos + length, y_pos + length * 3 / 4), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.15f)));
		draw_list->AddTriangleFilled(ImVec2(x_pos, y_pos + length*1/4 + 0.5), ImVec2(x_pos + length, y_pos + thickness -1), ImVec2(x_pos + length, y_pos + length * 1 / 4 + thickness -1.5), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.15f)));
		draw_list->AddTriangleFilled(ImVec2(x_pos, y_pos + length * 3 / 4 + 0.5), ImVec2(x_pos + length, y_pos + length*3/4 + thickness - 1.5), ImVec2(x_pos + length, y_pos + length + thickness - 1.5), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.15f)));
		draw_list->AddText(ImVec2(x_pos + length / 2 - 50, y_pos + length / 2 - 8), TextColor, text.c_str());
	}
	if (shape == shapes::RESISTOR)
	{
		draw_list->AddLine(ImVec2(x_pos, y_pos), ImVec2(x_pos, y_pos + length * 3), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos, y_pos), ImVec2(x_pos + length*2/3, y_pos + length), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos + length * 2 / 3, y_pos + length), ImVec2(x_pos+ length*7/6, y_pos), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos + length * 7 / 6, y_pos), ImVec2(x_pos + length*10/6, y_pos + length), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos + length * 10 / 6, y_pos + length), ImVec2(x_pos + length * 13 / 6, y_pos), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos + length * 13 / 6, y_pos), ImVec2(x_pos + length * 16 / 6, y_pos + length), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos + length * 16 / 6, y_pos + length), ImVec2(x_pos + length * 19 / 6, y_pos), col32, thickness);
		draw_list->AddLine(ImVec2(x_pos + length*19/6, y_pos), ImVec2(x_pos + length * 19 / 6, y_pos + length * 3), col32, thickness);
		draw_list->AddText(ImVec2(x_pos + length * 19 /12 - 47, y_pos - 20), TextColor, text.c_str());
		
	}
}