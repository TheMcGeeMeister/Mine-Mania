// SimpleNet.cpp : Defines the entry point for the console application.
//
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define End "\n"

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
#include <map>
#include <conio.h>
#include <cctype>

#include "..\include\SPacket.h"

#pragma comment(lib, "Ws2_32.lib")

void LoopRecieve(int id);


namespace SimpleNet
{
	SOCKET ListenSocket;
	SOCKET HostSocket = INVALID_SOCKET;
	SOCKET PlayerSocket = INVALID_SOCKET;
	SOCKET PlayerSocket02 = INVALID_SOCKET;
	SOCKET PlayerSocket03 = INVALID_SOCKET;

	std::map<int, SOCKET> s_players;
	std::map<int, std::thread> s_threads;

	bool ListenLoopContinue = true;
	bool PlayerLoopExit = false;
	bool PlayerConnected = false;
	bool Player02Connected = false;
	bool Player03Connected = false;
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

	int player_index = 0;
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

	/*SimpleNet::HostSocket = accept(SimpleNet::ListenSocket, NULL, NULL);

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
	}
	else
	{
		shutdown(SimpleNet::ListenSocket, 1);
		closesocket(SimpleNet::ListenSocket);
		return;
	}


	SimpleNet::PlayerSocket02 = accept(SimpleNet::ListenSocket, NULL, NULL);

	if (SimpleNet::PlayerSocket != INVALID_SOCKET)
	{
		SimpleNet::Player02Connected = true;
		msg << SetPlayer;
		send(SimpleNet::PlayerSocket, msg.str().c_str(), msg.str().length(), NULL);
		std::cout << "Player Connected" << std::endl;
	}
	else
	{
		shutdown(SimpleNet::ListenSocket, 1);
		closesocket(SimpleNet::ListenSocket);
		return;
	}

	SimpleNet::PlayerSocket03 = accept(SimpleNet::ListenSocket, NULL, NULL);

	if (SimpleNet::PlayerSocket != INVALID_SOCKET)
	{
		SimpleNet::Player03Connected = true;
		msg << SetPlayer;
		send(SimpleNet::PlayerSocket, msg.str().c_str(), msg.str().length(), NULL);
		std::cout << "Player Connected" << std::endl;
	}
	else
	{
		shutdown(SimpleNet::ListenSocket, 1);
		closesocket(SimpleNet::ListenSocket);
		return;
	}*/

	std::stringstream msg;

	for (int i = 0; i < 4; i++)
	{
		SimpleNet::s_players[i];
		SimpleNet::s_players[i] = accept(SimpleNet::ListenSocket, NULL, NULL);
		if (SimpleNet::s_players[i] == INVALID_SOCKET)
		{
			shutdown(SimpleNet::ListenSocket, 1);
			closesocket(SimpleNet::ListenSocket);
			return;
		}
		if (i == 0)
		{
			msg << SetHost << End << i;
			send(SimpleNet::s_players[i], msg.str().c_str(), msg.str().length(), NULL);
			msg.str(std::string());
		}
		else
		{
			msg << SetPlayer << End << i;
			send(SimpleNet::s_players[i], msg.str().c_str(), msg.str().length(), NULL);
			msg.str(std::string());
		}

		SimpleNet::s_threads[i] = std::thread(LoopRecieve, i);

		std::cout << "Player Connected" << std::endl;
	}
}

void Send(int sender, std::string msg)
{
	for (auto& iter : SimpleNet::s_players)
	{
		if (iter.first == sender) continue;
		send(iter.second, msg.c_str(), msg.length(), NULL);
	}
}

void SendTo(int to, std::string msg)
{
	if (SimpleNet::s_players.count(to))
	{
		send(SimpleNet::s_players[to], msg.c_str(), msg.length(), NULL);
	}
}



void Go(std::string rMsg, int sender)
{
	std::stringstream sMsg; // Stream Msg
	sMsg << rMsg;
	uint16_t send;
	sMsg >> send;
	SimpleNet::Log(rMsg);
	if (send == SendDefault)
	{
		std::stringstream rMsg;
		rMsg << sMsg.rdbuf();
		Send(sender, rMsg.str());
	}
	else
	{
		std::stringstream rMsg;
		rMsg << sMsg.rdbuf();
		SendTo(send, rMsg.str());
	}
}

void Go(std::string rMsg, int sender, int to)
{
	std::stringstream sMsg; // Stream Msg
	sMsg << rMsg;
	uint16_t send;
	sMsg >> send;
	SimpleNet::Log(rMsg);
	if (send == SendDefault)
	{
		std::stringstream rMsg;
		rMsg << sMsg.rdbuf();
		Send(sender, rMsg.str());
	}
	else
	{
		std::stringstream rMsg;
		rMsg << sMsg.rdbuf();
		SendTo(send, rMsg.str());
	}
}

void LoopRecieve(int id)
{
	while (true)
	{
		/* Player */
		/////////////////////////////////////
		char pbuff[150000];
		memset(pbuff, '\0', sizeof(pbuff));
		int result = recv(SimpleNet::s_players[id], pbuff, 150000, NULL);
		std::string msg = pbuff;
		if (result == 0)
		{
			SimpleNet::Log("Player Disconnected");
			std::cout << "Server:Player Disconnected..." << std::endl;
			closesocket(SimpleNet::s_players[id]);
			SimpleNet::s_players.erase(id);
			break;
		}
		else if (result > 0)
		{
			Go(msg, id);
		}
		/////////////////////////////////////
		Sleep(10);
		if (SimpleNet::PlayerLoopExit == true)
		{
			return;
		}
	}
}

bool ProcessCommand(std::string cmd)
{
	if (cmd == "exit")
	{
		return true;
	}
	return false;
}
int main()
{
	if (Initialize() == false)
		exit(1);
	if (StartServer() == false)
		exit(1);

	SimpleNet::ListenLoopContinue = true;

	std::thread listen(ListenLoop);

	Sleep(250);

	std::string msg;
	std::string input;

	bool exitFlag = false;
	while (exitFlag == false)
	{
		if (GetActiveWindow() == GetFocus())
		{
			if (_kbhit())
			{
				char i = _getch();
				if (isalnum(i))
				{
					std::cout << i;
					input.push_back(i);
				}
				if (i == VK_RETURN)
				{
					std::cout << End;
					exitFlag = ProcessCommand(input);
					input = std::string();
					continue;
				}
			}
		}
		Sleep(100);
	}
	SimpleNet::ListenLoopContinue = false;
	SimpleNet::PlayerLoopExit = true;
	WSACancelBlockingCall();
	Sleep(250);
	shutdown(SimpleNet::ListenSocket, 1);
	closesocket(SimpleNet::ListenSocket);

	for (auto& iter : SimpleNet::s_players)
	{
		shutdown(iter.second, 1);
		closesocket(iter.second);
		if (SimpleNet::s_threads[iter.first].joinable())
		{
			SimpleNet::s_threads[iter.first].join();
		}
	}

	if (listen.joinable())
	{
		listen.join();
	}

	WSACleanup();
	return 0;
}

