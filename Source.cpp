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
#include "ChemicalCalculations.h"
#include "CheckBoxUI.h"
#include "Graphics Simulation.h"


sf::Texture image;

std::string filePath = " ";

bool p_open = true;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 768), "Chemical Properties Calculator");
	ImGui::SFML::Init(window);
	std::string fname = "";
	std::string species;
	float temperature1 = 0;
	float temperature2 = 0;
	float MinTemp = 0;
	float MaxTemp = 0;
	sf::Clock deltaClock;
	std::string line, word;
	std::fstream file;
	bool tableLoaded = 0;
	bool CheckBox[150];
	memset(CheckBox, 0, sizeof(CheckBox));
	float fontSize = 16;
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", fontSize);
	ImFont* font2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", fontSize);
	ImGui::SFML::UpdateFontTexture();
	std::stringstream results;
	std::string finalResult = "";
	if (!image.loadFromFile("Assets\\color.tga"))
	{
		std::cout << "Error" << std::endl;
	}
	sf::Sprite backgroundtexture;
	backgroundtexture.setTexture(image); 


	//Flags of windows
	bool ShowDeveloperInfo = FALSE;
	bool ShowPropertiesCalculator = FALSE;
	bool FontSettings = 0;
	window.setFramerateLimit(60);





	//Rectangle Properties
	////////////////////////////////////////



	while (window.isOpen())
	{
		sf::Event event;
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
				if (ImGui::MenuItem("Add Table", NULL))
				{
					_AddTable(filePath);
				}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Chemical Properties Calculator", NULL, &ShowPropertiesCalculator))
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
			ImGui::Begin("Physical Properties Calculation", &ShowPropertiesCalculator);
			if (CheckBoxUI(CheckBox, fname, file, species, MinTemp, MaxTemp, filePath))
			{
				ImGui::InputFloat("Input Inital Temperature", &temperature1);
				ImGui::InputFloat("Input Final Temperature", &temperature2);
				ImGui::SliderFloat("Inital Temperature", &temperature1, MinTemp, MaxTemp);
				ImGui::SliderFloat("Final Temperature", &temperature2, MinTemp, MaxTemp);

				if (ImGui::Button("Calculate"))
				{
					finalResult.clear();
					results = CalculateEnthalpy(species, temperature1, temperature2, file, line, word, fname);
					insertInfo(results, finalResult);
				}
			}
			else
				finalResult.clear();

			ImGui::Text(finalResult.c_str());
			ImGui::PopFont();
			ImGui::End();
		}
		





		ImGui::PushFont(font);
		ShowExampleAppCustomRendering(&p_open);
		ImGui::PopFont();

		//if (FontSettings)
		//{
		//	ImGui::PushFont(font2);
		//	ImGui::Begin("Settings");
		//	ImGui::InputFloat("Input the size of the font", &fontSize);
		//	ImGui::SliderFloat("Font Size:", &fontSize, 16, 32);
		//	font->FontSize = fontSize;
		//	font2->FontSize = fontSize;
		//	ImGui::PopFont();
		//}

		//ImGui::Begin("Chemical Properties Calculation");
		//ImGui::Checkbox("Elliot Liquid", &CheckBox[0]);
		//ImGui::Checkbox("Gas Enthalpy", &CheckBox[1]);
		//ImGui::Checkbox("Liquid Enthalpy", &CheckBox[2]);
		//ImGui::End();

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