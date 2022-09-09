#include "Bernoulli.h"


void Shapes::Pipes::Draw(ImDrawList* draw_list, float &outlet_multiplier, Bernoulli bern)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	draw_list->AddRectFilled(ImVec2(p.x + Position.x, p.y + Position.y), ImVec2(p.x + Position.x + length,p.y + Position.y + diameter), PipeColor, 0);
	draw_list->AddRectFilled(ImVec2(p.x + Position.x + length - diameter, p.y + Position.y), ImVec2(p.x + Position.x + length, p.y + Position.y - height), PipeColor, 0);
	draw_list->AddRectFilled(ImVec2(p.x + Position.x + length - diameter, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2,p.y + Position.y - height + diameter), PipeColor, 0);

	float out_let_diameter = diameter * outlet_multiplier;
	draw_list->AddLine(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height - out_let_diameter), PipeColor, 0);
	draw_list->AddLine(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height + diameter), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height + out_let_diameter + diameter), PipeColor, 0);
	//Draw filled into the shape
	draw_list->AddRectFilled(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height + diameter), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.20f)));



	draw_list->AddTriangleFilled(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height - out_let_diameter), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.20f)));
	draw_list->AddTriangleFilled(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height + diameter -1.5), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height + diameter -1.5), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height + diameter + out_let_diameter), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.20f)));

	draw_list->AddRectFilled(ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height - out_let_diameter),ImVec2(p.x + Position.x + length*2 + out_let_diameter + 80, p.y + Position.y - height + out_let_diameter + diameter), PipeColor, 0 );

	//Draw vertical arrows
	draw_list->AddTriangleFilled(ImVec2(p.x + Position.x + length + 7.5, p.y + Position.y + diameter - height + 10), ImVec2(p.x + Position.x + length + 12.5, p.y + Position.y + diameter - height + 10), ImVec2(p.x + Position.x + length + 10, p.y + Position.y + diameter - height + 5), ImColor(ImVec4(0.6f, 0.9f, 0.3f, 0.85f)) );
	draw_list->AddLine(ImVec2(p.x + Position.x + length + 10, p.y + Position.y + diameter - 6), ImVec2(p.x + Position.x + length + 10, p.y + Position.y - height + diameter + 5), ImColor(0, 255, 0, 255), 0);



	draw_list->AddText(ImVec2(p.x + Position.x + length + 15, p.y + Position.y + diameter - height*0.5 - 6), ImColor(0, 255, 0, 255), bern.returnHeightString().c_str());
	draw_list->AddTriangleFilled(ImVec2(p.x + Position.x + length + 6, p.y + Position.y + diameter - 5), ImVec2(p.x + Position.x + length + 14, p.y + Position.y + diameter - 5), ImVec2(p.x + Position.x + length + 10, p.y + Position.y + diameter), ImColor(ImVec4(0.6f, 0.9f, 0.3f, 0.85f)) );

	DrawShapes arrow1 = DrawShapes(p.x + Position.x - 120.0f, p.y + Position.y + diameter/2.0f, 100.0f,  2.0f, DrawShapes::ARROW);
	DrawShapes arrow2 = DrawShapes(p.x + Position.x + length*2 + out_let_diameter + 90, p.y + Position.y - height + diameter/2.0f, 150.0f, 2.0f, DrawShapes::ARROW);

	draw_list->AddText(ImVec2(p.x + Position.x + length*2 + out_let_diameter + 95, p.y + Position.y - height + diameter/2.0f - 20), ImColor(0, 255, 0, 255), bern.returnOutletDiameterString().c_str());
	arrow1.Draw(draw_list);
	arrow2.Draw(draw_list);

}






void Bernoulli::Draw(ImDrawList* draw_list)
{
	float outlet_size = outlet_diameter/(inlet_diameter * 3.0f);
	_pipes.Draw(draw_list, outlet_size, *this);

}

void Bernoulli::setPipeDraw(float height, float diameter)
{
	_pipes = Shapes::Pipes(height, diameter);
}

void Bernoulli::Bernoulli_Simulation(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Bernoulli Simulation", p_open))
	{
		return;
	}
	static Bernoulli bern;
	bern.setPipeDraw(bern.height*10.0f, 20); //Height in meters
	static ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImGui::TextColored(ImColor(170, 80, 90, 200), "Outlet Diameter");
	if(ImGui::SliderFloat("Outlet Diameter", bern.returnOutlet(), 35.0f, 110.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
	{
		
	}
	ImGui::TextColored(ImColor(80, 170, 90, 200), "Height");
	if(ImGui::SliderFloat("Height", bern.returnHeight(), 5.0f, 35.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
	{
	}

	bern.Draw(draw_list);

}
