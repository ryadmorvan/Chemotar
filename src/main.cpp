/* Project: Chemotar
 * Author: Ryad M
 * Contact: riyadhalkhamees@gmail.com
 *
 * MIT License

Copyright (c) 2022 Ryad M

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
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
#include "../src/Tools/ChemicalCalculations.h"
#include "../src/Tools/Empirical Calculators.h"
#include <thread>






//Simulations
#include "../src/Simulations/Ideal_Gas_Simulation.h"
#include "../src/Simulations/BoilerSimulation.h"
#include "../src/Tools/SteamTable.h"
#include "../src/Chemotar.h"



void ShowInfo(ImFont* font2);




//void HideConsole()
//{
//	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
//}

//#define _CRT_SECURE_NO_WARNINGS







int main()
{

	//HideConsole();
	//Using 1366 by 768 just to simulate the default laptop's resolution
	//SETUP////////////////////////////////////////////////////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(1366, 768), "Chemotar");
	ImGui::SFML::Init(window);
	//Enable docking
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	static sf::Texture image; static std::string file_path = " ";
	//Docking system
	sf::Clock deltaClock;
	//Loading up the fonts
	float fontSize = 16;
	ImGuiIO& io1 = ImGui::GetIO();
	ImFont* font = io1.Fonts->AddFontFromFileTTF("Assets\\Arial.ttf", fontSize);
	io1.FontDefault = font;
	ImFont* font2 = io1.Fonts->AddFontFromFileTTF("Assets\\Arial.ttf", 17);
	ImGui::SFML::UpdateFontTexture();
	//Loading our background picture
	if (!image.loadFromFile("Assets\\color.tga")) { std::cout << "Error loading background" << std::endl; }
	sf::Sprite backgroundtexture; 	backgroundtexture.setTexture(image);
	window.setFramerateLimit(60);
	SetupImGuiStyle();
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////

	//Creating our program AND linking our functions
	Chemotar chemotar;
	chemotar._Settings = &_Settings;	chemotar.heat_capacity_calculator = &heat_capacity_calculator;
	chemotar.ViscosityCalc = &ViscosityCalculator;	chemotar.SteamTableSimulation = &SteamTableSimulation;
	chemotar.IdealGasLaw = &IdealGasLaw;	chemotar.BoilerSimulation = &BoilerSimulation;
	chemotar.ShowInfo = &ShowInfo;	chemotar._AddTable = &_AddTable;
	


;

	////////////////////////////////////////
	////////////////////////////////////////

	while (window.isOpen())
	{
		bool *p_open = FALSE;
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

		ImGui::ShowDemoWindow();
		//Flags that will be used for docking
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Main DockSpace", p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		static bool once = []() {
				dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				return true;
		} ();
		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			std::cout << "Error Implementing Dockspace Changed" << std::endl;
		}






		chemotar.MainMenu(font2, file_path); //Displays the main menu
		chemotar.run(font); //Simulates all the tools and calculators
		//ImGui::SetMouseCursor(ImGuiMouseCursor_None); hide Cursor
		//Run the program and push the font into it


		//window.clear(sf::Color::Color(176, 216, 30));
		ImGui::SFML::Render(window);
		window.display();
	}


	ImGui::SFML::Shutdown();

	
	return 0;
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