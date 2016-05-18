#pragma once
#include <fstream>
#include <sstream>

class LoadParser
{
public:
	LoadParser();
	~LoadParser();

	int ReadInt(std::fstream& stream);
	int ReadInt(std::stringstream& stream);
	double ReadDouble(std::fstream& stream);
	double ReadDouble(std::stringstream& stream);
	bool ReadBool(std::fstream& stream);
	bool ReadBool(std::stringstream& stream);
	std::string ReadString(std::fstream& stream);
	std::string ReadString(std::stringstream& stream);

	std::string GetReport();

	int& GetGlobalErrors();
	int& GetGlobalLines();

	bool isError();

	void NextLoad();
	void Reset();

private:
	int Global_Errors_;
	int Global_Lines_;
	int Current_Errors_;
	int Current_Line_;
	bool isError_;
};

