// SimpleNet.cpp : Defines the entry point for the console application.
//
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
	SOCKET HostSocket = INVALID_SOCKET;
	SOCKET PlayerSocket = INVALID_SOCKET;
	bool ListenLoopContinue = true;
	bool PlayerLoopExit = false;
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

	SimpleNet::HostSocket = accept(SimpleNet::ListenSocket, NULL, NULL);

	std::stringstream msg;

	if (SimpleNet::HostSocket != INVALID_SOCKET)
	{
		msg << SetHost;
		send(SimpleNet::HostSocket, msg.str().c_str(), msg.str().length(), NULL);
		SimpleNet::HostConnected = true;
		std::cout << "Host Connected" << std::endl;
	}else
	{
		shutdown(SimpleNet::ListenSocket, 1);
		closesocket(SimpleNet::ListenSocket);
		return;
	}

	msg.str(std::string());

	SimpleNet::PlayerSocket = accept(SimpleNet::ListenSocket, NULL, NULL);

	if(SimpleNet::PlayerSocket != INVALID_SOCKET)
	{
		SimpleNet::PlayerConnected = true;
		msg << SetPlayer;
		send(SimpleNet::PlayerSocket, msg.str().c_str(), msg.str().length(), NULL);
		std::cout << "Player Connected" << std::endl;
		return;
	}
	else
	{
		shutdown(SimpleNet::ListenSocket, 1);
		closesocket(SimpleNet::ListenSocket);
	}
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
		if (SimpleNet::PlayerConnected == true)
		{
			send(SimpleNet::PlayerSocket, msg.c_str(), msg.length(), NULL);
		}
	}
}

void Go(std::string rMsg, bool isHost)
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
		if (isHost)
		{
			Send(false, rMsg.str());
		}
		else
		{
			Send(true, rMsg.str());
		}
	}
	else if (send == SendPlayer)
	{
		std::stringstream rMsg;
		rMsg << sMsg.rdbuf();
		Send(false, rMsg.str());
	}
	else
	{
		SimpleNet::Log("Error: Unknown Send");
	}
}

void PlayerLoop()
{
	while (true)
	{
		/* Player */
		/////////////////////////////////////
		if (SimpleNet::PlayerConnected)
		{
			char pbuff[150000];
			memset(pbuff, '\0', sizeof(pbuff));
			int result = recv(SimpleNet::PlayerSocket, pbuff, 150000, NULL);
			std::string msg = pbuff;
			if (result == 0)
			{
				SimpleNet::Log("Player Disconnected");
				std::cout << "Server:Player Disconnected..." << std::endl;
				closesocket(SimpleNet::PlayerSocket);
				break;
			}
			else if (result > 0)
			{
				Go(msg, false);
			}
		}
		/////////////////////////////////////
		Sleep(10);
		if (SimpleNet::PlayerLoopExit == true)
		{
			return;
		}
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
	std::thread player(PlayerLoop);

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
				SimpleNet::Log("Host Disconnected");
				std::cout << "Server:Host Disconnected..." << std::endl;
				closesocket(SimpleNet::HostSocket);
				break;
			}
			else if (iResult > 0)
			{
				Go(text, true);
			}
		}
		/////////////////////////////////////

		/* Player 
		/////////////////////////////////////
		if (SimpleNet::PlayerConnected)
		{
			char pbuff[150000];
			memset(pbuff, '\0', sizeof(pbuff));
			int result = recv(SimpleNet::PlayerSocket, pbuff, 150000, NULL);
			std::string msg = pbuff;
			if (result == 0)
			{
				SimpleNet::Log("Player Disconnected");
				std::cout << "Server:Player Disconnected..." << std::endl;
				closesocket(SimpleNet::PlayerSocket);
				break;
			}
			else if (result > 0)
			{
				Go(msg, false);
			}
		}*/
		/////////////////////////////////////

		Sleep(10);
	}
	SimpleNet::ListenLoopContinue = false;
	SimpleNet::PlayerLoopExit = true;
	WSACancelBlockingCall();
	Sleep(250);
	shutdown(SimpleNet::ListenSocket, 1);
	closesocket(SimpleNet::ListenSocket);
	shutdown(SimpleNet::PlayerSocket, 1);
	closesocket(SimpleNet::PlayerConnected);
	WSACleanup();
	Sleep(250);
	if (listen.joinable())
	{
		listen.join();
	}
	if (player.joinable())
	{
		player.join();
	}
	FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	return 0;
}

