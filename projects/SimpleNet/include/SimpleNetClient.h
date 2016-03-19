#pragma once
#include <winsock2.h>
#include <string>

class SimpleNetClient
{
public:
	SimpleNetClient();
	~SimpleNetClient();

	bool Start();
	void Loop();
	void Send(std::string msg);
	void Do(std::string rMsg);
private:
	bool isExit_;
	SOCKET ConnectSocket;
};

