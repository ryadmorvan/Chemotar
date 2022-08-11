#include "Boiler.h"


template<size_t size>

float CalculateFromSteamTable(std::unique_ptr<std::vector<std::array<std::string, size>>>& SteamTable, SteamTableFlag table, SteamTableCalculate calculate, float& pressure, float& temperature, float &enthalpy, std::string& phase);


enum class SteamTableFlag
{
	COMPRESSED_SUPERHEATED_TABLE = 0x01, SATURATED_TABLE = 0x02
};

enum class SteamTableCalculate
{
	PRESSURE = 0X01, TEMPERATURE = 0X02, DENSITY = 0X03, INTERNAL_ENERGY = 0x04, ENTHALPY = 0X05, ENTROPY = 0X06, SPECIFIC_VOLUME = 0x07
};



boiler<FEED::SINGLE>::boiler() : 
	feed(0), temperature_inlet(273.15), temperature_outlet(273.15), 
	enthalpy_feed(200), enthalpy_outlet(500), outlet_diameter(5), Boiler()
{
}

boiler<FEED::DOUBLE>::boiler()
	:feed1(0), feed2(0), temperature1(273.15), temperature2(273.15), temperature_outlet(298.15), Boiler()
	, phase1(" Liquid", ImColor(59, 154, 225)), phase2(" Liquid", ImColor(59, 154, 225)), phase3(" Liquid", ImColor(59, 154, 225))
{

}

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
	enthalpy_feed1 = CalculateFromSteamTable(SteamTable, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure1, temperature1, enthalpy_feed1, phase1.phase);
	enthalpy_feed2 = CalculateFromSteamTable(SteamTable, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure2, temperature2, enthalpy_feed2, phase2.phase);
	enthalpy_outlet = CalculateFromSteamTable(SteamTable, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::ENTHALPY, pressure3, temperature_outlet, enthalpy_outlet, phase3.phase);
}

Phase boiler<FEED::DOUBLE>::returnPhase1() {
	if (phase1.phase == " Liquid") {
		phase1.color = ImColor(59, 154, 225); return phase1;
	}
	else if ((phase1.phase == " Saturated Liquid") or (phase1.phase == " Saturated Vapor"))
	{
		phase1.color = ImColor(159, 254, 225); return phase1;
	}
	else if (phase1.phase == " Vapor")
	{
		phase1.color = ImColor(59, 254, 225); return phase1;
	}
	phase1.color = ImColor(59, 154, 225);
	return phase1;
}

Phase boiler<FEED::DOUBLE>::returnPhase2() {
	if (phase2.phase == " Liquid") {
		phase2.color = ImColor(59, 154, 225); return phase2;
	}
	else if ((phase2.phase == " Saturated Liquid") or (phase2.phase == " Saturated Vapor"))
	{
		phase2.color = ImColor(159, 254, 225); return phase2;
	}
	else if (phase2.phase == " Vapor")
	{
		phase2.color = ImColor(59, 254, 225); return phase2;
	}
	phase2.color = ImColor(59, 154, 225);
	return phase2;
}

Phase boiler<FEED::DOUBLE>::returnPhase3() {
	if (phase3.phase == " Liquid") {
		phase3.color = ImColor(59, 154, 225); return phase3;
	}
	else if ((phase3.phase == " Saturated Liquid") or (phase3.phase == " Saturated Vapor"))
	{
		phase3.color = ImColor(159, 254, 225); return phase3;
	}
	else if (phase3.phase == " Vapor")
	{
		phase3.color = ImColor(59, 254, 225); return phase3;
	}
	phase3.color = ImColor(59, 154, 225);
	return phase3;
}





void boiler<FEED::DOUBLE>::DrawInfo(ImDrawList* draw_list,DrawShapes& Arrow_In1, DrawShapes& Arrow_In2, DrawShapes& Arrow_Out1)
{
	//Feed and outlet rates Info
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 100), Arrow_In1.returnColor(), returnFeed1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 100), Arrow_In2.returnColor(), returnFeed2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 100), Arrow_Out1.returnColor(), returnFeed3().c_str());
	//Feeds and outlet Temperatures
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 80), Arrow_In1.returnColor(), returnTemp1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 80), Arrow_In2.returnColor(), returnTemp2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 80), Arrow_Out1.returnColor(), returnTemp3().c_str());
	//Feeds and outlet Enthalpy
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 60), Arrow_In1.returnColor(), returnEnthalpy1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 60), Arrow_In2.returnColor(), returnEnthalpy2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 60), Arrow_Out1.returnColor(), returnEnthalpy3().c_str());
	//Feeds Pressure
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 40), Arrow_In1.returnColor(), returnPressure1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 40), Arrow_In2.returnColor(), returnPressure2().c_str());

	//Feeds phase
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 20), returnPhase1().color, ("Phase:" + returnPhase1().phase).c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 20), returnPhase2().color, ("Phase:" + returnPhase2().phase).c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 20), returnPhase3().color, ("Phase:" + returnPhase3().phase).c_str());


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
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 40), Arrow_Out1.returnColor(), returnOutletPressure().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() + 50), ImColor(200, 80, 80, 220), returnHeatEvolved().c_str());


}




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
	float specific_volume1 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::SPECIFIC_VOLUME, pressure1, temperature1, enthalpy_feed1, phase1.phase);
	float specific_volume2 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::SPECIFIC_VOLUME, pressure2, temperature2, enthalpy_feed2, phase2.phase);
	float specific_volume3 = CalculateFromSteamTable(SteamTables, SteamTableFlag::COMPRESSED_SUPERHEATED_TABLE, SteamTableCalculate::SPECIFIC_VOLUME, pressure3, temperature_outlet, enthalpy_outlet, phase3.phase);
	kinetic1.VolumetricFlowRate = feed1 * specific_volume1 / 60;
	kinetic2.VolumetricFlowRate = feed2 * specific_volume2 / 60;
	kinetic3.VolumetricFlowRate = feed3 * specific_volume3 / 60;
	//kinetic1.setDiameter(0.03); //In meters
	//kinetic2.setDiameter(0.03);
	//kinetic3.setDiameter(0.06);
	kinetic1.findVelocity();
	kinetic2.findVelocity();
	kinetic3.findVelocity();
	//std::cout << specific_volume3 << std::endl;
	//std::cout << kinetic3.VolumetricFlowRate << std::endl;
	//std::cout << kinetic3.Velocity << std::endl;
}


