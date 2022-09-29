#pragma once
#ifndef CHEMICALCALCULATIONS
#define CHEMICALCALCULATIONS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include "../src/Utility/UtilitySystemHandler.h"

//State of the specie
enum STATE
{
	LIQUID = 0X01, GAS = 0X02
};


inline double powerup(double temperature2, double temperature1, double power)
{
	return pow(temperature2, power) - pow(temperature1, power);
}

//Calculates the enthalpy evolved
inline double enthalpy_evolved(double coeff1, double coeff2, double coeff3, double coeff4, double coeff5, double temperature1, double temperature2)
{
	double Enthalpy = 0;
	Enthalpy = coeff1 * powerup(temperature2, temperature1, 1) + coeff2 * powerup(temperature2, temperature1, 2) / 2 + coeff3 * powerup(temperature2, temperature1, 3) / 3 + coeff4 * powerup(temperature2, temperature1, 4) / 4 + coeff5 * powerup(temperature2, temperature1, 5) / 5;
	return Enthalpy;
}
//Calculates the entropy Evolved
inline double entropy_evolved(double coeff1, double coeff2, double coeff3, double coeff4, double coeff5, double temperature1, double temperature2)
{
	double Enthalpy = 0;
	Enthalpy = coeff1*log(temperature2/temperature1) + coeff2 * powerup(temperature2, temperature1, 1) / 1 + coeff3 * powerup(temperature2, temperature1, 2) / 2 + coeff4 * powerup(temperature2, temperature1, 3) / 3 + coeff5 * powerup(temperature2, temperature1, 4) / 4;
	return Enthalpy;
}


//Calcaulates the heat capacity of the specie
inline double heat_capacity(double coeff1, double coeff2, double coeff3, double coeff4, double coeff5, double temperature1)
{
	double HeatCapacity = 0;
	HeatCapacity = coeff1 + coeff2 * temperature1 + coeff3 * pow(temperature1, 2) + coeff4 * pow(temperature1, 3) + coeff5 * pow(temperature1, 4);
	return HeatCapacity;
}

//Calculates the viscosity for each specie
inline double viscosity_calculation(double coeff1, double coeff2, double coeff3, double coeff4, double temperature, STATE state)
{
	double Viscosity = 0;
	if (state == STATE::LIQUID)
	{
		Viscosity = pow(10, (coeff1 + coeff2 / temperature + coeff3 * temperature + coeff4 * pow(temperature, 2)));
	}
	else if (state == STATE::GAS)
	{
		Viscosity = coeff1 + coeff2 * temperature + coeff3 * pow(temperature, 2);
	}
	return Viscosity;
}
//appends the information passed into
inline std::string insert_info(std::stringstream &results, std::string &finalResult)
{
	std::string placeholder;
	while (std::getline(results, placeholder))
	{
		finalResult.append(placeholder);
		finalResult.append("\n");
	}
}

std::stringstream CalculateEnthalpy(std::string& species, float& temperature1, float& temperature2, float& pressure1, float& pressure2, bool& ShowPressureInclusion, std::fstream& file, std::string& line, std::string& word, std::string fname)
{
	//Opens the file for the table we choose to calculate from
	file.open(fname, std::ios::in);
	std::stringstream finalResult; //Stores the result in this stringstream

	//Empirical coefficients
	double coeff1 = 0;
	double coeff2 = 0;
	double coeff3 = 0;
	double coeff4 = 0;
	double coeff5 = 0;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::stringstream str(line); //Stores the line into the str variable
			while (std::getline(str, word, ',')) //Extracts the line
			{
				int i = 0;
				if (word == species) //Compare each string that is seperated by ',' and if it matches the "word" it will create string stream
				{
					std::stringstream newstr(line);
					std::string placeholder; //From stringstream we extract the coefficents that are seperated by ','
					while (std::getline(newstr, placeholder, ',')) {
						//stores the data obtained from the table file to a "coeff" variable
						switch (i)
						{
						case 2:
							coeff1 = std::stod(placeholder);
							break;
						case 3:
							coeff2 = std::stod(placeholder);
							break;
						case 4:
							coeff3 = std::stod(placeholder);
							break;
						case 5:
							coeff4 = std::stod(placeholder);
							break;
						case 6:
							coeff5 = std::stod(placeholder);
							break;
						}
						++i;
					}
				}
			}
		}
		//PRINTS TO CONSOLE FOR DEBUGGING PURPOSES
		std::cout << "Coeff1: " << coeff1 << " Coeff2: " << coeff2 << " Coeff3: " << coeff3 << " Coeff4: " << coeff4 << " Coeff5: " << coeff5 << std::endl;
		std::cout << "Enthalpy evolved is: " << enthalpy_evolved(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1, temperature2) << " J/Mol" << std::endl;
		std::cout << "Heat Capacity at (" << temperature1 << "): Cp = " << heat_capacity(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1) << std::endl;
		//Stores the results into the finalResult variable to later display it on our GUI
		finalResult << "Enthalpy change is: " << enthalpy_evolved(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1, temperature2) << " J/Mol" << std::endl;
		finalResult << "Entropy change is: " << entropy_evolved(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1, temperature2) - (ShowPressureInclusion ? 8.31446*(logf(pressure2) - logf(pressure1)) : 0) << " J/Mol*K" << std::endl;
		finalResult << "Heat Capacity at (" << temperature1 << "): Cp = " << heat_capacity(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1) << " J/Mol*K" << std::endl;
		finalResult << "Heat Capacity at (" << temperature2 << "): Cp = " << heat_capacity(coeff1, coeff2, coeff3, coeff4, coeff5, temperature2) << " J/Mol*K" << std::endl;
	}
	else
		finalResult << "Could not open file" << std::endl;

	
	file.close();
	return finalResult;
}




std::stringstream CalculateViscosity(std::string& species, float& temperature, std::fstream& file, std::string& line, std::string& word, std::string fname)
{
	//Opens the file for the table we choose to calculate from
	file.open(fname, std::ios::in);
	std::stringstream finalResult; //Stores the result in this stringstream
	//Empirical coefficents
	double coeff1 = 0;
	double coeff2 = 0;
	double coeff3 = 0;
	double coeff4 = 0;
	if (file.is_open())
	{
		while (std::getline(file, line)) //Stores the line into the str variable
		{
			std::stringstream str(line);
			while (std::getline(str, word, ','))
			{
				int i = 0;
				if (word == species) //Compare each string that is seperated by ',' and if it matches the "word" it will create string stream
				{
					std::stringstream newstr(line);
					std::string placeholder; //From stringstream we extract the coefficents that are seperated by ','
					while (std::getline(newstr, placeholder, ',')) {
						if (_contains(fname, "Liquids"))
						{
							//stores the data obtained from the table file to a "coeff" variable
							switch (i)
							{
							case 1:
								coeff1 = std::stod(placeholder);
								break;
							case 2:
								coeff2 = std::stod(placeholder);
								break;
							case 3:
								coeff3 = std::stod(placeholder);
								break;
							case 4:
								coeff4 = std::stod(placeholder);
								break;
							}
						}
						else
						{
							switch (i)
							{
							case 1:
								coeff1 = std::stod(placeholder);
								break;
							case 2:
								coeff2 = std::stod(placeholder);
								break;
							case 3:
								coeff3 = std::stod(placeholder);
								break;
							}
						}
						++i;
					}
				}
			}
		}
		//Stores all the results to finaResult for later GUI display
		if (!_contains(fname, "Liquids")) //Checks if the table is Liquid or Gas
		{
			finalResult << "Viscosity of " << species << " at (" << temperature << ") : " << viscosity_calculation(coeff1, coeff2, coeff3, coeff4, temperature, STATE::GAS) << "cP" << std::endl;
		}
		else
		{
			finalResult << "Viscosity of " << species << " at (" << temperature << "): " << viscosity_calculation(coeff1, coeff2, coeff3, coeff4, temperature, STATE::LIQUID) << " cP" << std::endl;
		}
	}
	else
		finalResult << "Could not open file" << std::endl;


	file.close();
	return finalResult;
}

#endif
