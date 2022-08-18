#pragma once
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
#include "../src/Simulations/Boiler.h"
#include "../src/Utility/Windows_FileHandling.h"
#include "../src/Graph/Graph.h"




static void IdealGasLaw(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
    static Piston::process current_state;
    if (!ImGui::Begin("Ideal Gas Law", p_open))
    {
        current_state = Piston::process::Non;
        ImGui::End();
        return;
    }

    Piston piston(Piston::species::Air);

    // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc. 
    // Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4. 
    // ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types) 
    // In this example we are not using the maths operators! 
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Primitives 
    ImGui::Text("States");
    static float sz = 220.0f;
    static float thickness = 6.0f;
    static float HeatEvolved = 0.0f;
    static float SurroundingPressure = 1;
    static float HeatColor = 0;
    static float ColdColor = 0;
    static ImVec4 col = ImVec4(0.9f, 0.9f, 0.9f, 0.8f);
    static ImVec4 board_col = ImVec4(0.506f, 0.404f, 0.592f, 1.0f);
    ImVec4 Air_col = ImVec4(0.902f, 0.902f + HeatColor, 0.98f + HeatColor, 0.15f);
    static ImVec4 Text_col = ImVec4(0.1f, 0.9f, 0.3f, 0.85f);
    ImVec4 Heat_col = ImVec4(0.4157f + HeatEvolved/55.0f, 0.333, 0.3411f, 0.95);
    ImVec4 Cold_col = ImVec4(0.4157f + ColdColor, 0.333f - ColdColor, 0.3411f - ColdColor, 0.95);
    static ImVec4 Stopper_col = ImVec4(0.302f, 0.431f, 0.733f, 0.95);
    static ImVec4 Insulator_col = ImVec4(0.616f, 0.765f, 0.855f, 0.85);
    static ImVec4 Work_col = ImVec4(0.929f, 0.875f, 0.702f, 0.85);
    //ImGui::DragFloat("Size", &sz, 0.2f, 2.0f, 452.0f, "%.0f");
    //ImGui::DragFloat("Thickness", &thickness, 0.05f, 1.0f, 15.0f, "%.02f");
    static float raise = 0;




    static const char* current_process = "Choose Process";
    if (ImGui::BeginCombo("Process", current_process))
    {
        if (ImGui::Selectable("Constant Pressure", true))
        {
            current_process = "Constant Pressure";
            current_state = Piston::process::constant_pressure;

        }
        if (ImGui::Selectable("Constant Volume", true))
        {
            current_process = "Constant Volume";
            current_state = Piston::process::constant_volume;
        }
        if (ImGui::Selectable("Constant Temperature", true))
        {
            current_process = "Constant Temperature";
            current_state = Piston::process::constant_temperature;
        }
        if (ImGui::Selectable("Constant Heat", true))
        {
            current_process = "Constant Heat";
            current_state = Piston::process::constant_heat;
        }
    }
    if ((current_state == Piston::process::constant_pressure) or (current_state == Piston::process::constant_volume))
    {
        //ImGui::DragFloat("Heat in KJ", &HeatEvolved, 0.05f, 0, 30);
        ImGui::SliderFloat("Heat in KJ", &HeatEvolved, 0, 30);
    }
    else if((current_state == Piston::process::constant_temperature) or (current_state == Piston::process::constant_heat))
    {
        //ImGui::DragFloat("Pressure in atm", &SurroundingPressure, 0.01f, 1, 5);
        ImGui::SliderFloat("Pressure in atm", &SurroundingPressure, 1, 5);
    }
    HeatColor = 0;
    //Constant Pressure
    if (current_state == Piston::process::constant_pressure)
    {


        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImU32 col32 = ImColor(col);
        const ImU32 board_col32 = ImColor(board_col);
        const ImU32 Air_col32 = ImColor(Air_col);
        const ImU32 Text_col32 = ImColor(Text_col);
        ImU32 Heat_col32 = ImColor(Heat_col);
        ImU32 Work_col32 = ImColor(Work_col);
        float x = p.x + 100.0f, y = p.y + 200.0f, spacing = 15.0f;
        float x_board = p.x + 5, y_board = p.y + 20, board_spacing = 15.0f;
        Graph graph(x + 370, y + 300, x + 720, y - 50, 2);
        // First line uses a thickness of 1.0, second line uses the configurable thickness 
        piston.AddHeat(HeatEvolved * 1000, Piston::process::constant_pressure);
        raise = piston.returnChangeInPistonHeight();
        float th = thickness;
        draw_list->AddRect(ImVec2(x, y - 100), ImVec2(x + sz + 100, y + sz + 190), col32, 10.0f, ImDrawCornerFlags_All, th);
        draw_list->AddRect(ImVec2(x_board, y_board), ImVec2(x + sz + 530, y + sz + 200), board_col32, 10.0f, ImDrawCornerFlags_All, 4.0f);
        draw_list->AddLine(ImVec2(x + th - 2.0f, y + sz - raise + 100), ImVec2(x + sz - th + 2.0f + 100, y + sz - raise + 100), board_col32, 10);
        draw_list->AddRectFilled(ImVec2(x + th - 2.0f, y + sz + 6 - raise + 100), ImVec2(x + sz - th + 3.0f + 100, y + sz + 40 + 153), Air_col32, 0);
        graph.Draw(draw_list, Text_col32);
        graph.BuildLinesVector(piston, Graph::state::constant_pressure);
        graph.DrawLines(draw_list, piston);


        draw_list->AddText(ImVec2(x + th + 10, y + sz + 20 + 100), Text_col32, piston.returnVolumeText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 20 + 100), Text_col32, piston.returnPressureText().c_str());
        draw_list->AddText(ImVec2(x + th + 10, y + sz + 40 + 100), Text_col32, piston.returnTemperatureText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 40 + 100), Text_col32, piston.returnMolesText().c_str());
        draw_list->AddText(ImVec2(x + th + 60, y + sz + 70 - raise), Text_col32, "Surrounding Pressure = 1.0 atm");
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 100), Heat_col32, piston.returnHeatText().c_str());
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 80), Work_col32, piston.returnWorkText().c_str());


        x = p.x + 4;
        y += sz + spacing;
        //ImGui::Dummy(ImVec2((sz + spacing) * 8, (sz + spacing) * 3)); //Expand to available space
        return;
    }
    else if (current_state == Piston::process::constant_volume) //Constant Volume
    {
        raise = 0;
        HeatColor = - HeatEvolved / 55.0f;
        piston.AddHeat(HeatEvolved * 1000, Piston::process::constant_volume);
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImU32 col32 = ImColor(col);
        const ImU32 board_col32 = ImColor(board_col);
        const ImU32 Air_col32 = ImColor(Air_col);
        const ImU32 Text_col32 = ImColor(Text_col);
        ImU32 Heat_col32 = ImColor(Heat_col);
        const ImU32 Stopper_col32 = ImColor(Stopper_col);
        ImU32 Work_col32 = ImColor(Work_col);
        float x = p.x + 100.0f, y = p.y + 200.0f, spacing = 15.0f;
        float x_board = p.x + 5, y_board = p.y + 20, board_spacing = 15.0f;
        Graph graph(x + 370, y + 300, x + 720, y - 50, 2); //Create Graph
        // First line uses a thickness of 1.0, second line uses the configurable thickness 
        float th = thickness;
        draw_list->AddRect(ImVec2(x, y - 100), ImVec2(x + sz + 100, y + sz + 190), col32, 10.0f, ImDrawCornerFlags_All, th);
        draw_list->AddRect(ImVec2(x_board, y_board), ImVec2(x + sz + 530, y + sz + 200), board_col32, 10.0f, ImDrawCornerFlags_All, 4.0f);
        draw_list->AddLine(ImVec2(x + th - 2.0f, y + sz - raise + 100 ), ImVec2(x + sz - th + 2.0f + 100 + 1, y + sz - raise + 100), board_col32, 10);
        draw_list->AddRectFilled(ImVec2(x + th - 2.0f, y + sz + 6 - raise + 100), ImVec2(x + sz - th + 3.0f + 100, y + sz + 40 + 153), Air_col32, 0);
        graph.Draw(draw_list, Text_col32);
        graph.BuildLinesVector(piston, Graph::state::constant_volume);
        graph.DrawLines(draw_list, piston);
        draw_list->AddText(ImVec2(x + th + 10, y + sz + 20 + 100), Text_col32, piston.returnVolumeText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 20 + 100), Text_col32, piston.returnPressureText().c_str());
        draw_list->AddText(ImVec2(x + th + 10, y + sz + 40 + 100), Text_col32, piston.returnTemperatureText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 40 + 100), Text_col32, piston.returnMolesText().c_str());
        draw_list->AddText(ImVec2(x + th + 60, y + sz + 70 - raise), Text_col32, "Surrounding Pressure = 1.0 atm");
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 100), Heat_col32, piston.returnHeatText().c_str());
        draw_list->AddRectFilled(ImVec2(x + 40 , y + sz + 95 ), ImVec2(x + 3 , y + sz + 70 ), Stopper_col32, 0);
        draw_list->AddRectFilled(ImVec2(x + 60 + sz, y + sz + 95), ImVec2(x + sz + 97, y + sz + 70), Stopper_col32, 0);
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 80), Work_col32, piston.returnWorkText().c_str());

        x = p.x + 4;
        y += sz + spacing;
        return;
    }
    else if (current_state == Piston::process::constant_temperature)
    {

        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImU32 col32 = ImColor(col);
        const ImU32 board_col32 = ImColor(board_col);
        const ImU32 Air_col32 = ImColor(Air_col);
        const ImU32 Text_col32 = ImColor(Text_col);
        ImU32 Heat_col32 = ImColor(Heat_col);
        ImU32 Cold_col32 = ImColor(Cold_col);
        ImU32 Work_col32 = ImColor(Work_col);
        float x = p.x + 100.0f, y = p.y + 200.0f, spacing = 15.0f;
        float x_board = p.x + 5, y_board = p.y + 20, board_spacing = 15.0f;
        Graph graph(x + 370, y + 300, x + 720, y - 50, 2); //Create Graph

        //change color depending on heat evolved

        piston.MolesChange(4.5, Piston::process::constant_temperature);
        piston.PressureChange(SurroundingPressure, Piston::process::constant_temperature);
        raise = piston.returnChangeInPistonHeight();

        ColdColor = piston.returnHeat() / 55.0f;
        // First line uses a thickness of 1.0, second line uses the configurable thickness 
        float th = thickness;
        draw_list->AddRect(ImVec2(x, y - 100), ImVec2(x + sz + 100, y + sz + 190), col32, 10.0f, ImDrawCornerFlags_All, th);
        draw_list->AddRect(ImVec2(x_board, y_board), ImVec2(x + sz + 530, y + sz + 200), board_col32, 10.0f, ImDrawCornerFlags_All, 4.0f);
        draw_list->AddLine(ImVec2(x + th - 2.0f, y + sz - raise + 100), ImVec2(x + sz - th + 2.0f + 100, y + sz - raise + 100), board_col32, 10);
        draw_list->AddRectFilled(ImVec2(x + th - 2.0f, y + sz + 6 - raise + 100), ImVec2(x + sz - th + 3.0f + 100, y + sz + 40 + 153), Air_col32, 0);
        graph.Draw(draw_list, Text_col32);
        graph.BuildLinesVector(piston, Graph::state::constant_temperature);
        graph.DrawLines(draw_list, piston);

        draw_list->AddText(ImVec2(x + th + 10, y + sz + 20 + 100), Text_col32, piston.returnVolumeText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 20 + 100), Text_col32, piston.returnPressureText().c_str());
        draw_list->AddText(ImVec2(x + th + 10, y + sz + 40 + 100), Text_col32, piston.returnTemperatureText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 40 + 100), Text_col32, piston.returnMolesText().c_str());
        std::string PressureSurrounding = "Surrounding Pressure = " + _Format(SurroundingPressure, 3) + " atm";
        draw_list->AddText(ImVec2(x + th + 55, y + sz + 70 - raise), Text_col32, PressureSurrounding.c_str());
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 100), Cold_col32, piston.returnHeatText().c_str());
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 80), Work_col32, piston.returnWorkText().c_str());
        return;
    }
    else if (current_state == Piston::process::constant_heat)
    {
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImU32 col32 = ImColor(col);
        const ImU32 board_col32 = ImColor(board_col);
        const ImU32 Air_col32 = ImColor(Air_col);
        const ImU32 Text_col32 = ImColor(Text_col);
        ImU32 Heat_col32 = ImColor(Heat_col);
        ImU32 Insulator_col32 = ImColor(Insulator_col);
        ImU32 Work_col32 = ImColor(Work_col);
        float x = p.x + 100.0f, y = p.y + 200.0f, spacing = 15.0f;
        float x_board = p.x + 5, y_board = p.y + 20, board_spacing = 15.0f;
        Graph graph(x + 370, y + 300, x + 720, y - 50, 2); //Create Graph

        piston.MolesChange(4.5, Piston::process::constant_temperature);
        piston.PressureChange(SurroundingPressure, Piston::process::constant_heat);
        raise = piston.returnChangeInPistonHeight();

        graph.Draw(draw_list, Text_col32);
        graph.BuildLinesVector(piston, Graph::state::constant_heat);
        graph.DrawLines(draw_list, piston);
        // First line uses a thickness of 1.0, second line uses the configurable thickness 
        float th = thickness;
        draw_list->AddRect(ImVec2(x, y - 100), ImVec2(x + sz + 100, y + sz + 190), col32, 10.0f, ImDrawCornerFlags_All, th);
        draw_list->AddRect(ImVec2(x_board, y_board), ImVec2(x + sz + 530, y + sz + 200), board_col32, 10.0f, ImDrawCornerFlags_All, 4.0f);
        draw_list->AddLine(ImVec2(x + th - 2.0f, y + sz - raise + 100), ImVec2(x + sz - th + 2.0f + 100, y + sz - raise + 100), board_col32, 10);
        draw_list->AddRectFilled(ImVec2(x + th - 2.0f, y + sz + 6 - raise + 100), ImVec2(x + sz - th + 3.0f + 100, y + sz + 40 + 153), Air_col32, 0);
        draw_list->AddLine(ImVec2(x - 20, y + 170), ImVec2(x - th + 4, y + 170), Insulator_col32, 450);
        draw_list->AddLine(ImVec2(x - 20 + sz + 122, y + 170), ImVec2(x - th + 2 + sz + 123, y + 170), Insulator_col32, 450);

        draw_list->AddText(ImVec2(x + th + 10, y + sz + 20 + 100), Text_col32, piston.returnVolumeText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 20 + 100), Text_col32, piston.returnPressureText().c_str());
        draw_list->AddText(ImVec2(x + th + 10, y + sz + 40 + 100), Text_col32, piston.returnTemperatureText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 40 + 100), Text_col32, piston.returnMolesText().c_str());
        std::string PressureSurrounding = "Surrounding Pressure = " + _Format(SurroundingPressure, 3) + " atm";
        draw_list->AddText(ImVec2(x + th + 55, y + sz + 70 - raise), Text_col32, PressureSurrounding.c_str());
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 100), Heat_col32, piston.returnHeatText().c_str());
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 60), Insulator_col32, "Heat Insulator Added");
        draw_list->AddText(ImVec2(x + th + 350, y + sz + 40 + 80), Work_col32, piston.returnWorkText().c_str());
        return;
    }
    else
    {
        HeatEvolved = 0;
        SurroundingPressure = 1;
    }

    ImGui::End();
}




