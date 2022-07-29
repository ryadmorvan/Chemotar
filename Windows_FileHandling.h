#pragma once
#include <iostream>
#include <Windows.h>
#include <ShObjIdl.h>
#include <iomanip>
#include <vector>
#include<stdio.h>





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
	if (DATA == HEAT_CAPACITY)
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
	if (DATA == VISCOSITY)
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




int _Round(double number)
{
	return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}

std::string _Format(double f, int n)
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
			std::cout << fileNameLine << std::endl;
		}
		if (line == fileInfo.filePath)
		{
			fileNamePath = iterator;
			std::cout << fileNamePath << std::endl;
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