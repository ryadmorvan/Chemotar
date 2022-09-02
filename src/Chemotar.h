#pragma once

#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/imgui-SFML.h>
#include <thread>
#include <future>
//
//enum TABLES_SAVE_DATA
//{
//	
//};









class Chemotar
{
private:

	bool ShowPropertiesCalculator = false;
	bool ShowViscosityCalculator = false;
	bool ShowSteamTableCalculator = false;
	bool ShowIdealGasLaw = false;
	bool ShowBoiler = false;
	bool ShowBernoulli = false;
	bool ShowSettings = false;
	bool ShowDeveloperInfo = false;


	//AddTable pointer
public:
	Chemotar() : _AddTable(nullptr), _Settings(nullptr), heat_capacity_calculator(nullptr), ViscosityCalc(nullptr),
	SteamTableSimulation(nullptr), IdealGasLaw(nullptr), BoilerSimulation(nullptr), ShowInfo(nullptr) {
	}

	void (*_AddTable)(std::string&, int);
	//Function pointers to the simulations and tools
	void (*_Settings)(void);
	void (*heat_capacity_calculator)(bool&);
	void (*ViscosityCalc)(bool&);
	void (*SteamTableSimulation)(void);
	void (*IdealGasLaw)(bool*);
	void (*BoilerSimulation)(bool*);
	void (*BernoulliSim)(bool*);
	void (*ShowInfo)(ImFont*);

	//Main methods that will be used to run our program
	void run(ImFont* font);
	int MainMenu(ImFont* font, std::string file_path);


	bool& returnShowHeatCapacityCalculator() { return ShowPropertiesCalculator; }
	bool& returnShowViscosity() { return ShowViscosityCalculator; }
	bool& returnShowSteamTable() { return ShowSteamTableCalculator; }
	bool& returnShowIdealGas() { return ShowIdealGasLaw; }
	bool& returnShowBoiler() { return ShowBoiler; }
	bool& returnShowSettings() { return ShowSettings; }
	bool& returnShowDeveloper() { return ShowDeveloperInfo; }
};

