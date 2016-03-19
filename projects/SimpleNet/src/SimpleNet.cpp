// SimpleNet.cpp : Defines the entry point for the console application.
//
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <utility>
#include <fstream>
#include <sstream>

#include "..\include\SimpleNetClient.h"
#include "..\include\SPacket.h"

#pragma comment(lib, "Ws2_32.lib")


namespace SimpleNet
{
	SOCKET ListenSocket;
	SOCKET HostSocket;
	SOCKET PlayerSocket;
	bool ListenLoopContinue;
	bool PlayerConnected = false;
	bool HostConnected = false;
	void Log(std::string text)
	{
		std::fstream file("Log.txt", std::ios::app);
		file << text << std::endl;
		if (!file)
		{
			std::cout << "FUCK MY LIFE" << std::endl;
		}
		file.close();
	}
}

bool Initialize()
{
	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		return false;
	}
	return true;
}

bool StartServer()
{
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(NULL, "15015", &hints, &result);
	if (iResult != 0) 
	{
		WSACleanup();
		return false;
	}

	SimpleNet::ListenSocket = INVALID_SOCKET;

	// Create a SOCKET for the server to listen for client connections

	SimpleNet::ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (SimpleNet::ListenSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	iResult = bind(SimpleNet::ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		closesocket(SimpleNet::ListenSocket);
		WSACleanup();
		return false;
	}

	freeaddrinfo(result);

	if (listen(SimpleNet::ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(SimpleNet::ListenSocket);
		WSACleanup();
		return false;
	}

	return true;
}

void ListenLoop()
{
	//while (SimpleNet::ClientSocket == INVALID_SOCKET)
	//{
		/*timeval time;
		time.tv_sec = 0.01;
		if (select(NULL, &s, NULL, NULL, &time))
		{
			SimpleNet::ClientSocket = INVALID_SOCKET;

			if (FD_ISSET(&s, SimpleNet::ListenSocket))
			{
				SimpleNet::ClientSocket = accept(SimpleNet::ListenSocket, NULL, NULL);
				if (SimpleNet::ClientSocket == INVALID_SOCKET)
				{
					closesocket(SimpleNet::ListenSocket);
					WSACleanup();
					return;
				}
			}
		}*/
		SimpleNet::HostSocket = accept(SimpleNet::ListenSocket, NULL, NULL);
		std::string msg = (char)SetHost + "\n";
		send(SimpleNet::HostSocket, msg.c_str(), msg.length(), NULL);
		SimpleNet::HostConnected = true;
		if (SimpleNet::HostSocket == INVALID_SOCKET)
		{
			closesocket(SimpleNet::ListenSocket);
			WSACleanup();
			return;
		}
		SimpleNet::PlayerConnected = true;
		std::cout << "Host Connected" << std::endl;
		SimpleNet::PlayerSocket = accept(SimpleNet::ListenSocket, NULL, NULL);
		msg = (char)SetPlayer + "\n";
		send(SimpleNet::PlayerSocket, msg.c_str(), msg.length(), NULL);
		return;
	//}
	return;
}

void Send(bool toHost, std::string msg)
{
	if (toHost)
	{
		send(SimpleNet::HostSocket, msg.c_str(), msg.length(), NULL);
	}
	else
	{
		send(SimpleNet::PlayerSocket, msg.c_str(), msg.length(), NULL);
	}
}

void Go(std::string rMsg)
{
	/*std::string key;
	std::stringstream msg;
	std::stringstream remainder;
	msg << rMsg;
	msg >> key;
	SimpleNet::Log(rMsg);
	if (key == "SendHost")
	{
		remainder << msg.rdbuf();
		//std::cout << "Sent To Host:" << remainder.str() << std::endl;
		Send(true, remainder.str());
	}
	else if (key == "SendPlayer")
	{
		remainder << msg.rdbuf();
		//std::cout << "Sent To Player:" << remainder.str() << std::endl;
		Send(false, remainder.str());
	}
	else
	{
		Send(true, rMsg);
	}*/
	std::stringstream sMsg; // Stream Msg
	sMsg << rMsg;
	uint16_t send;
	sMsg >> send;
	if (send == SendDefault)
	{
		std::stringstream rMsg;
		rMsg << sMsg.rdbuf();
		Send(true, rMsg.str());
	}
	else
	{
		SimpleNet::Log("Error: Unknown Send");
	}
}

int main()
{
	if (Initialize() == false)
		exit(1);
	if (StartServer() == false)
		exit(1);

	SimpleNet::ListenLoopContinue = true;

	std::thread listen(ListenLoop);

	//SimpleNetClient client;

	
	//client.Start();

	//std::thread clientLoop(std::bind(&SimpleNetClient::Loop, &client));

	Sleep(250);

	std::string msg;

	while (true)
	{
		/* Host*/
		/////////////////////////////////////
		if (SimpleNet::HostConnected)
		{
			char hbuff[150000];
			memset(hbuff, '\0', sizeof(hbuff));
			int iResult = recv(SimpleNet::HostSocket, hbuff, 150000, NULL);
			std::string text = hbuff;
			if (iResult == 0)
			{
				SimpleNet::Log("Error: Host Result == 0");
				std::cout << "Server:Host Disconnected..." << std::endl;
				closesocket(SimpleNet::HostSocket);
				break;
			}
			else if (iResult > 0)
			{
				//std::cout << "Host:" << text << std::endl;
				Go(text);
			}
		}
		/////////////////////////////////////

		/* Player */
		/////////////////////////////////////
		if (SimpleNet::PlayerConnected)
		{
			char pbuff[150000];
			memset(pbuff, '\0', sizeof(pbuff));
			int result = recv(SimpleNet::HostSocket, pbuff, 150000, NULL);
			std::string msg = pbuff;
			if (result == 0)
			{
				SimpleNet::Log("Error: Result == 0\n");
				std::cout << "Server:Player Disconnected..." << std::endl;
				closesocket(SimpleNet::HostSocket);
				break;
			}
			else if (result > 0)
			{
				//std::cout << "Player:" << msg << std::endl
				Go(msg);
			}
		}
		/////////////////////////////////////

		Sleep(10);
	}
	SimpleNet::ListenLoopContinue = false;
	Sleep(250);
	FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	return 0;
}

