#include "DrawShapes.h"


DrawShapes::DrawShapes(float x_posistion, float y_posistion, float Length, float Thickness, DrawShapes::shapes import_shape) : length(1), thickness(1), Arrow_Col(ImVec4(0.1f, 0.9f, 0.3f, 0.65f)), col32(ImColor(Arrow_Col)), text("NULL")
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
}


void DrawShapes::Draw(ImDrawList* draw_list)
{
	if (shape == ARROW)
	{
		draw_list->AddLine(start_position, end_position, col32, thickness);
		draw_list->AddTriangleFilled(ImVec2(x_pos + length, y_pos - 2 + thickness / 2), ImVec2(x_pos + length, y_pos + 2 + thickness / 2), ImVec2(x_pos + length + 6, y_pos + thickness / 2), ImColor(ImVec4(0.6f, 0.9f, 0.3f, 0.85f)));
	}
	if (shape == BOILER)
	{
		draw_list->AddRect(start_position, end_position, col32, thickness);

		draw_list->AddText(ImVec2(x_pos + length / 2 - 20, y_pos + length / 2), TextColor, text.c_str());
		
	}
}