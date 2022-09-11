#pragma once
#include "../src/Drawing/DrawShapes.h"



class Bernoulli;

template<typename T1, typename T2>
std::string _Format(T1 f, T2 n);

namespace Shapes
{
	//We inherit our DrawShipes class into our specialized class for our bernoulli simulation
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
		//We first initalize the DrawShapes class then we pass the parameters of our shapes
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
		//We pass our ImDrawList so it can be used to draw the shapes
		//outlet_multiplier is variable that scales our outlet diameter in terms of diameter multiplier
		//bern is our object which will be used to retrieve the data of our current conditions and draw it to the user
		void Draw(ImDrawList* draw_list, float &outlet_multiplier, Bernoulli bern);
		ImU32 returnColor() { return col32; }
		ImU32 returnColorText() { return TextColor; }
		float& ReturnInlet() {return Inlet_Diameter;}
		float& ReturnOutlet() {return Outlet_Diameter;}
		float& ReturnHeight() {return height;}

	};
}


//our Bernoulli class which will store our process conditions and also the Bernoulli_Simulation function
class Bernoulli
{
private:
	Shapes::Pipes _pipes; //Shapes object
	float inlet_diameter = 20;
	float outlet_diameter = 40;
	float height = 25;

public:
	Bernoulli() : _pipes()
	{
		
	}

	void Draw(ImDrawList* draw_list); //will be used to draw our shapes
	void setPipeDraw(float height, float diameter); //function that sets our _pipes paramters
	static void Bernoulli_Simulation(bool* p_open); //Static function that will bind to our Chemotar program as a function pointer

	float* returnOutlet() {return &outlet_diameter;}
	float* returnHeight() {return &height;}
	std::string returnHeightString() const {return _Format(height, 3) + " m";}
	std::string returnInletDiameterString() const {return "Diameter: " + _Format(inlet_diameter, 4) + " cm";}
	std::string returnOutletDiameterString() const {return "Diameter: " + _Format(outlet_diameter, 4) + " cm";}
};

