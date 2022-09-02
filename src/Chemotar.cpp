#include "Chemotar.h"



int Chemotar::MainMenu(ImFont* font, std::string file_path)
{
	ImGui::PushFont(font);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Add Table Heat Capacity", NULL))
				{
					_AddTable(file_path, 2);
				}
				if (ImGui::MenuItem("Add Table Viscosity", NULL))
				{

					_AddTable(file_path, 1);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Exit", NULL))
			{
				ImGui::SFML::Shutdown();
				return 0;
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Heat Capacity Calculator", NULL, &ShowPropertiesCalculator))
			{

			}
			if (ImGui::MenuItem("Viscosity Calculator", NULL, &ShowViscosityCalculator))
			{

			}
			if (ImGui::MenuItem("Steam Table Calculator", NULL, &ShowSteamTableCalculator))
			{

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Simulations"))
		{
			if (ImGui::MenuItem("Ideal Gas Law Simulation", NULL, &ShowIdealGasLaw))
			{

			}
			if (ImGui::MenuItem("Boiler Simulation", NULL, &ShowBoiler))
			{

			}
			if(ImGui::MenuItem("Bernoulli Simulation", NULL, &ShowBernoulli))
			{
				
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Config"))
		{
			if (ImGui::MenuItem("Settings", NULL, &ShowSettings))
			{

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Developer"))
		{
			ImGui::MenuItem("Info", NULL, &ShowDeveloperInfo);
			//ImGui::MenuItem("Font Size", NULL, &\);
			ImGui::EndMenu();
		}
	}
	ImGui::PopFont();
}






void Chemotar::run(ImFont* font)
{
	if (ShowPropertiesCalculator)
	{
		ImGui::PushFont(font);
		ImGui::Begin("Heat Capacity Calculator", &ShowPropertiesCalculator);
		heat_capacity_calculator(ShowPropertiesCalculator);
		ImGui::PopFont();
		ImGui::End();
	}
	if(ShowViscosityCalculator)
	{
		ImGui::PushFont(font);
		ImGui::Begin("Viscosity Calculator", &ShowViscosityCalculator);
		ViscosityCalc(ShowViscosityCalculator);
		ImGui::PopFont();
		ImGui::End();
	}
	if (ShowSteamTableCalculator)
	{
		ImGui::PushFont(font);
		ImGui::Begin("Steam Table Calculator", &ShowSteamTableCalculator);
		SteamTableSimulation();
		ImGui::PopFont();
		ImGui::End();
	}
	if (ShowIdealGasLaw)
	{
		ImGui::PushFont(font);
		IdealGasLaw(&ShowIdealGasLaw);
		ImGui::PopFont();
	}
	if(ShowBoiler)
	{
		ImGui::PushFont(font);
		BoilerSimulation(&ShowBoiler);
		ImGui::PopFont();
	}
	if(ShowBernoulli)
	{
		ImGui::PushFont(font);
		BernoulliSim(&ShowBernoulli);
		ImGui::PopFont();
	}
	if(ShowDeveloperInfo)
	{
		ShowInfo(font);
	}
	if(ShowSettings)
	{
		ImGui::PushFont(font);
		ImGui::Begin("Settings", &ShowSettings);
		_Settings();
		ImGui::PopFont();
		ImGui::End();
	}
}