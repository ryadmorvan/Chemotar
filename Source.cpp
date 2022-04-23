#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "ChemicalCalculations.h"
#include "CheckBoxUI.h"

sf::Texture image;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Works");
	ImGui::SFML::Init(window);
	std::string fname = "NULL";
	std::string species;
	double temperature1 = 0;
	double temperature2 = 0;
	std::cout << "Type out the species name: ";
	std::cin >> species;
	std::cout << "Type out the species reference temperature: ";
	std::cin >> temperature1;
	std::cout << "Type out the spcies final temperature: ";
	std::cin >> temperature2;
	std::cout << std::endl;

	sf::Clock deltaClock;

	std::string line, word;

	std::fstream file;

	bool CheckBox[3] = { 0, 0, 0 };
	std::stringstream results;
	std::string finalResult = "NULL";
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

			ImGui::Begin("Chemical Enthalpy Calculation");
			CheckBoxUI(CheckBox, fname);








			if (ImGui::Button("Calculate"))
			{
				results = CalculateEnthalpy(species, temperature1, temperature2, file, line, word, fname);
				std::getline(results, finalResult);
			}






			ImGui::Text(finalResult.c_str());
			ImGui::End();

			//window.clear(sf::Color::Color(176, 216, 30));
			ImGui::SFML::Render(window);
			window.display();
		}

	ImGui::SFML::Shutdown();
	
	return 0;
}