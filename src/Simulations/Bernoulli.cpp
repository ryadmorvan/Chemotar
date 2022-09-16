#include "Bernoulli.h"


void Shapes::Pipes::Draw(ImDrawList* draw_list, float &outlet_multiplier, Bernoulli bern)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	//We will draw our pipes
	draw_list->AddRectFilled(ImVec2(p.x + Position.x, p.y + Position.y), ImVec2(p.x + Position.x + length,p.y + Position.y + diameter), PipeColor, 0);
	draw_list->AddRectFilled(ImVec2(p.x + Position.x + length - diameter, p.y + Position.y), ImVec2(p.x + Position.x + length, p.y + Position.y - height), PipeColor, 0);
	draw_list->AddRectFilled(ImVec2(p.x + Position.x + length - diameter, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2,p.y + Position.y - height + diameter), PipeColor, 0);

	float out_let_diameter = diameter * outlet_multiplier; //will be used to scale our outlet diameter
	draw_list->AddLine(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height - out_let_diameter), PipeColor, 0);
	draw_list->AddLine(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height + diameter), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height + out_let_diameter + diameter), PipeColor, 0);
	//Draw filled into the shape of diverging canal
	draw_list->AddRectFilled(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height + diameter), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.20f)));
	draw_list->AddTriangleFilled(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height - out_let_diameter), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.20f)));
	draw_list->AddTriangleFilled(ImVec2(p.x + Position.x + length*2, p.y + Position.y - height + diameter -1.5), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height + diameter -1.5), ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height + diameter + out_let_diameter), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 0.20f)));

	draw_list->AddRectFilled(ImVec2(p.x + Position.x + length*2 + out_let_diameter, p.y + Position.y - height - out_let_diameter),ImVec2(p.x + Position.x + length*2 + out_let_diameter + 80, p.y + Position.y - height + out_let_diameter + diameter), PipeColor, 0 );

	//Draw vertical arrows to indicate height
	draw_list->AddTriangleFilled(ImVec2(p.x + Position.x + length + 7.5, p.y + Position.y + diameter - height + 10), ImVec2(p.x + Position.x + length + 12.5, p.y + Position.y + diameter - height + 10), ImVec2(p.x + Position.x + length + 10, p.y + Position.y + diameter - height + 5), ImColor(ImVec4(0.6f, 0.9f, 0.3f, 0.85f)) );
	draw_list->AddLine(ImVec2(p.x + Position.x + length + 10, p.y + Position.y + diameter - 6), ImVec2(p.x + Position.x + length + 10, p.y + Position.y - height + diameter + 5), ImColor(0, 255, 0, 255), 0);
	draw_list->AddText(ImVec2(p.x + Position.x + length + 15, p.y + Position.y + diameter - height*0.5 - 6), ImColor(0, 255, 0, 255), bern.returnHeightString().c_str()); //Will display the current height to the user
	draw_list->AddTriangleFilled(ImVec2(p.x + Position.x + length + 6, p.y + Position.y + diameter - 5), ImVec2(p.x + Position.x + length + 14, p.y + Position.y + diameter - 5), ImVec2(p.x + Position.x + length + 10, p.y + Position.y + diameter), ImColor(ImVec4(0.6f, 0.9f, 0.3f, 0.85f)) );

	//we draw the inlet/outlet arrows
	DrawShapes arrow1 = DrawShapes(p.x + Position.x - 190.0f, p.y + Position.y + diameter/2.0f, 170.0f,  2.0f, DrawShapes::ARROW);
	DrawShapes arrow2 = DrawShapes(p.x + Position.x + length*2 + out_let_diameter + 90, p.y + Position.y - height + diameter/2.0f, 170.0f, 2.0f, DrawShapes::ARROW);
	//it will display to our user the current inlet and outlet diameters and other information
	draw_list->AddText(ImVec2(p.x + Position.x + length*2 + out_let_diameter + 95, p.y + Position.y - height + diameter/2.0f - 20), ImColor(0, 255, 0, 255), bern.returnOutletDiameterString().c_str());
	draw_list->AddText(ImVec2(p.x + Position.x + length*2 + out_let_diameter + 95, p.y + Position.y - height + diameter/2.0f - 40), ImColor(0, 255, 0, 255), bern.returnPressure2String().c_str());
	draw_list->AddText(ImVec2(p.x + Position.x - 190.0f, p.y + Position.y + diameter/2.0f - 40), ImColor(0, 255, 0, 255), bern.returnPressure1String().c_str());
	draw_list->AddText(ImVec2(p.x + Position.x - 190.0f, p.y + Position.y + diameter/2.0f - 20), ImColor(0, 255, 0, 255), bern.returnInletDiameterString().c_str());

	//Draw the velocities
	draw_list->AddText(ImVec2(p.x + Position.x - 190.0f, p.y + Position.y + diameter/2.0f - 60), ImColor(0, 255, 0, 255), bern.returnVelocity1String().c_str());
	draw_list->AddText(ImVec2(p.x + Position.x + length*2 + out_let_diameter + 95, p.y + Position.y - height + diameter/2.0f - 60), ImColor(0, 255, 0, 255), bern.returnVelocity2String().c_str());

	//Draw the volumetric and mass flow rates
	if(*bern.returnKineticProfile() == true)
		{
		
			draw_list->AddText(ImVec2(p.x + Position.x - 190.0f, p.y + Position.y + diameter/2.0f + 10), ImColor(180, 230, 50, 255), bern.returnKineticEnergy1String().c_str());
			draw_list->AddText(ImVec2(p.x + Position.x - 190.0f, p.y + Position.y + diameter/2.0f + 30), ImColor(180, 230, 50, 255), bern.returnPressureEnergy1String().c_str());
			draw_list->AddText(ImVec2(p.x + Position.x + length*2 + out_let_diameter + 95, p.y + Position.y - height + diameter/2.0f + 10), ImColor(180, 230, 50, 255), bern.returnKineticEnergy2String().c_str());
			draw_list->AddText(ImVec2(p.x + Position.x + length*2 + out_let_diameter + 95, p.y + Position.y - height + diameter/2.0f + 30), ImColor(180, 230, 50, 255), bern.returnPressureEnergy2String().c_str());
		}

	draw_list->AddText(ImVec2(p.x +10, p.y + 35), ImColor(100, 160, 100, 240), "General Condition");
	draw_list->AddText(ImVec2(p.x + 10, p.y + 55), ImColor(0, 255, 0, 255), bern.returnFlowRate1String().c_str());
	draw_list->AddText(ImVec2(p.x + 10, p.y + 75), ImColor(0, 255, 0, 255), bern.returnMassFlowRate1String().c_str());
	draw_list->AddText(ImVec2(p.x + 10, p.y + 95), ImColor(0, 255, 0, 255), bern.returnPotentialEnergy().c_str());

	arrow1.Draw(draw_list);
	arrow2.Draw(draw_list);

}






void Bernoulli::Draw(ImDrawList* draw_list)
{
	float outlet_size = outlet_diameter_/(inlet_diameter_ * 3.0f); //Scales with respect to our outlet and inlet ratio
	_pipes.Draw(draw_list, outlet_size, *this);

}

//sets our _pipes paramters
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
	bern.setPipeDraw(bern.height_, 20); //Height in meters
	static ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65);
	ImGui::TextColored(ImColor(170, 80, 90, 200), "Outlet Diameter");
	if(ImGui::SliderFloat(" ", bern.returnOutlet(), 20.0f, 110.0f, "%.2f cm", ImGuiSliderFlags_AlwaysClamp))
	{
		bern.CalculateVelocity();
		bern.CalculateFlowRateIn();
		bern.CalculateMassFlowRateIn();
		bern.CalculateFlowRateOut();
		bern.CalculateMassFlowRateOut();
		bern.CalculateKineticEnergy1();
		bern.CalculateKineticEnergy2();
	}
	ImGui::TextColored(ImColor(130, 80, 80), "Inlet Pressure");
	if(ImGui::SliderFloat("   ", bern.return_pressure1(), 100.0f, 500.0f, "%.2f kpa", ImGuiSliderFlags_AlwaysClamp))
	{
		bern.CalculatePressure();
	}
	ImGui::TextColored(ImColor(110, 190, 100, 220), "Inlet Velocity");
	if(ImGui::SliderFloat("    ", bern.return_velocity1(), 2.0f, 15.0f, "%.2f m/s", ImGuiSliderFlags_AlwaysClamp))
	{
		bern.CalculateVelocity();
		bern.CalculateFlowRateIn();
		bern.CalculateMassFlowRateIn();
		bern.CalculateFlowRateOut();
		bern.CalculateMassFlowRateOut();
		bern.CalculateKineticEnergy1();
		bern.CalculateKineticEnergy2();
	}
	ImGui::TextColored(ImColor(80, 170, 90, 200), "Height");
	if(ImGui::SliderFloat("  ", bern.returnHeight(), 50.0f, 300.0f, "%.2f cm", ImGuiSliderFlags_AlwaysClamp))
	{
		bern.CalculatePressure();
		bern.CalculatePotentialEnergy();
	}
	bern.Draw(draw_list);
	ImGui::Checkbox("Enable Kinetic Profile", bern.returnKineticProfile());

}

void Bernoulli::CalculatePressure()
{
	pressure2_ = ((fluid_density_*pow(velocity1_, 2)/2.0f) - fluid_density_*Gravity*(height_/100.0f) + pressure1_*pow(10, 3) - (fluid_density_*pow(velocity2_, 2)/2.0f))/1000.0f;
}
