#include "Piston.h"


std::string Piston::Format(double f, int n)
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

//Ideal gas equations is used to calculate all the changes
void Piston::VolumeChange(double volumeChange, Piston::process state)
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
//methods that calculates the other variables when pressure changes on a certain Piston::process
void Piston::PressureChange(double pressureChange, Piston::process state)
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
		temperature = 273 * pow(pressureChange / 1.0, 8.314 / HeatCapacityConstPressure);
		pressure = pressureChange;
		volume = moles * 0.08206 * temperature / pressure;
		work = moles * HeatCapacityConstVolume * (temperature - 273.15) / 1000;

	}
}
//methods that calculates the other variables when heat is added or rejected on a certain Piston::process
void Piston::AddHeat(float Heat, Piston::process state)
{
	if (state == Piston::constant_pressure)
	{
		heat = Heat / 1000;
		temperature = (Heat + HeatCapacityConstPressure * temperature) / HeatCapacityConstPressure;
		volume = moles * 0.08206 * temperature / pressure;
		work = pressure * (101325) * ((volume - 22.4) / 1000) / 1000;
	}
	if (state == Piston::constant_volume)
	{
		heat = Heat / 1000;
		temperature = (Heat + moles * HeatCapacityConstVolume * temperature) / (HeatCapacityConstVolume * moles);
		pressure = moles * 0.08206 * temperature / volume;
	}

}
