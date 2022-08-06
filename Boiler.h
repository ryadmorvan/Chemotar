#pragma once
#include <iostream>
#include "DrawShapes.h"

namespace FEED
{
	struct SINGLE;
	struct DOUBLE;
}

//Declare in this header file
template<typename T>
int _Round(T number);
template<typename T1, typename T2>
std::string _Format(T1 f, T2 n);


template<typename T> // primary template
class boiler : std::false_type {};

template<>           // explicit specialization for T = void
class boiler<FEED::SINGLE>
{
private:
	DrawShapes Boiler;
	float feed;
	float temperature_inlet;
	float temperature_outlet;
	float enthalpy_feed;
	float enthalpy_outlet;
	float outlet_diameter;
public:
	boiler<FEED::SINGLE>();
};

template<>
class boiler<FEED::DOUBLE> 
{
private:
	DrawShapes Boiler;
	//Temperature in Kelvin
	//Enthalpy in kJ/kg
	//Feed in Kg/min
	//Heating in kJ/min
	//Diameter in inches
	std::string outlet_state = "Saturated";
	float feed1;
	float feed2;
	float feed3;
	float temperature1;
	float temperature2;
	float enthalpy_feed1;
	float enthalpy_feed2;
	float enthalpy_outlet;
	float outlet_diameter;
	float temperature_outlet;

	bool Saturated = false;


	float pressure = 1; //In bar

	float Heating_Element = 0; //Heat
public:
	boiler<FEED::DOUBLE>();
	boiler<FEED::DOUBLE>(float FEED_1, float FEED_2, float FEED_3, float Temperature1, float Temperature2,
		float Enthalpy_Feed1, float Enthalpy_Feed2, float Enthalpy_Outlet, float Outlet_Diameter,
		float Temperature_Outlet, bool saturation);
	void setShape(DrawShapes Boiler_Shape) { Boiler = Boiler_Shape; }
	void setPressure(float Pressure) { pressure = Pressure; }
	void Draw(ImDrawList *draw_list)
	{
		Boiler.Draw(draw_list);
	}
	void DrawInfo(ImDrawList* draw_list, DrawShapes &Arrow_In1, DrawShapes& Arrow_In2, DrawShapes& Arrow_Out1);
	void Info()
	{
		std::cout << "Feed1: " << feed1 << std::endl << "Feed2: " << feed2
			<< std::endl << "Temperature1 = " << temperature1 << std::endl << "Temperature2 = "
			<< temperature2 << std::endl;
	}

	std::string returnFeed1() { return "Feed Rate: " + _Format(feed1, 4) + " Kg/min"; }
	std::string returnFeed2() { return "Feed Rate: " + _Format(feed2, 4)+ " Kg/min"; }
	std::string returnFeed3() { return "Feed Rate: " + _Format(feed3, 4) + " Kg/min"; }
	std::string returnTemp1() { return "Temperature: " + _Format(temperature1, 4) + "K"; }
	std::string returnTemp2() { return "Temperature: " + _Format(temperature2, 4) + "K"; }
	std::string returnTemp3() { return "Temperature: " + _Format(temperature_outlet, 4) + "K"; }
	std::string returnEnthalpy1() { return "Enthalpy: " + _Format(enthalpy_feed1, 4) + " kJ/kg"; }
	std::string returnEnthalpy2() { return "Enthalpy: " + _Format(enthalpy_feed2, 4) + " kJ/kg"; }
	std::string returnEnthalpy3() { return "Enthalpy: " + _Format(enthalpy_outlet, 4) + " kJ/kg"; }
	std::string returnOutletPressure() { return "Pressure: " + _Format(pressure, 4) + " bars"; }
	std::string returnOutletDiameter() { return "Diameter: " + _Format(outlet_diameter, 4) + " inch"; }

};



