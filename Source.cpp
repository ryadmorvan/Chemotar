#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/imgui-SFML.h>
#include "ChemicalCalculations.h"
#include "CheckBoxUI.h"

//Simulations
#include "Graphics Simulation.h"
#include "BoilerSimulation.h"


sf::Texture image;

std::string filePath = " ";

bool p_open = 0;


int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 768), "Chemical Properties Calculator");
	ImGui::SFML::Init(window);

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	sf::Clock deltaClock;
	float fontSize = 16;
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", fontSize);
	ImFont* font2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", fontSize);
	ImGui::SFML::UpdateFontTexture();

	sf::Texture propertiesImage;
	if (!propertiesImage.loadFromFile("properties.png"))
	{
		std::cout << "Error" << std::endl;
	}
	sf::Sprite propertiesIcon; propertiesIcon.setTexture(propertiesImage);
	


	if (!image.loadFromFile("Assets\\color.tga"))
	{
		std::cout << "Error" << std::endl;
	}
	sf::Sprite backgroundtexture; 	backgroundtexture.setTexture(image);

	


	//Flags of windows
	bool ShowDeveloperInfo = FALSE;
	bool ShowPropertiesCalculator = FALSE;
	bool ShowViscosityCalculator = FALSE;
	bool ShowIdealGasLaw = FALSE;
	bool ShowBoiler = FALSE;
	bool FontSettings = 0;
	window.setFramerateLimit(60);





	//Rectangle Properties
	////////////////////////////////////////



	while (window.isOpen())
	{
		sf::Event event;
		//window.draw(propertiesIcon);
		window.draw(backgroundtexture);
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}
		ImGui::SFML::Update(window, deltaClock.restart());

		//ImGui::SetMouseCursor(ImGuiMouseCursor_None); hide Cursor

		ImGui::PushFont(font2);
		if (ImGui::BeginMainMenuBar())
		{
			
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("Add"))
				{
					if (ImGui::MenuItem("Add Table Enthalpy", NULL))
					{
						_AddTable(filePath, TABLES_SAVE_DATA::HEAT_CAPACITY);
					}
					if (ImGui::MenuItem("Add Table Viscosity", NULL))
					{

						_AddTable(filePath, TABLES_SAVE_DATA::VISCOSITY);
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Exit", NULL))
				{
					return 0;
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Chemical Enthalpy Calculator", NULL, &ShowPropertiesCalculator))
				{

				}
				if (ImGui::MenuItem("Chemical Viscosity Calculator", NULL, &ShowViscosityCalculator))
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
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Developer"))
			{
				ImGui::MenuItem("Info", NULL, &ShowDeveloperInfo);
				//ImGui::MenuItem("Font Size", NULL, &FontSettings);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		ImGui::PopFont();
		//Chemical Enthalpy Calculation Window
		if (ShowPropertiesCalculator == TRUE)
		{
			ImGui::PushFont(font);
			ImGui::Begin("Enthalpy Calculation", &ShowPropertiesCalculator);
			EnthalpyCalculator(ShowPropertiesCalculator);
			ImGui::PopFont();
			ImGui::End();
		}



		if (ShowViscosityCalculator == TRUE)
		{
			ImGui::PushFont(font);
			ImGui::Begin("Viscosity Calculation", &ShowViscosityCalculator);
			ViscosityCalculator(ShowViscosityCalculator);
			ImGui::PopFont();
			ImGui::End();
		}


		//Simulations
		ImGui::PushFont(font);
		if (ShowIdealGasLaw == TRUE)
		{
			IdealGasLaw(&ShowIdealGasLaw);
		}

		ImGui::PopFont();

		ImGui::PushFont(font);
		if (ShowBoiler == TRUE)
		{
			BoilerSimulation(&ShowBoiler);
		}
		ImGui::PopFont();



		//Developer Window
		if (ShowDeveloperInfo)
		{
			ShowInfo(font2);
		}

		//window.clear(sf::Color::Color(176, 216, 30));
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	
	return 0;
}