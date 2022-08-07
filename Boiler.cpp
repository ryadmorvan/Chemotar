#include "Boiler.h"

boiler<FEED::SINGLE>::boiler() : 
	feed(0), temperature_inlet(273.15), temperature_outlet(273.15), 
	enthalpy_feed(200), enthalpy_outlet(500), outlet_diameter(5), Boiler()
{
}

boiler<FEED::DOUBLE>::boiler()
	:feed1(0), feed2(0), temperature1(273.15), temperature2(273.15), enthalpy_feed1(200),
	enthalpy_feed2(500), enthalpy_outlet(800), outlet_diameter(5), temperature_outlet(298.15), Boiler()
{
}

boiler<FEED::DOUBLE>::boiler(float FEED_1, float FEED_2,  float Temperature1, float Temperature2,
	float Enthalpy_Feed1, float Enthalpy_Feed2, float Enthalpy_Outlet, float Outlet_Diameter,
	float Temperature_Outlet, bool saturation)
{
	feed1 = FEED_1;
	feed2 = FEED_2;
	feed3 = FEED_1 + FEED_2;
	temperature1 = Temperature1;
	temperature2 = Temperature2;
	enthalpy_feed1 = Enthalpy_Feed1;
	enthalpy_feed2 = Enthalpy_Feed2;
	enthalpy_outlet = Enthalpy_Outlet;
	temperature_outlet = Temperature_Outlet;
	outlet_diameter = Outlet_Diameter;
	Saturated = saturation;
}

void boiler<FEED::DOUBLE>::DrawInfo(ImDrawList* draw_list,DrawShapes& Arrow_In1, DrawShapes& Arrow_In2, DrawShapes& Arrow_Out1)
{
	//Feed and outlet rates Info
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 60), Arrow_In1.returnColor(), returnFeed1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 60), Arrow_In2.returnColor(), returnFeed2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 100), Arrow_Out1.returnColor(), returnFeed3().c_str());
	//Feeds and outlet Temperatures
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 40), Arrow_In1.returnColor(), returnTemp1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 40), Arrow_In2.returnColor(), returnTemp2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 80), Arrow_Out1.returnColor(), returnTemp3().c_str());
	//Feeds and outlet Enthalpy
	draw_list->AddText(ImVec2(Arrow_In1.returnX(), Arrow_In1.returnY() - 20), Arrow_In1.returnColor(), returnEnthalpy1().c_str());
	draw_list->AddText(ImVec2(Arrow_In2.returnX(), Arrow_In2.returnY() - 20), Arrow_In2.returnColor(), returnEnthalpy2().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 60), Arrow_Out1.returnColor(), returnEnthalpy3().c_str());

	//Outlet extra conditions
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 40), Arrow_Out1.returnColor(), returnOutletPressure().c_str());
	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() - 20), Arrow_Out1.returnColor(), returnOutletDiameter().c_str());

	draw_list->AddText(ImVec2(Arrow_Out1.returnX() + 10, Arrow_Out1.returnY() + 20), ImColor(200, 80, 80, 220), returnHeatEvolved().c_str());


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