#include "..\Include\NoteUI.h"



NoteUI::NoteUI()
{
	readIndex_ = 0;
	index_ = 0;
}


NoteUI::~NoteUI()
{
}

void NoteUI::AddLine(std::string text)
{
	n_data[index_] = text;
	index_++;
}

std::string NoteUI::GetNextLine()
{
	if (readIndex_ != index_)
	{
		readIndex_++;
		return n_data[readIndex_ - 1];
	}
	else
	{
		return "ERROR: READ NOTE FAILURE";
	}
}

bool NoteUI::IsEndOfNote()
{
	return readIndex_ == index_;
}
