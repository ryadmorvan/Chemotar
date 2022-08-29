#pragma once
#include <iostream>
#include <array>
#include <math.h>
#include "../src/Drawing/DrawShapes.h"


#define _USE_MATH_DEFINES

# define M_PI           3.14159265358979323846  /* pi */


//flags that will be used for specializing the boiler's class template
namespace FEED
{
	struct SINGLE;
	struct DOUBLE;
}

//Declare in this header file
template<typename T>
int round(T number);
template<typename T1, typename T2>
std::string format(T1 f, T2 n);

enum class SteamTableFlag;
enum class CompressedSuperheatedTablesFlags;



//Used to store the phase information of each inlet and outlet
struct Phase
{
	std::string phase;
	float quality = 0;
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
//Used to store the information of kinetic energy of each inlet and outlet
struct KineticEnergy
{
	float velocity;
	float kinetic;
	float volumetric_flow_rate;
	float diameter = 10;
	float area;

	static void set_diameter(float diameter)
	{
		diameter = diameter;
	}
	void find_area()
	{
		area = M_PI * pow((diameter / 2)/100, 2);
	}
	void find_velocity()
	{
		find_area();
		velocity = volumetric_flow_rate / area;
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
class boiler<FEED::DOUBLE>  //Double inlet implementation
{
private:
	DrawShapes _Boiler;
	//Temperature in Kelvin
	//Enthalpy in kJ/kg
	//Feed in Kg/min
	//Heating in kJ/min
	//Diameter in inches
	std::string outlet_state = "Saturated";
	bool VelocityProfile = false;

	//Phases information
	Phase _phase1;
	Phase _phase2;
	Phase _phase3;
	//Kinetic Information
	KineticEnergy _kinetic1;
	KineticEnergy _kinetic2;
	KineticEnergy _kinetic3;
public:
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

	float Heating_Element = 0; //Heat evolved
	friend struct KineticEnergy;

	boiler<FEED::DOUBLE>();

	boiler<FEED::DOUBLE>(float FEED_1, float FEED_2, float Temperature1, float Temperature2, float Pressure1, float Pressure2, float Pressure3,
		float Temperature_Outlet, std::unique_ptr<std::vector<std::array<std::string, 8>>>& SteamTable);

	//sets the shape that will be used to draw the boiler's shape
	void setShape(DrawShapes Boiler_Shape) { _Boiler = Boiler_Shape; }
	void Draw(ImDrawList *draw_list)
	{
		_Boiler.Draw(draw_list);
	}

	//Draws the states of all inlets and outlets using the arrows positions
	void DrawInfo(ImDrawList* draw_list, DrawShapes &Arrow_In1, DrawShapes& Arrow_In2, DrawShapes& Arrow_Out1);
	void Info()
	{
		std::cout << "Feed1: " << feed1 << std::endl << "Feed2: " << feed2
			<< std::endl << "Temperature1 = " << temperature1 << std::endl << "Temperature2 = "
			<< temperature2 << std::endl;
	}

	float& ReturnFeedRef1() { return feed1; }
	float& ReturnFeedRef2() { return feed2; }
	float& ReturnFeedRef3() { return feed3; }
	float& ReturnTempRef1() { return temperature1; }
	float& ReturnTempRef2() { return temperature2; }
	float& ReturnTempRef3() { return temperature_outlet; }
	float& ReturnEnthalpyRef1() { return enthalpy_feed1; }
	float& ReturnEnthalpyRef2() { return enthalpy_feed2; }
	float& ReturnEnthalpyRef3() { return enthalpy_outlet; }
	float& ReturnPressureRef1() { return pressure1; }
	float& ReturnPressureRef2() { return pressure2; }
	float& ReturnPressureRef3() { return pressure3; }

	float ReturnDiameter1() { return _kinetic1.diameter; }
	float ReturnDiameter2() { return _kinetic2.diameter; }
	float ReturnDiameter3() { return _kinetic3.diameter; }
	float& ReturnDiameter1Ref() { return _kinetic1.diameter; }
	float& ReturnDiameter2Ref() { return _kinetic2.diameter; }
	float& ReturnDiameter3Ref() { return _kinetic3.diameter; }

	std::string ReturnPressureString1() { return format(pressure1, 3); }
	std::string ReturnPressureString2() { return format(pressure2, 3); }
	std::string ReturnPressureString3() { return format(pressure3, 3); }
	//std::array<std::string, 3>& ReturnPhases() { return feeds_phase; }
	//enum to indicate which values will be updated real time
	enum class UPDATING
	{
		ENTHALPY = 0X01, HEAT = 0X02, TEMPERATURE = 0x03, NORMAL = 0x10
	};

	//functions that updates the objects real time values depending on multiple variables by calling the CalculateFromSteamTable functions and passing the appropriate flags
	template<size_t size>
	void Update(std::unique_ptr<std::vector<std::array<std::string, size>>>& SteamTables, UPDATING update) {
		if (update == UPDATING::ENTHALPY)
		{
			enthalpy_feed1 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::ENTHALPY, pressure1, temperature1, enthalpy_feed1, _phase1);
			enthalpy_feed2 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::ENTHALPY, pressure2, temperature2, enthalpy_feed2, _phase2);
			enthalpy_outlet = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::ENTHALPY, pressure3, temperature_outlet, enthalpy_outlet, _phase3);
			CalculateHeat(); CalculateFlowRate();
		}
		if (update == UPDATING::TEMPERATURE)
		{
			temperature1 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::TEMPERATURE, pressure1, temperature1, enthalpy_feed1, _phase1);
			temperature2 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::TEMPERATURE, pressure2, temperature2, enthalpy_feed2 ,_phase2);
			temperature_outlet = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::TEMPERATURE, pressure3, temperature_outlet, enthalpy_outlet,  _phase3);
			CalculateHeat(); CalculateFlowRate();
		}
		if(update == UPDATING::NORMAL)
		{
			CalculateHeat(); CalculateFlowRate();
		}
	}

	void EnableVelocityProfile() { VelocityProfile = true; }
	void DisabeVelocityProfile() { VelocityProfile = false; }

	//functions to calculate the variable changes to the object
	void CalculateHeat();
	void CalculateFlowRate();
	template<size_t array_size>
	void CalculateEnthalpy(std::vector<std::array<std::string, array_size>> &steam_table);
	void CalculateVelocity(std::unique_ptr<std::vector<std::array<std::string, 8>>>& SteamTables);

	std::string returnFeed1() { return "Flow Rate: " + format(feed1, 4) + " Kg/min"; }
	std::string returnFeed2() { return "Flow Rate: " + format(feed2, 4)+ " Kg/min"; }
	std::string returnFeed3() { return "Flow Rate: " + format(feed3, 4) + " Kg/min"; }
	std::string returnTemp1() { return "Temperature: " + format(temperature1, 4) + " C"; }
	std::string returnTemp2() { return "Temperature: " + format(temperature2, 4) + " C"; }
	std::string returnTemp3() { return "Temperature: " + format(temperature_outlet, 4) + " C"; }
	std::string returnPressure1() { return "Pressure: " + format(pressure1, 3) + " MPA"; }
	std::string returnPressure2() { return "Pressure: " + format(pressure2, 3) + " MPA"; }
	std::string returnEnthalpy1() { return "Enthalpy: " + format(enthalpy_feed1, 5) + " kJ/kg"; }
	std::string returnEnthalpy2() { return "Enthalpy: " + format(enthalpy_feed2, 5) + " kJ/kg"; }
	std::string returnEnthalpy3() { return "Enthalpy: " + format(enthalpy_outlet, 5) + " kJ/kg"; }
	std::string returnOutletPressure() { return "Pressure: " + format(pressure3, 4) + " MPA"; }
	std::string returnHeatEvolved() { return "Heat: " + format(Heating_Element, 5) + " TJ"; }
	std::string returnVelocity1() { return "Velcotiy: " + format(_kinetic1.velocity, 4) + " m/s"; }
	std::string returnVelocity2() { return "Velcotiy: " + format(_kinetic2.velocity, 4) + " m/s"; }
	std::string returnVelocity3() { return "Velcotiy: " + format(_kinetic3.velocity, 4) + " m/s"; }
	std::string returnDiameter1() { return "Diameter: " + format(_kinetic1.diameter, 3) + " Cm"; }
	std::string returnDiameter2() { return "Diameter: " + format(_kinetic2.diameter, 3) + " Cm"; }
	std::string returnDiameter3() { return "Diameter: " + format(_kinetic3.diameter, 3) + " Cm"; }
	Phase returnPhase1();
	Phase returnPhase2();
	Phase returnPhase3();
	template<typename T>
	T returnQuality1();
	template<typename T>
	T returnQuality2();
	template<typename T>
	T returnQuality3();
};



