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
#include "Graphics Simulation.h"



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
	bool ShowDensityCalculator = FALSE;
	bool ShowIdealGasLaw = FALSE;
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

		ImGui::PushFont(font2);
		if (ImGui::BeginMainMenuBar())
		{
			
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Add Table Enthalpy", NULL))
				{
					_AddTable(filePath, TABLES_SAVE_DATA::HEAT_CAPACITY);
				}
				if (ImGui::MenuItem("Add Table Density", NULL))
				{
					_AddTable(filePath, TABLES_SAVE_DATA::DENSITY);
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Chemical Properties Calculator", NULL, &ShowPropertiesCalculator))
				{

				}
				if (ImGui::MenuItem("Chemical Density Calculator", NULL, &ShowDensityCalculator))
				{

				}
				if (ImGui::MenuItem("Ideal Gas Law Simulation", NULL, &ShowIdealGasLaw))
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



		if (ShowDensityCalculator == TRUE)
		{
			ImGui::PushFont(font);
			ImGui::Begin("Density Calculation", &ShowDensityCalculator);
			DensityCalculator(ShowDensityCalculator);
			ImGui::PopFont();
			ImGui::End();
		}





		//Chemical Density Calculation Window
		//if (ShowDensityCalculator == TRUE)
		//{
		//	ImGui::PushFont(font);
		//	ImGui::Begin("Density Calculation", &ShowDensityCalculator);
		//	if (CheckBoxUI(CheckBox, fname, file, species, MinTemp, MaxTemp, CritTemp, filePath, TableName))
		//	{
		//		ImGui::InputFloat("Input Inital Temperature", &temperature1);
		//		ImGui::InputFloat("Input Final Temperature", &temperature2);
		//		ImGui::SliderFloat("Inital Temperature", &temperature1, MinTemp, MaxTemp);
		//		ImGui::SliderFloat("Final Temperature", &temperature2, MinTemp, MaxTemp);

		//		if (ImGui::Button("Calculate"))
		//		{
		//			finalResult.clear();
		//			results = CalculateEnthalpy(species, temperature1, temperature2, file, line, word, fname);
		//			insertInfo(results, finalResult);
		//		}
		//		if (ImGui::Button("Remove Table"))
		//		{
		//			memset(CheckBox, 0, sizeof(CheckBox));
		//			_Find_File_Delete(TableName);
		//		}
		//	}
		//	else
		//	{
		//		finalResult.clear(); species = "";
		//	}

		//	ImGui::Text(finalResult.c_str());
		//	ImGui::PopFont();
		//	ImGui::End();
		//}
		//else
		//{
		//	temperature1 = 0, temperature2 = 0, memset(CheckBox, FALSE, sizeof(CheckBox));
		//	finalResult = "";
		//}





		





		ImGui::PushFont(font);
		if (ShowIdealGasLaw == TRUE)
		{
			IdealGasLaw(&ShowIdealGasLaw);
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