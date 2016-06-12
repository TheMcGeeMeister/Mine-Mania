#pragma once
#include <string>
#include <map>

class NoteUI
{
public:
	NoteUI();
	~NoteUI();
	void AddLine(std::string text);

	std::string GetNextLine();

	bool IsEndOfNote();
private:
	std::map<unsigned short int, std::string> n_data;
	int index_;
	int readIndex_;
};

