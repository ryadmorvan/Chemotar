#include "MixtureAnalysis.h"

void MixtureAnalysis::MixtureAnalyisTool(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Mixture Analysis", p_open))
	{
		return;
	}

}
