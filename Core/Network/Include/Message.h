#pragma once
#include <Windows.h>
#include <string>

class Message
{
public:
	Message();
	Message(bool isSendDefualt);
	Message(int sendTo);
	~Message();

	Message& operator<<(int value);
	Message& operator<<(std::string value);
	Message& operator<<(double value);
	Message& operator<<(WORD value);
	Message& operator<<(char value);
	Message& operator<<(bool value);

	void SendAndReset();
	void Send();
	std::string getMessage();
	std::stringstream& get();
	void clear();
private:
	std::stringstream msg;
};

