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
#include "Piston.h"




static void ShowExampleAppCustomRendering(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Ideal Gas Law", p_open))
    {
        ImGui::End();
        return;
    }

    Piston piston;

    // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc. 
    // Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4. 
    // ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types) 
    // In this example we are not using the maths operators! 
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Primitives 
    ImGui::Text("States");
    static float sz = 220.0f;
    static float thickness = 6.0f;
    static ImVec4 col = ImVec4(0.9f, 0.9f, 0.9f, 0.8f);
    static ImVec4 board_col = ImVec4(0.506f, 0.404f, 0.592f, 1.0f);
    static ImVec4 Air_col = ImVec4(0.902f, 0.902f, 0.98f, 0.15f);
    static ImVec4 Text_col = ImVec4(0.1f, 0.9f, 0.3f, 0.85f);
    //ImGui::DragFloat("Size", &sz, 0.2f, 2.0f, 452.0f, "%.0f");
    //ImGui::DragFloat("Thickness", &thickness, 0.05f, 1.0f, 15.0f, "%.02f");
    static float raise = 0;
    ImGui::DragFloat("Volume", &raise, 0.5f, 0, 300);
    piston.VolumeChange(22.4 + raise / 3.88392, Piston::process::constant_pressure);
    ImGui::ColorEdit4("Color", &col.x);
    {
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImU32 col32 = ImColor(col);
        const ImU32 board_col32 = ImColor(board_col);
        const ImU32 Air_col32 = ImColor(Air_col);
        const ImU32 Text_col32 = ImColor(Text_col);
        float x = p.x + 100.0f, y = p.y + 200.0f, spacing = 15.0f;
        float x_board = p.x + 5, y_board = p.y + 20, board_spacing = 15.0f;
            // First line uses a thickness of 1.0, second line uses the configurable thickness 
        float th = thickness;
        draw_list->AddRect(ImVec2(x, y - 100), ImVec2(x + sz + 100, y + sz + 190), col32, 10.0f, ImDrawCornerFlags_All, th); 
        draw_list->AddRect(ImVec2(x_board, y_board), ImVec2(x + sz + 380, y + sz + 200), board_col32, 10.0f, ImDrawCornerFlags_All, 4.0f); 
        draw_list->AddLine(ImVec2(x + th - 2.0f, y + sz - raise + 100), ImVec2(x + sz - th + 2.0f + 100, y + sz - raise + 100), board_col32, 10);
        draw_list->AddRectFilled(ImVec2(x + th - 2.0f, y + sz + 6 - raise + 100), ImVec2(x + sz - th + 3.0f + 100, y + sz + 40 + 153), Air_col32, 0);


        draw_list->AddText(ImVec2(x + th + 10, y + sz + 20 + 100), Text_col32, piston.returnVolumeText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 20 + 100), Text_col32, piston.returnPressureText().c_str());
        draw_list->AddText(ImVec2(x + th + 10, y + sz + 40 + 100 ), Text_col32, piston.returnTemperatureText().c_str());
        draw_list->AddText(ImVec2(x + th + 180, y + sz + 40 + 100 ), Text_col32, piston.returnMolesText().c_str());
        draw_list->AddText(ImVec2(x + th + 60, y + sz + 70 - raise), Text_col32, "Surrounding Pressure = 1.0 atm");

        x = p.x + 4;
        y += sz + spacing;
        //ImGui::Dummy(ImVec2((sz + spacing) * 8, (sz + spacing) * 3)); //Expand to available space
    }


















    //const ImVec2 p = ImGui::GetCursorScreenPos();
    //const ImU32 col32 = ImColor(col);
    //float x = p.x + 4.0f, y = p.y + 4.0f, spacing = 15.0f;
    //// First line uses a thickness of 1.0, second line uses the configurable thickness 
    //float th = thickness;
    //draw_list->AddCircle(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col32, 6, th); x += sz + spacing;     // Hexagon 
    //draw_list->AddCircle(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col32, 8, th); x += sz + spacing;    // Circle 
    //draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col32, 0.0f, ImDrawCornerFlags_All, th); x += sz + spacing;
    //draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col32, 10.0f, ImDrawCornerFlags_All, th); x += sz + spacing;
    //draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col32, 10.0f, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotRight, th); x += sz + spacing;
    //draw_list->AddTriangle(ImVec2(x + sz * 0.5f, y), ImVec2(x + sz, y + sz - 0.5f), ImVec2(x, y + sz - 0.5f), col32, th); x += sz + spacing;
    //draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y), col32, th); x += sz + spacing;               // Horizontal line (note: drawing a filled rectangle will be faster!) 
    //draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + sz), col32, th); x += spacing;                  // Vertical line (note: drawing a filled rectangle will be faster!) 
    //draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y + sz), col32, th); x += sz + spacing;               // Diagonal line 
    //draw_list->AddBezierCurve(ImVec2(x, y), ImVec2(x + sz * 1.3f, y + sz * 0.3f), ImVec2(x + sz - sz * 1.3f, y + sz - sz * 0.3f), ImVec2(x + sz, y + sz), col32, th);
    //x = p.x + 4;
    //y += sz + spacing;
    //ImGui::Dummy(ImVec2((sz + spacing) * 8, (sz + spacing) * 3));




    ImGui::End();
}