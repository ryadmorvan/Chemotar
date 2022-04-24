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
	bool CheckBox[3] = { 0, 0, 0 };
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16);
	ImFont* font2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18);
	ImGui::SFML::UpdateFontTexture();
	std::stringstream results;
	std::string finalResult = "";
	if (!image.loadFromFile("color.jpg"))
	{
		std::cout << "Error" << std::endl;
	}
	sf::Sprite backgroundtexture;
	backgroundtexture.setTexture(image);
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





		//Chemical Enthalpy Calculation Window
		ImGui::PushFont(font);
		ImGui::Begin("Chemical Enthalpy Calculation");
		if (CheckBoxUI(CheckBox, fname, file, species, MinTemp, MaxTemp))
		{
			ImGui::InputFloat("Input Inital Temperature", &temperature1);
			ImGui::InputFloat("Input Final Temperature", &temperature2);
			ImGui::SliderFloat("Inital Temperature", &temperature1, MinTemp, MaxTemp);
			ImGui::SliderFloat("Final Temperature", &temperature2, MinTemp, MaxTemp);

			if (ImGui::Button("Calculate"))
			{
				results = CalculateEnthalpy(species, temperature1, temperature2, file, line, word, fname);
				std::getline(results, finalResult);
			}
		}
			
		ImGui::Text(finalResult.c_str());
		ImGui::PopFont();
		ImGui::End();







		//Developer Window
		ImGui::PushFont(font2);
		ImGui::Begin("Developer");
		ImGui::Text("Designed and Developed By Riyadh Al-Khamees\n");
		ImGui::Text("Email: riyadhalkhamees@gmail.com\n");
		ImGui::Text("Github: https://github.com/ryadmorvan\n\n");
		ImGui::Text("Credits to: J. Richard Elliott, Carl T. Lira, \nYaws, C.L., J.R. Hopper, and R.W. Pike");
		ImGui::PopFont();
		ImGui::End();

		//window.clear(sf::Color::Color(176, 216, 30));
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	
	return 0;
}