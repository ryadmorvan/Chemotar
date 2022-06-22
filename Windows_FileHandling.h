#pragma once
#include <iostream>
#include <Windows.h>
#include <ShObjIdl.h>



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

bool _Find_File(std::string &name)
{
	std::string line;
	std::ifstream file;
	file.open("save.ini", std::ios::in);
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