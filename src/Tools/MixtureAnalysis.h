#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include "../src//Tools/PhysicalProperties.h"
#include "../src//Tools/VaporPressureCalculator.h"

template<typename T1, typename T2>
std::string _Format(T1 f, T2 n);

inline double powerup(double temperature2, double temperature1, double power);

#define GAS_CONSTANT 8.3144598

class MixtureAnalysis
{
	friend PhysicalProperties;
private:
	PhysicalProperties m_properties;
	VaporPressureCalculator AntoineCalculator;

	std::vector<std::string> Components;
	std::vector<float> liquidFraction_Components;
	std::vector<float> vaporFraction_Components;
	std::vector<float> SaturatedPressure;
	std::vector<float> m_CriticalTemperatures;
	std::vector<float> m_CriticalPressures;
	std::vector<float> m_AcentricFactors;
	enum class VaporPressure
	{ ANTOINE, SHORTCUT, NONE};
	std::array<std::string, 2> methods {"Antoine Vapor Pressure", "Shortcut Vapor Pressure"};
	const char* current_specie = "Choose";
	std::tuple<std::string, VaporPressure> Method = std::make_tuple<std::string, VaporPressure>("Choose", VaporPressure::NONE);
	//VaporPressure Method = VaporPressure::NONE;
public:
	static void MixtureAnalyisTool(bool* p_open);
	MixtureAnalysis() {m_properties = PhysicalProperties(PhysicalProperties::type::Elliot);}
	void SpeciesList();
	void AddCurrentSpecie();
	void ShowCurrentComponents();
	void ResetCurrentComponents();
	void VaporPressureCalculationMethod();

	std::tuple<std::string, VaporPressure> VaporPressureMethod() {return Method;}
};

