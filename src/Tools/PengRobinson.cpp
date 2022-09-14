#include "PengRobinson.h"

void PengRobinson::PengRobinsonCalculator(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Peng-Robinson EOS Calculator", p_open))
	{
		return;
	}

}
