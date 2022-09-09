#pragma once
#include "../src/Drawing/DrawShapes.h"


class Bernoulli;

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
			Position.x = 100; Position.y = 550; length = 150;
			height = 150;
			PipeColor = ImColor(106, 121, 92, 200);
		}
		Pipes(float Height, float Diameter) : DrawShapes()
		{
			diameter = Diameter;
			Position.x = 250; Position.y = 550; length = 150;
			height = Height;
			PipeColor = ImColor(106, 121, 92, 200);
		}
		
		void Draw(ImDrawList* draw_list, float &outlet_multiplier, Bernoulli bern);
		ImU32 returnColor() { return col32; }
		ImU32 returnColorText() { return TextColor; }
		float& ReturnInlet() {return Inlet_Diameter;}
		float& ReturnOutlet() {return Outlet_Diameter;}
		float& ReturnHeight() {return height;}

	};
}



class Bernoulli
{
private:
	Shapes::Pipes _pipes;
	float inlet_diameter = 20;
	float outlet_diameter = 40;

public:
	Bernoulli() : _pipes()
	{
		
	}

	void Draw(ImDrawList* draw_list);
	void setPipeDraw(float height, float diameter);
	static void Bernoulli_Simulation(bool* p_open);

	float* returnOutlet() {return &outlet_diameter;}
};

