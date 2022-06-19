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

sf::Texture image;

std::string filePath = " ";

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Chemical Properties Calculator");
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
	bool CheckBox[50];
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
	bool ShowDeveloperInfo = 1;
	bool FontSettings = 0;
	window.setFramerateLimit(60);
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
			if (ImGui::BeginMenu("Developer"))
			{
				ImGui::MenuItem("Info", NULL, &ShowDeveloperInfo);
				//ImGui::MenuItem("Font Size", NULL, &FontSettings);
			}
			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Table", NULL))
				{
					_AddTable(filePath);
				}

			}
		}
		ImGui::PopFont();
		//Chemical Enthalpy Calculation Window
		ImGui::PushFont(font);
		ImGui::Begin("Physical Properties Calculation");
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