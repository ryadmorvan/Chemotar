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
	double heat;
	double HeatCapacityConstPressure;
	double HeatCapacityConstVolume;
	double work = 0;
	enum process {Non = 0, constant_pressure = 5, constant_volume = 6, constant_temperature = 7, constant_heat = 8};
	enum species { Air = 0 };
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


	Piston(species specie)
	{
		if (specie == Piston::species::Air)
		{
			HeatCapacityConstPressure = 29.19; // J/mol*k
			HeatCapacityConstVolume = 20.85;
		}
		volume = 22.4;
		temperature = 273;
		moles = 1;
		pressure = 1;
		heat = 0;
		work = 0.0;
	}
	void VolumeChange(double volumeChange, Piston::process state)
	{
		if (state == constant_pressure)
		{
			temperature = temperature * (volumeChange / volume);
			volume = volumeChange;
		}
		if (state == constant_volume)
		{

		}
	}
	void TemperatureChange(double temperatureChange, Piston::process state)
	{
		temperature = temperatureChange;
	}
	void MolesChange(double molesChange, Piston::process state)
	{
		volume = volume * molesChange / moles;
		moles = molesChange;

	}
	void PressureChange(double pressureChange, Piston::process state)
	{
		if (state == Piston::process::constant_temperature)
		{
			heat = moles * 8.314 * temperature * log(1 / pressureChange) / 1000;
			pressure = pressureChange;
			volume = moles * 0.08206 * temperature / pressure;
			work = -heat;
		}
		if (state == Piston::process::constant_heat)
		{
			temperature = temperature * pow(pressureChange / 1.0, 8.314 / HeatCapacityConstPressure);
			pressure = pressureChange;
			volume = moles * 0.08206 * temperature / pressure;
			work = moles * HeatCapacityConstVolume * (temperature - 273.15) / 1000;
		}
	}
	void AddHeat(float Heat, Piston::process state)
	{
		if (state == Piston::constant_pressure)
		{
			heat = Heat/1000;
			temperature = (Heat + HeatCapacityConstPressure * temperature) / HeatCapacityConstPressure;
			volume = moles * 0.08206 * temperature / pressure;
			work = pressure * (101325) * ((volume - 22.4) / 1000) / 1000;
		}
		if (state == Piston::constant_volume)
		{
			heat = Heat / 1000;
			temperature = (Heat + moles* HeatCapacityConstVolume * temperature) / (HeatCapacityConstVolume * moles);
			pressure = moles * 0.08206 * temperature / volume;
		}

	}

	std::string returnVolumeText()
	{
		return "Volume: " + Format(volume, 4) + " L";
	}
	std::string returnPressureText()
	{
		return "Pressure: " + Format(pressure, 3) + " atm";
	}
	std::string returnTemperatureText()
	{
		return "Temperature: " + Format(temperature, 4) + " K";
	}
	std::string returnMolesText()
	{
		return "Moles: " + Format(moles, 3) + " mol";
	}
	std::string returnHeatText()
	{
		return "Heat Evolved: " + Format(heat, 3) + " KJ";
	}
	std::string returnWorkText()
	{
		return "Work Done: " + Format(work, 3) + " KJ";
	}

	double returnHeat()
	{
		return heat;
	}
	float returnChangeInPistonHeight()
	{
		return (volume - 22.4) * 3.88392;
	}
};

