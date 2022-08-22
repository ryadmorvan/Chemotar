#include "Graph.h"

//Constructs the whole graph depending on the position of the points and length
Graph::Graph(double x_start_pos, double y_start_pos, double x_end_pos, double y_end_pos, double Thickness)
{
	x = 0, y = 0;
	start_pos_x = ImVec2(x_start_pos, y_start_pos);
	end_pos_x = ImVec2(x_end_pos, y_start_pos);
	start_pos_y = ImVec2(x_start_pos, y_start_pos);
	end_pos_y = ImVec2(x_start_pos, y_end_pos);

	pressure_text_pos = ImVec2(x_start_pos - 15, y_end_pos + 5);
	volume_text_pos = ImVec2(x_end_pos - 10, y_start_pos + 5);
	thickness = Thickness;
}
//Build all the lines on the graph before rendering them
void Graph::BuildLinesVector(Piston piston, Graph::state state)
{
	if (state == Graph::state::constant_temperature)
	{
		//Linegraph object will be used to store all the information on it, then pass it to the lines vector
		LineGraph linegraph;
		VectorOfLines = std::vector<LineGraph>();
		//Draw all the dots on the graph
		for (float pressure = 1.0; pressure <= piston.pressure; pressure += 0.01)
		{
			//piston.PressureChange(pressure, Piston::process::constant_temperature);
			linegraph.setcord_point((piston.returnVolume() - 22.4) * 3.0 + 30, piston.pressure * 50, start_pos_x, start_pos_y);
			VectorOfPoints.push_back(linegraph);
		}
		//This build of dots will be used to simulate the position of the indicator on the function's line
		for (float pressure = 1.0; pressure <= 5; pressure += 0.01)
		{
			piston.PressureChange(pressure, Piston::process::constant_temperature);
			linegraph.setcord((piston.returnVolume() - 22.4) * 3.0 + 30, piston.pressure * 50, start_pos_x, start_pos_y);
			VectorOfLines.push_back(linegraph);
		}

	}
	if (state == Graph::state::constant_heat)
	{
		//Linegraph object will be used to store all the information on it, then pass it to the lines vector
		LineGraph linegraph;
		VectorOfLines = std::vector<LineGraph>();
		//Draw all the dots on the graph
		for (float pressure = 1.0; pressure <= piston.pressure; pressure += 0.01)
		{
			//piston.PressureChange(pressure, Piston::process::constant_temperature);
			linegraph.setcord_point((piston.returnVolume() - 22.4) * 3.0 + 30, piston.pressure * 50, start_pos_x, start_pos_y);
			VectorOfPoints.push_back(linegraph);
		}
		//This build of dots will be used to simulate the position of the indicator on the function's line
		for (float pressure = 1.0; pressure <= 5.0; pressure += 0.01)
		{
			piston.PressureChange(pressure, Piston::process::constant_heat);
			linegraph.setcord((piston.returnVolume() - 22.4) * 3.0 + 30, piston.pressure * 50, start_pos_x, start_pos_y);
			VectorOfLines.push_back(linegraph);
		}
	}
	if (state == Graph::state::constant_pressure)
	{
		//Linegraph object will be used to store all the information on it, then pass it to the lines vector
		LineGraph linegraph;
		VectorOfLines = std::vector<LineGraph>();
		//Draw all the dots on the graph
		for (float volume = 0; volume <= 84.3; volume += 0.01) //Max Value of Volume 106.7 - 22.4 = 84.3
		{
			//piston.AddHeat(heat, Piston::process::constant_pressure);
			linegraph.setcord((volume + 22.4) * 2, piston.pressure * 50, start_pos_x, start_pos_y);
			VectorOfLines.push_back(linegraph);
		}
		//This build of dots will be used to simulate the position of the indicator on the function's line
		for (float heat = 0; heat <= 30; heat += 0.01)
		{
			/*piston.AddHeat(heat, Piston::process::constant_pressure);*/
			linegraph.setcord_point((piston.returnVolume()) * 2, piston.pressure * 50, start_pos_x, start_pos_y);
			VectorOfPoints.push_back(linegraph);
		}
	}
	if (state == Graph::state::constant_volume)
	{
		//Linegraph object will be used to store all the information on it, then pass it to the lines vector
		LineGraph linegraph;
		VectorOfLines = std::vector<LineGraph>();
		//Draw all the dots on the graph
		for (float pressure = 0; pressure <= 5.27; pressure += 0.01) //Max value of pressure 6.27 - 1 = 5.27
		{
			//piston.AddHeat(heat, Piston::process::constant_pressure);
			linegraph.setcord((22.4) * 2 + 2, pressure * 50 + 50, start_pos_x, start_pos_y);
			VectorOfLines.push_back(linegraph);
		}
		//This build of dots will be used to simulate the position of the indicator on the function's line
		for (float heat = 0; heat <= 30; heat += 0.01)
		{
			/*piston.AddHeat(heat, Piston::process::constant_pressure);*/
			linegraph.setcord_point((piston.returnVolume()) * 2, piston.pressure * 50, start_pos_x, start_pos_y);
			VectorOfPoints.push_back(linegraph);
		}
	}
}

//Draw the actual graph before drawing the function's line
void Graph::Draw(ImDrawList* draw_list, const ImU32& Graph_col32)
{
	ImU32 Text_col32 = ImColor(Text_col);
	draw_list->AddLine(start_pos_x, end_pos_x, Graph_col32, thickness); //Draws X_Axis
	draw_list->AddLine(start_pos_y, end_pos_y, Graph_col32, thickness); //Draws Y_Axis
	draw_list->AddText(pressure_text_pos, Text_col32, "P");
	draw_list->AddText(volume_text_pos, Text_col32, "V");
}

//start drawing the function's line by extracting the information from the LineGraph's vector
void Graph::DrawLines(ImDrawList* draw_list, Piston& piston)
{

	ImVec4 Text_col2 = ImVec4(0.2f, 0.902f, 0.98f, 0.3f);
	ImU32 Text_col32 = ImColor(Text_col);
	ImU32 Text_col32_2 = ImColor(Text_col2);


	for (auto& linegraphs : VectorOfLines)
	{
		draw_list->AddLine(linegraphs.start_cord, linegraphs.end_cord, Text_col32, 2);
	}
	for (auto& linegraphs : VectorOfPoints)
	{
		draw_list->AddLine(linegraphs.start_cord, linegraphs.end_cord, Text_col32_2, 5);
	}

}
