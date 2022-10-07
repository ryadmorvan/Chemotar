#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include "../src/Tools/PhysicalProperties.h"



template<typename T1, typename T2>
std::string _Format(T1 f, T2 n);




class PengRobinson
{
	friend PhysicalProperties;
private:
	//class object where we store the data of our chemical properties and heat capacity coefficient
	PhysicalProperties m_properties;
	std::string SpecieName = "NULL";
	float m_Tcritical = 0;
	float m_Pcritical = 0;
	float m_AcentricFactor = 0;
	//Heat Capacity Coefficients
public:
	float HeatCapacityCoefficients[5] = {0, 0, 0,0, 0};
	PengRobinson() {m_properties = PhysicalProperties(PhysicalProperties::type::Elliot);}
	static void PengRobinsonCalculator(bool* p_open);
	void setCriticalTemperature(float value) {m_Tcritical = value;}
	void setCriticalPressure(float value) {m_Pcritical = value;}
	void setAcentricFactor(float value) {m_AcentricFactor = value;}
	void setSpecieName(std::string name) {SpecieName = name;}

	std::string returnDetails() {
		return "Name: " + SpecieName + "\nCritical Temperature: " + _Format(m_Tcritical, 5) 
			+" K\nCritical Pressure: " + _Format(m_Pcritical, 5) + " Mpa\nAcentric Factor: " + _Format(m_AcentricFactor, 5);
	}
	std::string returnHeatCapacityDetails()
	{
		return ((HeatCapacityCoefficients[0] != 0) ? ("Heat Capacity Coeff 1: " +_Format(HeatCapacityCoefficients[0], 5) + "\nHeat Capacity Coeff 2: " + _Format(HeatCapacityCoefficients[1], 5) + "\nHeat Capacity Coeff 3: "
				+ _Format(HeatCapacityCoefficients[2], 8) + "\nHeat Capacity Coeff 4: " + _Format(HeatCapacityCoefficients[3], 10)): 
				
				
				
				"ERROR SPECIE HEAT CAPACITY NOT YET IMPLEMENTED");
	}

	float KappaCalculation(float AcentricFactor) {0.37464 + 1.54226*AcentricFactor - 0.26992*AcentricFactor*AcentricFactor;}
	float Alpha(float temperature, float temperatureCrit, float AcentricFactor) {return powf(1+ KappaCalculation(AcentricFactor)*(1-powf(temperature/temperatureCrit, 0.5)), 2);}
};

