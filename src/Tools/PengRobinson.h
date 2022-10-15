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


#define GAS_CONSTANT 8.3144598

class PengRobinson
{
	friend PhysicalProperties;
private:
	//class object where we store the data of our chemical properties and heat capacity coefficient
	PhysicalProperties m_properties;
	//Species properties
	std::string SpecieName = "NULL";

	std::vector<float> solutions;

	float m_Tcritical = 0;
	float m_Pcritical = 0;
	float m_AcentricFactor = 0;

	//Our system pressure and temperature
	float m_pressure = 0;
	float m_temperature = 0;

	//coefficients used for calculations
	float m_Tr = 0;
	float m_Pr = 0;
	float m_Kappa = 0;
	float m_Alpha = 0;
	float m_ac = 0;
	float m_a = 0;
	float m_b = 0;
	
	float m_A = 0;
	float m_B = 0;


	bool solver_flag_accuracy = NULL;
	//Heat Capacity Coefficients
public:
	float HeatCapacityCoefficients[5] = {1, 0, 0,0, 0};
	PengRobinson() {m_properties = PhysicalProperties(PhysicalProperties::type::Elliot);}
	static void PengRobinsonCalculator(bool* p_open);
	void setCriticalTemperature(float value) {m_Tcritical = value;}
	void setCriticalPressure(float value) {m_Pcritical = value;}
	void setAcentricFactor(float value) {m_AcentricFactor = value;}
	void setSpecieName(std::string name) {SpecieName = name;}

	void setPressure(float pressure) {m_pressure = pressure;}
	void setTemperature(float temperature) {m_temperature = temperature;}




	std::string returnDetails() {
		return "Name: " + SpecieName + "\nCritical Temperature: " + _Format(m_Tcritical, 5) 
			+" K\nCritical Pressure: " + _Format(m_Pcritical, 5) + " Mpa\nAcentric Factor: " + _Format(m_AcentricFactor, 5);
	}
	std::string returnHeatCapacityDetails()
	{
		return ((HeatCapacityCoefficients[0] != 0) ? ("Heat Capacity Coeff 1: " +_Format(HeatCapacityCoefficients[0], 5) + "\nHeat Capacity Coeff 2: " + _Format(HeatCapacityCoefficients[1], 5) + "\nHeat Capacity Coeff 3: "
				+ _Format(HeatCapacityCoefficients[2], 8) + "\nHeat Capacity Coeff 4: " + _Format(HeatCapacityCoefficients[3], 10)): 
				
				
				
				"Specie heat capacity not yet implemented.");
	}
	float* returnTemperaturePointer() {return &m_temperature;}
	float* returnPressurePointer() {return &m_pressure;}
	float KappaCalculation(float AcentricFactor) {return (0.37464 + 1.54226*AcentricFactor - 0.26992*AcentricFactor*AcentricFactor);}
	float AlphaCalculation() {return powf(1+ KappaCalculation(m_AcentricFactor)*(1-powf(m_Tr, 0.5)), 2);}
	float aCalculation() {return AlphaCalculation() * 0.45723553*GAS_CONSTANT*GAS_CONSTANT*m_Tcritical*m_Tcritical/(m_Pcritical*pow(10, 6));}
	float bCalculation() {return 0.07779607*GAS_CONSTANT*m_Tcritical/(m_Pcritical*pow(10, 6));}
	float A_Calculation() {return m_a*m_pressure*pow(10, 6)/(GAS_CONSTANT*GAS_CONSTANT*m_temperature*m_temperature);}
	float B_Calculation() {return m_b*m_pressure*pow(10, 6)/(GAS_CONSTANT*m_temperature);}

	std::vector<float> CubicEquationSolver();

	float CalculateMolarVolume(float Z) {return Z*83.144598*m_temperature/(m_pressure*10);}
	float CalculateFugacityCoef(float Z)
	{
		return Z - 1 - log(Z-m_B) - m_A/(sqrtf(8)*m_B)*log( (Z+(1+sqrtf(2))*m_B)/(Z+(1-sqrtf(2))*m_B) );
	}
	float CalculateFugacity(float Z) {
		return exp(CalculateFugacityCoef(Z))*m_pressure;
	}

	float CalculateEnthalpyDeparture(float Z)
	{
		return Z - 1 - m_A/(m_B*sqrtf(8))*(1+m_Kappa*sqrtf(m_Tr)/sqrtf(m_Alpha)*log((Z+(1+sqrtf(2))*m_B)/(Z+(1-sqrtf(2))*m_B)));
	}



	void Calculate() {m_Tr = m_temperature/m_Tcritical; m_Pr = m_pressure/m_Pcritical; m_Kappa = KappaCalculation(m_AcentricFactor); m_Alpha = AlphaCalculation(); m_a= aCalculation(); m_b = bCalculation();
		m_A = A_Calculation(); m_B = B_Calculation();
			solutions = CubicEquationSolver();
	}


	std::string returnCoefficients()
	{
		return "Tr: " + _Format(m_Tr, 5) + "							 Pr: " + _Format(m_Pr, 5) + "\nK: " + _Format(m_Kappa, 5) + "							  Alpha: " + _Format(m_Alpha, 5) + "\na: " + _Format(m_a, 5) + " Pa*m^3/mol		  b: " + _Format(m_b, 5) +
			" m^3/mol\nA: " + _Format(m_A, 5) + "							  B: " + _Format(m_B, 5);
			;
	}

	void ShowResults();
	void ShowResultsReference();

	bool* solver_flag()
	{
		return &solver_flag_accuracy;
	}
};

