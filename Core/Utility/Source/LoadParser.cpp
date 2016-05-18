#include "..\Include\LoadParser.h"



LoadParser::LoadParser()
{
	Global_Errors_ = 0;
	Global_Lines_ = 0;
	Current_Errors_ = 0;
	Current_Line_ = 0;
	isError_ = false;
}


LoadParser::~LoadParser()
{

}

int LoadParser::ReadInt(std::fstream & stream)
{
	Current_Line_++;
	Global_Lines_++;
	int data;
	stream >> data;
	if (stream.good() == false)
	{
		isError_ = true;
		Current_Errors_++;
		Global_Errors_++;
		stream.clear();
		return 0;
	}
	else
	{
		return data;
	}
}

int LoadParser::ReadInt(std::stringstream & stream)
{
	Current_Line_++;
	Global_Lines_++;
	int data;
	stream >> data;
	if (stream.good() == false)
	{
		isError_ = true;
		Current_Errors_++;
		Global_Errors_++;
		stream.clear();
		return 0;
	}
	else
	{
		return data;
	}
}

double LoadParser::ReadDouble(std::fstream & stream)
{
	Current_Line_++;
	Global_Lines_++;
	double data;
	stream >> data;
	if (stream.good() == false)
	{
		isError_ = true;
		Current_Errors_++;
		Global_Errors_++;
		stream.clear();
		return 0.0;
	}
	else
	{
		return data;
	}
}

double LoadParser::ReadDouble(std::stringstream & stream)
{
	Current_Line_++;
	Global_Lines_++;
	double data;
	stream >> data;
	if (stream.good() == false)
	{
		isError_ = true;
		Current_Errors_++;
		Global_Errors_++;
		stream.clear();
		return 0.0;
	}
	else
	{
		return data;
	}
}

bool LoadParser::ReadBool(std::fstream & stream)
{
	Current_Line_++;
	Global_Lines_++;
	bool data;
	stream >> data;
	if (stream.good() == false)
	{
		isError_ = true;
		Current_Errors_++;
		Global_Errors_++;
		stream.clear();
		return false;
	}
	else
	{
		return data;
	}
}

bool LoadParser::ReadBool(std::stringstream & stream)
{
	Current_Line_++;
	Global_Lines_++;
	bool data;
	stream >> data;
	if (stream.good() == false)
	{
		isError_ = true;
		Current_Errors_++;
		Global_Errors_++;
		stream.clear();
		return false;
	}
	else
	{
		return data;
	}
}

std::string LoadParser::ReadString(std::fstream & stream)
{
	Current_Line_++;
	Global_Lines_++;
	std::string data;
	stream >> data;
	if (stream.good() == false)
	{
		isError_ = true;
		Current_Errors_++;
		Global_Errors_++;
		stream.clear();
		return "ERROR";
	}
	else
	{
		return data;
	}
}

std::string LoadParser::ReadString(std::stringstream & stream)
{
	Current_Line_++;
	Global_Lines_++;
	std::string data;
	stream >> data;
	if (stream.good() == false)
	{
		isError_ = true;
		Current_Errors_++;
		Global_Errors_++;
		stream.clear();
		return "ERROR";
	}
	else
	{
		return data;
	}
}

std::string LoadParser::GetReport()
{
	std::stringstream report;
	report << "Report:" << "\n"
		<< "Errors: " << Global_Errors_ << "\n"
		<< "Lines Read: " << Global_Lines_ << "\n";
	return report.str();
}

int & LoadParser::GetGlobalErrors()
{
	return Global_Errors_;
}

int & LoadParser::GetGlobalLines()
{
	return Global_Lines_;
}

bool LoadParser::isError()
{
	return isError_;
}

void LoadParser::NextLoad()
{
	Current_Errors_ = 0;
	Current_Line_ = 0;
	isError_ = false;
}

void LoadParser::Reset()
{
	Global_Errors_ = 0;
	Global_Lines_ = 0;
	Current_Errors_ = 0;
	Current_Line_ = 0;
	isError_ = false;
}
