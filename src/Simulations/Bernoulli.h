#pragma once
#include "../src/Drawing/DrawShapes.h"



class Bernoulli
{
private:
	Shapes::Pipes _pipes;
public:
	Bernoulli() : _pipes()
	{
		
	}
	void Draw(ImDrawList* draw_list);
	static void Bernoulli_Simulation(bool* p_open);

};

