#pragma once
#include <iostream>
#include <array>
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

enum class SteamTableFlag;
enum class SteamTableCalculate;




struct Phase
{
	std::string phase;
	ImColor color;
	Phase() {
		phase = "NULL";
	}
	Phase(std::string name, ImColor colored)
	{
		phase = name;
		color = colored;
	}
};




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

	Phase phase1;
	Phase phase2;
	Phase phase3;
	float feed1;
	float feed2;
	float feed3;
	float temperature1;
	float temperature2;
	float temperature_outlet;
	float pressure1;
	float pressure2;
	float pressure3;
	float enthalpy_feed1;
	float enthalpy_feed2;
	float enthalpy_outlet;
	float outlet_diameter;

	float Heating_Element = 0; //Heat
public:
	boiler<FEED::DOUBLE>();
	boiler<FEED::DOUBLE>(float FEED_1, float FEED_2, float Temperature1, float Temperature2, float Pressure1, float Pressure2, float Pressure3, float Outlet_Diameter,
		float Temperature_Outlet, std::unique_ptr<std::vector<std::array<std::string, 8>>>& SteamTable);
	void setShape(DrawShapes Boiler_Shape) { Boiler = Boiler_Shape; }
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
	float& ReturnFeed1() { return feed1; }
	float& ReturnFeed2() { return feed2; }
	float& ReturnFeed3() { return feed3; }
	float& ReturnTemp1() { return temperature1; }
	float& ReturnTemp2() { return temperature2; }
	float& ReturnTemp3() { return temperature_outlet; }
	float& ReturnEnthalpy1() { return enthalpy_feed1; }
	float& ReturnEnthalpy2() { return enthalpy_feed2; }
	float& ReturnEnthalpy3() { return enthalpy_outlet; }
	//std::array<std::string, 3>& ReturnPhases() { return feeds_phase; }

	enum class UPDATING
	{
		ENTHALPY = 0X01, HEAT = 0X02
	};
	template<size_t size>
	void Update(std::unique_ptr<std::vector<std::array<std::string, size>>>& SteamTables, float &pressure_feed, UPDATING update) {
		if (update == UPDATING::ENTHALPY)
		{
			enthalpy_feed1 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure1, temperature1, phase1.phase);
			enthalpy_feed2 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure2, temperature2, phase2.phase);
			enthalpy_outlet = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure3, temperature_outlet, phase3.phase);
			CalculateHeat(); CalculateFlowRate();
		}
		if (update == UPDATING::HEAT)
		{
			enthalpy_feed1 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure1, temperature1, phase1.phase);
			enthalpy_feed2 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure2, temperature2, phase2.phase);
			enthalpy_outlet = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure3, temperature_outlet, phase3.phase);
			CalculateHeat(); CalculateFlowRate();
		}
	}

	void CalculateHeat();
	void CalculateFlowRate();
	template<size_t array_size>
	void CalculateEnthalpy(std::vector<std::array<std::string, array_size>> &steam_table);

	std::string returnFeed1() { return "Flow Rate: " + _Format(feed1, 4) + " Kg/min"; }
	std::string returnFeed2() { return "Flow Rate: " + _Format(feed2, 4)+ " Kg/min"; }
	std::string returnFeed3() { return "Flow Rate: " + _Format(feed3, 4) + " Kg/min"; }
	std::string returnTemp1() { return "Temperature: " + _Format(temperature1, 4) + "C"; }
	std::string returnTemp2() { return "Temperature: " + _Format(temperature2, 4) + "C"; }
	std::string returnTemp3() { return "Temperature: " + _Format(temperature_outlet, 4) + "C"; }
	std::string returnPressure1() { return "Pressure: " + _Format(pressure1, 3) + " MPA"; }
	std::string returnPressure2() { return "Pressure: " + _Format(pressure2, 3) + " MPA"; }
	std::string returnEnthalpy1() { return "Enthalpy: " + _Format(enthalpy_feed1, 5) + " kJ/kg"; }
	std::string returnEnthalpy2() { return "Enthalpy: " + _Format(enthalpy_feed2, 5) + " kJ/kg"; }
	std::string returnEnthalpy3() { return "Enthalpy: " + _Format(enthalpy_outlet, 5) + " kJ/kg"; }
	std::string returnOutletPressure() { return "Pressure: " + _Format(pressure3, 4) + " MPA"; }
	std::string returnOutletDiameter() { return "Diameter: " + _Format(outlet_diameter, 4) + " inch"; }
	std::string returnHeatEvolved() { return "Heat: " + _Format(Heating_Element, 5) + " TJ"; }
	Phase returnPhase1();
	Phase returnPhase2();
	Phase returnPhase3();
};



