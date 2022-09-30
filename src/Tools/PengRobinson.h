#pragma once
#include <imgui.h>
#include <math.h>


class PengRobinson
{
private:

public:
	static void PengRobinsonCalculator(bool* p_open);
	float KappaCalculation(float AcentricFactor) {0.37464 + 1.54226*AcentricFactor - 0.26992*AcentricFactor*AcentricFactor;}
	float Alpha(float temperature, float temperatureCrit, float AcentricFactor) {return powf(1+ KappaCalculation(AcentricFactor)*(1-powf(temperature/temperatureCrit, 0.5)), 2);}
};

