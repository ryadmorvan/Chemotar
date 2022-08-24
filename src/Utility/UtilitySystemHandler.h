#pragma once
#include <iostream>
#include <Windows.h>
#include <ShObjIdl.h>
#include <iomanip>
#include <vector>
#include<stdio.h>


static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void OsOpenInShell(const char* path)
{
#ifdef _WIN32
	// Note: executable path must use backslashes!
	::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
	const char* open_executable = "open";
#else
	const char* open_executable = "xdg-open";
#endif
	char command[256];
	snprintf(command, 256, "%s \"%s\"", open_executable, path);
	system(command);
#endif
}

struct FileInfo
{
	std::string fileName;
	std::string filePath;
};

bool _openFile(std::string& sSelectedFile, std::string& sFilePath)
{
	//  CREATE FILE OBJECT INSTANCE
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(f_SysHr))
		return FALSE;

	// CREATE FileOpenDialog OBJECT
	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}

	//  SHOW OPEN FILE DIALOG WINDOW
	f_SysHr = f_FileSystem->Show(NULL);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
	IShellItem* f_Files;
	f_SysHr = f_FileSystem->GetResult(&f_Files);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  STORE AND CONVERT THE FILE NAME
	PWSTR f_Path;
	f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
	if (FAILED(f_SysHr)) {
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}
	//  FORMAT AND STORE THE FILE PATH
	std::wstring path(f_Path);
	std::string c(path.begin(), path.end());
	sFilePath = c;

	//  FORMAT STRING FOR EXECUTABLE NAME
	const size_t slash = sFilePath.find_last_of("/\\");
	sSelectedFile = sFilePath.substr(slash + 1);

	//  SUCCESS, CLEAN UP
	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();
	return TRUE;
}


std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

//FLAGS FOR TABLES
enum TABLES_SAVE_DATA
{
	HEAT_CAPACITY = 0x01, VISCOSITY = 0x02
};

bool _Find_File(std::string &name, TABLES_SAVE_DATA DATA)
{
	std::string line;
	std::ifstream file;
	if (DATA == 2)
	{
		file.open("table_data/Heat Capacity Tables.ini", std::ios::in);
		while (std::getline(file, line))
		{
			if (line == name)
			{
				return 0;
				file.close();
			}
		}
		file.close();
		return 1;
	}
	if (DATA == 1)
	{
		file.open("table_data/Viscosity Tables.ini", std::ios::in);
		while (std::getline(file, line))
		{
			if (line == name)
			{
				return 0;
				file.close();
			}
		}
		file.close();
		return 1;
	}
}

int _Tables_Count()
{
	std::ifstream file;
	std::string placeholder;
	int count = 0;
	file.open("save.ini", std::ios::in);
	while (std::getline(file, placeholder))
	{
		++count;
	}
	return count / 2;
}

void replace_all(
	std::string& s,
	std::string const& toReplace,
	std::string const& replaceWith
) {
	std::ostringstream oss;
	std::size_t pos = 0;
	std::size_t prevPos = pos;

	while (true) {
		prevPos = pos;
		pos = s.find(toReplace, pos);
		if (pos == std::string::npos)
			break;
		oss << s.substr(prevPos, pos - prevPos);
		oss << replaceWith;
		pos += toReplace.size();
	}

	oss << s.substr(prevPos);
	s = oss.str();
}




void _TableNamesCorrection(std::vector<std::string>& TableNames)
{
	for (auto& Table : TableNames)
	{
		replace_all(Table, ".csv", "");
	}
}




template<typename T>
int _Round(T number)
{
	return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}

template<typename T1, typename T2>
std::string _Format(T1 f, T2 n)
{
	if (f == 0) {
		return "0";
	}
	int d = (int)::ceil(::log10(f < 0 ? -f : f)); /*digits before decimal point*/
	double order = ::pow(10., n - d);
	std::stringstream ss;
	ss << std::fixed << std::setprecision(std::max(n - d, 0)) << round(f * order) / order;
	return ss.str();
}


void _Find_File_Delete(FileInfo fileInfo, TABLES_SAVE_DATA DATA)
{
	std::vector<std::string> lines;
	std::string fileName_Path;
	if (DATA == TABLES_SAVE_DATA::HEAT_CAPACITY)
	{
		fileName_Path = "table_data/Heat Capacity Tables.ini";
	}
	if (DATA == TABLES_SAVE_DATA::VISCOSITY)
	{
		fileName_Path = "table_data/Viscosity Tables.ini";
	}
	//Reading the file to the storage
	{
		//opening the file for reading from it
		std::ifstream file(fileName_Path);
		//checking if the file has been opened correctly
		if (not file.is_open()) {
			std::cerr << "can't open the file " << fileName_Path << std::endl;
			return;
		}
		//Reading
		for (std::string one_line; std::getline(file, one_line); lines.push_back(one_line));
	}
	//Get all the lines that are going to be removed from the file
	std::string line;
	std::fstream file;
	file.open(fileName_Path);
	auto iterator = 0;
	auto fileNameLine = 0;
	auto fileNamePath = 0;
	//line.replace(line.find(fileInfo.filePath), fileInfo.filePath.length(), "");
	while (std::getline(file, line))
	{
		/*line.replace(line.find(fileInfo.fileName), fileInfo.fileName.length(), "");*/
		if (line == fileInfo.fileName)
		{
			fileNameLine = iterator;
		}
		if (line == fileInfo.filePath)
		{
			fileNamePath = iterator;
		}
		iterator++;
	}
	file.close();

	remove(fileName_Path.c_str());

	std::ofstream fileWrite(fileName_Path);
	iterator = 0;
	for (auto& line : lines)
	{
		if ((iterator != fileNameLine) && (iterator != fileNamePath))
		{
			fileWrite << line << std::endl;
		}
		iterator++;
	}

	fileWrite.close();
}



bool _contains(const std::string& str, const std::string substr)
{
	if (str.size() < substr.size()) return false;

	for (int i = 0; i < str.size(); i++)
	{
		if (str.size() - i < substr.size()) return false;

		bool match = true;
		for (int j = 0; j < substr.size(); j++)
		{
			if (str.at(i + j) != substr.at(j))
			{
				match = false;
				break;
			}
		}
		if (match) return true;
	}
	return false;
}


inline void _Settings()
{
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::CollapsingHeader("Style"))
	{
		HelpMarker("The same contents can be accessed in 'Tools->Style Editor' or by calling the ShowStyleEditor() function.");
		ImGui::ShowStyleEditor();
		ImGui::TreePop();
		ImGui::Separator();
	}
	if (ImGui::CollapsingHeader("Backend Flags"))
	{
		HelpMarker(
			"Those flags are set by the back-ends (imgui_impl_xxx files) to specify their capabilities.\n"
			"Here we expose then as read-only fields to avoid breaking interactions with your back-end.");
		// Make a local copy to avoid modifying actual back-end flags.
		ImGuiBackendFlags backend_flags = io.BackendFlags;
		ImGui::CheckboxFlags("io.BackendFlags: HasGamepad", (unsigned int*)&backend_flags, ImGuiBackendFlags_HasGamepad);
		ImGui::CheckboxFlags("io.BackendFlags: HasMouseCursors", (unsigned int*)&backend_flags, ImGuiBackendFlags_HasMouseCursors);
		ImGui::CheckboxFlags("io.BackendFlags: HasSetMousePos", (unsigned int*)&backend_flags, ImGuiBackendFlags_HasSetMousePos);
		ImGui::CheckboxFlags("io.BackendFlags: RendererHasVtxOffset", (unsigned int*)&backend_flags, ImGuiBackendFlags_RendererHasVtxOffset);
		ImGui::TreePop();
		ImGui::Separator();
	}
	if (ImGui::CollapsingHeader("Capture/Logging"))
	{
		HelpMarker(
			"The logging API redirects all text output so you can easily capture the content of "
			"a window or a block. Tree nodes can be automatically expanded.\n"
			"Try opening any of the contents below in this window and then click one of the \"Log To\" button.");
		ImGui::LogButtons();
		HelpMarker("You can also call ImGui::LogText() to output directly to the log without a visual output.");
		if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
		{
			ImGui::LogToClipboard();
			ImGui::LogText("Hello, world!");
			ImGui::LogFinish();
		}
		ImGui::TreePop();
	}
}

inline void SetupImGuiStyle()
{
	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;
	ImGui::GetStyle().WindowRounding = 12.0f;
	ImGui::GetStyle().ChildRounding = 7.0f;
	ImGui::GetStyle().FrameRounding = 7.0f;
	ImGui::GetStyle().ScrollbarRounding = 12.0f;
	ImGui::GetStyle().GrabRounding = 8.0f;
	ImGui::GetStyle().FramePadding.x = 11.0f;
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.85f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

