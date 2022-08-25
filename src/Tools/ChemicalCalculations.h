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

double powerup(double temperature2, double temperature1, double power)
{
	return pow(temperature2, power) - pow(temperature1, power);
}

//Calculates the enthalpy evolved
double EnthalpyEvolved(double coeff1, double coeff2, double coeff3, double coeff4, double coeff5, double temperature1, double temperature2)
{
	double Enthalpy = 0;
	Enthalpy = coeff1 * powerup(temperature2, temperature1, 1) + coeff2 * powerup(temperature2, temperature1, 2) / 2 + coeff3 * powerup(temperature2, temperature1, 3) / 3 + coeff4 * powerup(temperature2, temperature1, 4) / 4 + coeff5 * powerup(temperature2, temperature1, 5) / 5;
	return Enthalpy;
}

double EntropyEvolved(double coeff1, double coeff2, double coeff3, double coeff4, double coeff5, double temperature1, double temperature2)
{
	double Enthalpy = 0;
	Enthalpy = coeff1*log(temperature2/temperature1) + coeff2 * powerup(temperature2, temperature1, 1) / 1 + coeff3 * powerup(temperature2, temperature1, 2) / 2 + coeff4 * powerup(temperature2, temperature1, 3) / 3 + coeff5 * powerup(temperature2, temperature1, 4) / 4;
	return Enthalpy;
}



double HeatCapacity(double coeff1, double coeff2, double coeff3, double coeff4, double coeff5, double temperature1)
{
	double HeatCapacity = 0;
	HeatCapacity = coeff1 + coeff2 * temperature1 + coeff3 * pow(temperature1, 2) + coeff4 * pow(temperature1, 3) + coeff5 * pow(temperature1, 4);
	return HeatCapacity;
}


double _ViscosityCalculation(double coeff1, double coeff2, double coeff3, double coeff4, double temperature, STATE state)
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

std::stringstream CalculateEnthalpy(std::string& species, float& temperature1, float& temperature2, std::fstream& file, std::string& line, std::string& word, std::string fname)
{
	file.open(fname, std::ios::in);
	std::stringstream finalResult;
	double coeff1 = 0;
	double coeff2 = 0;
	double coeff3 = 0;
	double coeff4 = 0;
	double coeff5 = 0;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::stringstream str(line);
			//std::getline(str, word);
			//std::cout << word << std::endl;
			while (std::getline(str, word, ','))
			{
				//if (word == "CO2")
				//{
				//	std::stringstream newstr(line);
				//	std::string sent;
				//	std::getline(newstr, sent);
				//	std::cout << sent << std::endl;
				//}
				int i = 0;
				if (word == species)
				{
					std::stringstream newstr(line);
					//if (i == 2)
					//{
					//	double coeff1 = std::stod(word);
					//	std::cout << coeff1 << "'";
					//}
					std::string placeholder;
					while (std::getline(newstr, placeholder, ',')) {
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
		std::cout << "Coeff1: " << coeff1 << " Coeff2: " << coeff2 << " Coeff3: " << coeff3 << " Coeff4: " << coeff4 << " Coeff5: " << coeff5 << std::endl;
		std::cout << "Enthalpy evolved is: " << EnthalpyEvolved(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1, temperature2) << " J/Mol" << std::endl;
		std::cout << "Heat Capacity at (" << temperature1 << "): Cp = " << HeatCapacity(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1) << std::endl;
		finalResult << "Enthalpy change is: " << EnthalpyEvolved(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1, temperature2) << " J/Mol" << std::endl;
		finalResult << "Entropy change is: " << EntropyEvolved(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1, temperature2) << " J/Mol*K" << std::endl;
		finalResult << "Heat Capacity at (" << temperature1 << "): Cp = " << HeatCapacity(coeff1, coeff2, coeff3, coeff4, coeff5, temperature1) << " J/Mol*K" << std::endl;
		finalResult << "Heat Capacity at (" << temperature2 << "): Cp = " << HeatCapacity(coeff1, coeff2, coeff3, coeff4, coeff5, temperature2) << " J/Mol*K" << std::endl;
	}
	else
		finalResult << "Could not open file" << std::endl;

	
	file.close();
	return finalResult;
}




std::stringstream CalculateViscosity(std::string& species, float& temperature, std::fstream& file, std::string& line, std::string& word, std::string fname)
{
	file.open(fname, std::ios::in);
	std::stringstream finalResult;
	double coeff1 = 0;
	double coeff2 = 0;
	double coeff3 = 0;
	double coeff4 = 0;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::stringstream str(line);
			//std::getline(str, word);
			//std::cout << word << std::endl;
			while (std::getline(str, word, ','))
			{
				//if (word == "CO2")
				//{
				//	std::stringstream newstr(line);
				//	std::string sent;
				//	std::getline(newstr, sent);
				//	std::cout << sent << std::endl;
				//}
				int i = 0;
				if (word == species)
				{
					std::stringstream newstr(line);
					//if (i == 2)
					//{
					//	double coeff1 = std::stod(word);
					//	std::cout << coeff1 << "'";
					//}
					std::string placeholder;
					while (std::getline(newstr, placeholder, ',')) {
						if (_contains(fname, "Liquids"))
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

		if (!_contains(fname, "Liquids"))
		{
			finalResult << "Viscosity of " << species << " at (" << temperature << ") : " << _ViscosityCalculation(coeff1, coeff2, coeff3, coeff4, temperature, STATE::GAS) << "cP" << std::endl;
		}
		else
		{
			finalResult << "Viscosity of " << species << " at (" << temperature << "): " << _ViscosityCalculation(coeff1, coeff2, coeff3, coeff4, temperature, STATE::LIQUID) << " cP" << std::endl;
		}
	}
	else
		finalResult << "Could not open file" << std::endl;


	file.close();
	return finalResult;
}





void ShowInfo(ImFont* font2)
{
	ImGui::PushFont(font2);
	ImGui::Begin("Developer");
	ImGui::Text("Designed and Developed By Riyadh Al-Khamees\n");
	ImGui::Text("Email: riyadhalkhamees@gmail.com\n");
	ImGui::Text("Github: https://github.com/ryadmorvan");
	if (ImGui::Button("Github Repo")) { OsOpenInShell("https://github.com/ryadmorvan/Chemotar"); }
	ImGui::Text("\nCredits to: J. Richard Elliott, Carl T. Lira, \nYaws, C.L., J.R. Hopper, and R.W. Pike\nNeil Hendren from University of Colorado Boulder");
	ImGui::End();
	ImGui::PopFont();
}

#endif
