#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <SFML/Graphics.hpp>



class Piston
{
public:
	double volume;
	double temperature;
	double moles;
	double pressure;
	enum process {constant_pressure = 0, constant_volume = 1, constant_temperature = 2};
public:
	int Round(double number)
	{
		return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
	}

	std::string Format(double f, int n)
	{
		if (f == 0) {
			return "0";
		}
		int d = (int)::ceil(::log10(f < 0 ? -f : f)); /*digits before decimal point*/
		double order = ::pow(10., n - d);
		std::stringstream ss;
		ss << std::fixed << std::setprecision(std::max(n - d, 0)) << round(f * order) / order;
		return ss.str();
	}


	Piston()
	{
		volume = 22.4;
		temperature = 273;
		moles = 1;
		pressure = 1;
	}
	void VolumeChange(double volumeChange, Piston::process state)
	{
		if (state == constant_pressure)
		{
			temperature = temperature * (volumeChange / volume);
			volume = volumeChange;
		}
	}
	void TemperatureChange(double temperatureChange)
	{
		temperature = temperatureChange;
	}
	void MolesChange(double molesChange)
	{
		moles = molesChange;
	}
	void PressureChange(double pressureChange)
	{
		pressure = pressureChange;
	}

	std::string returnVolumeText()
	{
		return "Volume: " + Format(volume, 4) + " L";
	}
	std::string returnPressureText()
	{
		return "Pressure: " + Format(pressure, 2) + " atm";
	}
	std::string returnTemperatureText()
	{
		return "Temperature: " + Format(temperature, 4) + " K";
	}
	std::string returnMolesText()
	{
		return "Moles: " + Format(moles, 2) + " mol";
	}
};

