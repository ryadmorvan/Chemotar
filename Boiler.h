#pragma once
#include <iostream>

namespace FEED
{
	struct SINGLE;
	struct DOUBLE;
}



template<typename T> // primary template
class boiler : std::false_type {};

template<>           // explicit specialization for T = void
class boiler<FEED::SINGLE>
{
public:
	float feed;
	float temperature_inlet;
	float temperature_outlet;
	float enthalpy_feed;
	float enthalpy_outlet;
	float outlet_diameter;

};

template<>
class boiler<FEED::DOUBLE> 
{
public:
	float feed1;
	float feed2;
	float temperature1;
	float temperature2;
	float enthalpy_feed1;
	float enthalpy_feed2;
	float enthalpy_outlet;
	float outlet_diameter;
	float temperature_outlet;
};



