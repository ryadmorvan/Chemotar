#include "Boiler.h"




//Declare the function name from different file
template<size_t size>
float CalculateFromSteamTable(std::unique_ptr<std::vector<std::array<std::string, size>>>& SteamTable, SteamTableFlag table, CompressedSuperheatedTablesFlags calculate, float& pressure, float& temperature, float &enthalpy, Phase &phase);

//Flag that will be used to indicate which table will be used for our calculations
enum class SteamTableFlag
{
	COMPRESSED_SUPERHEATED_TABLE = 0x01, SATURATED_TABLE = 0x02
};

//Flags that will be used to indicate which variables that will be calculated from the steam tables
enum class CompressedSuperheatedTablesFlags
{
	PRESSURE = 0X01, TEMPERATURE = 0X02, DENSITY = 0X03, INTERNAL_ENERGY = 0x04, ENTHALPY = 0X05, ENTROPY = 0X06, SPECIFIC_VOLUME = 0x07
};



boiler<FEED::SINGLE>::boiler() : 
	feed(0), temperature_inlet(273.15), temperature_outlet(273.15), 
	enthalpy_feed(200), enthalpy_outlet(500), outlet_diameter(5), Boiler()
{
}

boiler<FEED::DOUBLE>::boiler()
	:feed1(0), feed2(0), temperature1(273.15), temperature2(273.15), temperature_outlet(298.15), _Boiler()
	, _phase1(" Liquid", ImColor(59, 154, 225)), _phase2(" Liquid", ImColor(59, 154, 225)), _phase3(" Liquid", ImColor(59, 154, 225))
{

}

//Initally set all the values for ours feeds
boiler<FEED::DOUBLE>::boiler(float FEED_1, float FEED_2,  float Temperature1, float Temperature2, float Pressure1, float Pressure2, float Pressure3,
	float Temperature_Outlet, std::unique_ptr<std::vector<std::array<std::string, 8>>>& SteamTable)  : boiler()
{
	feed1 = FEED_1;
	feed2 = FEED_2;
	feed3 = FEED_1 + FEED_2;
	temperature1 = Temperature1;
	temperature2 = Temperature2;
	temperature_outlet = Temperature_Outlet;
	pressure1 = Pressure1; pressure2 = Pressure2; pressure3 = Pressure3;
	//Calculate the appropriate values for enthalpies using steam table and passing our variables
	//phase object will be passed on this function aswell in order to determine the phase of the inlets and outlet
	enthalpy_feed1 = CalculateFromSteamTable(SteamTable, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::ENTHALPY, pressure1, temperature1, enthalpy_feed1, _phase1);
	enthalpy_feed2 = CalculateFromSteamTable(SteamTable, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::ENTHALPY, pressure2, temperature2, enthalpy_feed2, _phase2);
	enthalpy_outlet = CalculateFromSteamTable(SteamTable, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::ENTHALPY, pressure3, temperature_outlet, enthalpy_outlet, _phase3);
}

//Returns the phase information depending on what CalculateFromSteamTable set the phase for each inlet and outlet
Phase boiler<FEED::DOUBLE>::returnPhase1() {
	if (_phase1.phase == " Liquid") {
		_phase1.quality = 0;
		_phase1.color = ImColor(59, 154, 225); return _phase1;
	}
	else if ((_phase1.phase == " Saturated Liquid") or (_phase1.phase == " Saturated Vapor"))
	{
		if (_phase1.quality > 0.5)
		{
			_phase1.phase = " Saturated Vapor";
		}
		else
			_phase1.phase = " Saturated Liquid";
		_phase1.color = ImColor(159, 254, 225); return _phase1;
	}
	else if (_phase1.phase == " Vapor")
	{
		_phase1.quality = 1;
		_phase1.color = ImColor(59, 254, 225); return _phase1;
	}
	_phase1.color = ImColor(59, 154, 225);
	return _phase1;
}

Phase boiler<FEED::DOUBLE>::returnPhase2() {
	if (_phase2.phase == " Liquid") {
		_phase2.quality = 0;
		_phase2.color = ImColor(59, 154, 225); return _phase2;
	}
	else if ((_phase2.phase == " Saturated Liquid") or (_phase2.phase == " Saturated Vapor"))
	{
		if (_phase2.quality > 0.5)
		{
			_phase2.phase = " Saturated Vapor";
		}
		else
			_phase2.phase = " Saturated Liquid";
		_phase2.color = ImColor(159, 254, 225); return _phase2;
	}
	else if (_phase2.phase == " Vapor")
	{
		_phase2.quality = 1;
		_phase2.color = ImColor(59, 254, 225); return _phase2;
	}
	_phase2.color = ImColor(59, 154, 225);
	return _phase2;
}

Phase boiler<FEED::DOUBLE>::returnPhase3() {
	if (_phase3.phase == " Liquid") {
		_phase3.quality = 0;
		_phase3.color = ImColor(59, 154, 225); return _phase3;
	}
	else if ((_phase3.phase == " Saturated Liquid") or (_phase3.phase == " Saturated Vapor"))
	{
		if (_phase3.quality > 0.5)
		{
			_phase3.phase = " Saturated Vapor";
		}
		else
			_phase3.phase = " Saturated Liquid";
		_phase3.color = ImColor(159, 254, 225); return _phase3;
	}
	else if (_phase3.phase == " Vapor")
	{
		_phase3.quality = 1;
		_phase3.color = ImColor(59, 254, 225); return _phase3;
	}
	_phase3.color = ImColor(59, 154, 225);
	return _phase3;
}

//Templated returnQuality function that will be used to either return a float or a string (that will be used to draw on the information section of the inlets/outlets)
template<typename T>
T boiler<FEED::DOUBLE>::returnQuality1()
{
	if constexpr (std::is_same<T, float>())
	{
		return _phase1.quality;
	}
	if constexpr(std::is_same<T, std::string>())
	{
		return "Quality: " + _Format(_phase1.quality, 2);
	}
}

template<typename T>
T boiler<FEED::DOUBLE>::returnQuality2()
{
	if constexpr (std::is_same<T, float>())
	{
		return _phase2.quality;
	}
	if constexpr (std::is_same<T, std::string>())
	{
		return "Quality: " + _Format(_phase2.quality, 2);
	}
}
template<typename T>
T boiler<FEED::DOUBLE>::returnQuality3()
{
	if constexpr (std::is_same<T, float>())
	{
		return _phase3.quality;
	}
	if constexpr (std::is_same<T, std::string>())
	{
		return "Quality: " + _Format(_phase3.quality, 2);
	}
}




//Start drawing the relative variable information for each inlet/outlet
void boiler<FEED::DOUBLE>::DrawInfo(ImDrawList* draw_list,DrawShapes& Arrow_In1, DrawShapes& Arrow_In2, DrawShapes& Arrow_Out1)
{
	//Feed and outlet rates Info
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 120), Arrow_In1.returnColor(), returnFeed1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 120), Arrow_In2.returnColor(), returnFeed2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 120), Arrow_Out1.returnColor(), returnFeed3().c_str());
	//Feeds and outlet Temperatures
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 100), Arrow_In1.returnColor(), returnTemp1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 100), Arrow_In2.returnColor(), returnTemp2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 100), Arrow_Out1.returnColor(), returnTemp3().c_str());
	//Feeds and outlet Enthalpy
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 80), Arrow_In1.returnColor(), returnEnthalpy1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 80), Arrow_In2.returnColor(), returnEnthalpy2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 80), Arrow_Out1.returnColor(), returnEnthalpy3().c_str());
	//Feeds Pressure
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 60), Arrow_In1.returnColor(), returnPressure1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 60), Arrow_In2.returnColor(), returnPressure2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 60), Arrow_Out1.returnColor(), returnOutletPressure().c_str());

	//Feeds phase
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 40), returnPhase1().color, ("Phase:" + returnPhase1().phase).c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 40), returnPhase2().color, ("Phase:" + returnPhase2().phase).c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 40), returnPhase3().color, ("Phase:" + returnPhase3().phase).c_str());

	//Feeds Quality
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 20), returnPhase1().color, (returnQuality1<std::string>().c_str()));
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 20), returnPhase2().color, (returnQuality2<std::string>().c_str()));
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 20), returnPhase3().color, (returnQuality3<std::string>().c_str()));

	//Feeds Velocity Profile
	if (VelocityProfile)
	{
		draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() + 10), Arrow_In1.returnColor(), returnDiameter1().c_str());
		draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() + 10), Arrow_In2.returnColor(), returnDiameter2().c_str());
		draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() + 10), Arrow_Out1.returnColor(), returnDiameter3().c_str());
		draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() + 30), returnPhase1().color, returnVelocity1().c_str());
		draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() + 30), returnPhase2().color, returnVelocity2().c_str());
		draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() + 30), returnPhase3().color, returnVelocity3().c_str());
	}

	//Outlet extra conditions
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() + 50), ImColor(200, 80, 80, 220), returnHeatEvolved().c_str());


}




//Calculating all the variables
template<size_t array_size>
void boiler<FEED::DOUBLE>::CalculateEnthalpy(std::vector<std::array<std::string, array_size>> &steam_table)
{

}


void boiler<FEED::DOUBLE>::CalculateHeat()
{
	Heating_Element = feed3 * enthalpy_outlet - feed1 * enthalpy_feed1 - feed2 * enthalpy_feed2;
	Heating_Element = Heating_Element / 1000.0f;
}

void boiler<FEED::DOUBLE>::CalculateFlowRate()
{
	feed3 = feed1 + feed2;
}

void boiler<FEED::DOUBLE>::CalculateVelocity(std::unique_ptr<std::vector<std::array<std::string, 8>>>& SteamTables)
{
	float specific_volume1 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::SPECIFIC_VOLUME, pressure1, temperature1, enthalpy_feed1, _phase1);
	float specific_volume2 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::SPECIFIC_VOLUME, pressure2, temperature2, enthalpy_feed2, _phase2);
	float specific_volume3 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, CompressedSuperheatedTablesFlags::SPECIFIC_VOLUME, pressure3, temperature_outlet, enthalpy_outlet, _phase3);
	_kinetic1.volumetric_flow_rate = feed1 * specific_volume1 / 60;
	_kinetic2.volumetric_flow_rate = feed2 * specific_volume2 / 60;
	_kinetic3.volumetric_flow_rate = feed3 * specific_volume3 / 60;
	_kinetic1.find_velocity();
	_kinetic2.find_velocity();
	_kinetic3.find_velocity();

}


