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


template<typename T1, typename T2>
std::string _Format(T1 f, T2 n);

inline double powerup(double temperature2, double temperature1, double power);

#define GAS_CONSTANT 8.3144598

class MixtureAnalysis
{
	friend PhysicalProperties;
private:
	PhysicalProperties m_properties;
	std::vector<std::string> Components;
	std::vector<float> liquidFraction_Components;
	std::vector<float> vaporFraction_Components;
	std::vector<float> SaturatedPressure;
	std::vector<float> m_CriticalTemperatures;
	std::vector<float> m_CriticalPressures;
	std::vector<float> m_AcentricFactors;
	const char* current_specie = "Choose";
public:
	static void MixtureAnalyisTool(bool* p_open);
	MixtureAnalysis() {m_properties = PhysicalProperties(PhysicalProperties::type::Elliot);}
	void SpeciesList();
	void AddCurrentSpecie();
	void ShowCurrentComponents();
	void ResetCurrentComponents();
};

