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

#include <Windows.h>

//Simulations
#include "Graphics Simulation.h"
#include "BoilerSimulation.h"
#include "SteamTable.h"


sf::Texture image;

std::string filePath = " ";

bool p_open = 0;

//void HideConsole()
//{
//	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
//}

#define _CRT_SECURE_NO_WARNINGS







int main()
{
	//HideConsole();
	sf::RenderWindow window(sf::VideoMode(1366, 768), "Chemical Properties Calculator");
	ImGui::SFML::Init(window);

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//Docking system
	sf::Clock deltaClock;
	float fontSize = 16;
	ImGuiIO& io1 = ImGui::GetIO();
	ImFont* font = io1.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", fontSize);
	io1.FontDefault = font;

	ImFont* font2 = io1.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 17);
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
	bool ShowSteamTableCalculator = FALSE;
	bool ShowViscosityCalculator = FALSE;
	bool ShowIdealGasLaw = FALSE;
	bool ShowBoiler = FALSE;
	bool FontSettings = 0;
	window.setFramerateLimit(60);
	//Flags for settings
	bool ShowSettings = FALSE;
	bool *p_open = FALSE;

	SetupImGuiStyle();

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

		ImGui::ShowDemoWindow();

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

		ImGui::PushFont(font2);
		if (ImGui::BeginMenuBar())
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
					ImGui::SFML::Shutdown();
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
				//ImGui::MenuItem("Font Size", NULL, &FontSettings);
				ImGui::EndMenu();
			}
		}
		ImGui::PopFont();


		//ImGui::SetMouseCursor(ImGuiMouseCursor_None); hide Cursor


		//if (ImGui::BeginMainMenuBar())
		//{

		//	if (ImGui::BeginMenu("File"))
		//	{
		//		if (ImGui::BeginMenu("Add"))
		//		{
		//			if (ImGui::MenuItem("Add Table Enthalpy", NULL))
		//			{
		//				_AddTable(filePath, TABLES_SAVE_DATA::HEAT_CAPACITY);
		//			}
		//			if (ImGui::MenuItem("Add Table Viscosity", NULL))
		//			{

		//				_AddTable(filePath, TABLES_SAVE_DATA::VISCOSITY);
		//			}
		//			ImGui::EndMenu();
		//		}
		//		if (ImGui::MenuItem("Exit", NULL))
		//		{
		//			ImGui::SFML::Shutdown();
		//			return 0;
		//		}
		//		ImGui::EndMenu();

		//	}
		//	if (ImGui::BeginMenu("View"))
		//	{
		//		if (ImGui::MenuItem("Chemical Enthalpy Calculator", NULL, &ShowPropertiesCalculator))
		//		{

		//		}
		//		if (ImGui::MenuItem("Chemical Viscosity Calculator", NULL, &ShowViscosityCalculator))
		//		{

		//		}
		//		if (ImGui::MenuItem("Steam Table Calculator", NULL, &ShowSteamTableCalculator))
		//		{
		//			
		//		}
		//		ImGui::EndMenu();
		//	}
		//	if (ImGui::BeginMenu("Simulations"))
		//	{
		//		if (ImGui::MenuItem("Ideal Gas Law Simulation", NULL, &ShowIdealGasLaw))
		//		{

		//		}
		//		if (ImGui::MenuItem("Boiler Simulation", NULL, &ShowBoiler))
		//		{

		//		}
		//		ImGui::EndMenu();
		//	}
		//	if (ImGui::BeginMenu("Config"))
		//	{
		//		if (ImGui::MenuItem("Settings", NULL, &ShowSettings))
		//		{

		//		}
		//		ImGui::EndMenu();
		//	}

		//	if (ImGui::BeginMenu("Developer"))
		//	{
		//		ImGui::MenuItem("Info", NULL, &ShowDeveloperInfo);
		//		//ImGui::MenuItem("Font Size", NULL, &FontSettings);
		//		ImGui::EndMenu();
		//	}
		//	ImGui::EndMainMenuBar();
		//}
		//ImGui::PopFont();
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

		if (ShowSteamTableCalculator)
		{
			ImGui::PushFont(font);
			ImGui::Begin("Steam Table Calculator", &ShowSteamTableCalculator);
			SteamTableSimulation();
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

		//Show Settings
		if (ShowSettings)
		{
			ImGui::PushFont(font);
			ImGui::Begin("Settings", &ShowSettings);
			_Settings();
			ImGui::PopFont();
			ImGui::End();
		}

		//window.clear(sf::Color::Color(176, 216, 30));
		ImGui::SFML::Render(window);
		window.display();
	}


	ImGui::SFML::Shutdown();

	
	return 0;
}