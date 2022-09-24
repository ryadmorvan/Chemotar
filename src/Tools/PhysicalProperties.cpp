#include "PhysicalProperties.h"








void PhysicalProperties::load_table(std::string file_name)
{
}

void PhysicalProperties::ShowPhysicalProperties(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(1200, 728), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Physical Properties", p_open))
	{
		return;
	}
	static PhysicalProperties application;
	static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
		flags |= ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
		| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
		| ImGuiTableFlags_SizingFixedFit;

	if(ImGui::BeginTable("Physical Properties", 6, flags))
	{
		ImGui::TableSetupScrollFreeze(false, true);
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
		static bool display_headers = false;
		//ImGui::TableSetupColumn(" ");
		ImGui::TableSetupColumn("Compound");
		ImGui::TableSetupColumn("Molecular Weight (g/mol)");
		ImGui::TableSetupColumn("Melting Temperature (C)");
		ImGui::TableSetupColumn("Boiling Temperature (C)");
		ImGui::TableSetupColumn("Critical Temperature (K)");
		ImGui::TableSetupColumn("Critical Pressure (atm)");
		ImGui::TableHeadersRow();

		static int value = 0;
			std::vector<bool> placeholder;
			placeholder.resize((application.returnTable())->size()); //APPROPRIATE RESIZE OF OUR VECTOR
			for (int row = 0; row < (application.returnTable())->size(); row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 6; column++)
				{
					ImGui::TableSetColumnIndex(column);
					if ((value == row) && (value != 0))
					{
						//When the row is selected, we highlight it
						ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.8f, 0.9f), (application.returnTable())->at(row).at(column).c_str());
					}
					else if (ImGui::Selectable((application.returnTable())->at(row).at(column).c_str(), placeholder.at(row),
					                           ImGuiSelectableFlags_SpanAllColumns |
					                           ImGuiSelectableFlags_AllowItemOverlap))
					{
						placeholder.at(row) = true;
						value = row;
					}
				}
			}
			ImGui::EndTable();
	}

	ImGui::End();
}

PhysicalProperties::PhysicalProperties()
{
	filePath = "Empirical Tables/Physical Properties/Physical Properties.csv";
	std::fstream file(filePath, std::ios::in);
	PhysicalPropertiesVector = std::make_unique<std::vector<std::array<std::string, 6>>>();
	std::string word;
	if(file.is_open())
	{
		std::cout << "Successfully opened" << std::endl;
		std::string line;
		int iterate_vec = 0;
		while (std::getline(file, line)) 
		{
			std::stringstream str(line); //Extracts string data that are seperated by ',' in this line
			int iterate_arr = 0;
			PhysicalPropertiesVector->emplace_back();
			while (std::getline(str, word, ','))
			{
				PhysicalPropertiesVector->at(iterate_vec).at(iterate_arr) = word; //Stores all the steam table data into this vector
				iterate_arr++;
			}
			iterate_vec++;
		}
	}
	else
		std::cout << "Failed to Open" << std::endl;

	file.close();
}
