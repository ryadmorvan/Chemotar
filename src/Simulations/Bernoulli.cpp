#include "Bernoulli.h"


void Bernoulli::Draw(ImDrawList* draw_list)
{
	_pipes.Draw(draw_list);

}

void Bernoulli::Bernoulli_Simulation(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Bernoulli Simulation", p_open))
	{
		return;
	}
	Bernoulli bern;
	static ImDrawList* draw_list = ImGui::GetWindowDrawList();
	bern.Draw(draw_list);

}
