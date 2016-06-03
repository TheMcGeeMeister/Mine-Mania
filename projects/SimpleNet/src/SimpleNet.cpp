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

#include "SPacket.h"

#pragma comment(lib, "Ws2_32.lib")

void LoopRecieve(int id);


namespace SimpleNet
{
	SOCKET ListenSocket;

	std::map<int, SOCKET> s_players;
	std::map<int, std::thread> s_threads;

	bool ListenLoopContinue = true;
	bool PlayerLoopExit = false;
	bool HostConnected = false;
	bool HostDisconnectReset = false;
	bool LoggingEnabled = false;
	bool isReset = false;
	void Log(std::string text)
	{
		if (LoggingEnabled == false) return;
		std::fstream file("Log.txt", std::ios::app);
		file << text << std::endl;
		if (!file)
		{
			std::cout << "Error: Can't open Log.txt" << std::endl;
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
	std::stringstream msg;

	for (int i = 0; i < 4; i++)
	{
		SimpleNet::s_players[i] = INVALID_SOCKET;
		SimpleNet::s_players[i] = accept(SimpleNet::ListenSocket, NULL, NULL);
		if (SimpleNet::s_players[i] == INVALID_SOCKET)
		{
			shutdown(SimpleNet::ListenSocket, 2);
			closesocket(SimpleNet::ListenSocket);
			return;
		}
		if (i == 0)
		{
			msg << SetHost << End << i;
			SimpleNet::s_threads[i] = std::thread(LoopRecieve, i);
			send(SimpleNet::s_players[i], msg.str().c_str(), msg.str().length(), NULL);
			msg.str(std::string());
			SimpleNet::HostConnected = true;
			std::cout << "Host Connected" << std::endl;
		}
		else
		{
			msg << SetPlayer << End << i;
			SimpleNet::s_threads[i] = std::thread(LoopRecieve, i);
			send(SimpleNet::s_players[i], msg.str().c_str(), msg.str().length(), NULL);
			msg.str(std::string());
			std::cout << "Player Connected" << std::endl;
		}
	}
}

void Send(int sender, std::string& msg)
{
	for (auto& iter : SimpleNet::s_players)
	{
		if (iter.first == sender) continue;
		send(iter.second, msg.c_str(), msg.length(), NULL);
	}
}

void SendTo(int to, std::string& msg)
{
	if (SimpleNet::s_players.count(to))
	{
		send(SimpleNet::s_players[to], msg.c_str(), msg.length(), NULL);
	}
}

void Go(std::string rMsg, int sender)
{
	int CurPos = 0;
	int EndPos = rMsg.find('♥', 0);
	std::stringstream curMsg;
	std::stringstream sMsg;
	uint16_t send;
	while (EndPos != std::string::npos)
	{
		curMsg << rMsg.substr(CurPos, (EndPos - CurPos));
		curMsg >> send;
		SimpleNet::Log(curMsg.str());
		if (send == SendDefault)
		{
			sMsg << curMsg.rdbuf();
			Send(sender, sMsg.str());
		}
		else if(send < 12 && send >= 0)
		{
			sMsg << curMsg.rdbuf();
			SendTo(send, sMsg.str());
		}
		sMsg.str(std::string());
		curMsg.str(std::string());
		CurPos = EndPos + 1;
		EndPos = rMsg.find('♥', CurPos);
	}
}

void Go(std::string rMsg, int sender, int to)
{
	int CurPos = 0;
	int EndPos = rMsg.find('♥', 0);
	std::stringstream curMsg;
	std::stringstream sMsg;
	uint16_t send;
	while (EndPos != std::string::npos)
	{
		curMsg << rMsg.substr(CurPos, (EndPos - CurPos));
		curMsg >> send;
		SimpleNet::Log(curMsg.str());
		if (send == SendDefault)
		{
			sMsg << curMsg.rdbuf();
			Send(sender, sMsg.str());
		}
		else
		{
			sMsg << curMsg.rdbuf();
			SendTo(send, sMsg.str());
		}
		sMsg.str(std::string());
		curMsg.str(std::string());
		CurPos = EndPos + 1;
		EndPos = rMsg.find('♥', CurPos);
	}
}

bool ProcessCommand(std::string cmd);

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
			closesocket(SimpleNet::s_players[id]);
			if (id == 0)
			{
				std::cout << "Server:Host Disconnected..." << std::endl;
				SimpleNet::HostDisconnectReset = true;
				return;
			}
			else
			{
				std::cout << "Server:Player Disconnected..." << std::endl;
			}
			break;
		}
		else if (result > 0)
		{
			Go(msg, id);
		}
		/////////////////////////////////////
		if (SimpleNet::PlayerLoopExit == true)
		{
			std::stringstream msg;
			msg << ServerDisconnected;
			send(SimpleNet::s_players[id], msg.str().c_str(), msg.str().length(), NULL);
			SimpleNet::Log("Forced Disconnection... (PlayerLoopExit == true)");
			Sleep(250);
			shutdown(SimpleNet::s_players[id], 2);
			std::cout << "Server:Forced Disconnection..." << End;
			if(id == 0)
			{
				SimpleNet::HostDisconnectReset = true;
				return;
			}
			break;
		}
		Sleep(10);
	}
	return;
}

bool ProcessCommand(std::string cmd)
{
	if (cmd == "exit")
	{
		std::cout << End << "Exiting..." << End;
		return true;
	}
	else if (cmd == "log" || cmd == "Log")
	{
		SimpleNet::LoggingEnabled = !SimpleNet::LoggingEnabled;
		if (SimpleNet::LoggingEnabled == true)
		{
			std::cout << End << "Logging: Enabled" << End;
		}
		else
		{
			std::cout << End << "Logging: Disabled" << End;
		}
	}
	else if (cmd == "reset" || cmd == "Reset")
	{
		std::cout << "Resetting..." << End;
		SimpleNet::ListenLoopContinue = false;
		SimpleNet::PlayerLoopExit = true;
		closesocket(SimpleNet::ListenSocket);
		WSACancelBlockingCall();
		Sleep(1000);
		for (auto& iter : SimpleNet::s_players)
		{
			shutdown(iter.second, 2);
			closesocket(iter.second);
			if (SimpleNet::s_threads[iter.first].joinable())
			{
				SimpleNet::s_threads[iter.first].join();
			}
		}
		SimpleNet::s_players.clear();
		SimpleNet::s_threads.clear();
		if (StartServer() == true)
		{
			std::cout << "Server Reset: Successful" << End;
		}
		else
		{
			std::cout << "Server Reset: Failed" << End;
			SimpleNet::isReset = true;
		}
		SimpleNet::ListenLoopContinue = true;
		SimpleNet::PlayerLoopExit = false;
	}
	return false;
}
int main()
{
	if (Initialize() == false)
		return 1;
	if (StartServer() == false)
		return 1;

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
					if (SimpleNet::isReset == true)
					{
						listen = std::thread(ListenLoop);
						SimpleNet::isReset = false;
					}
					continue;
				}
			}
			if (SimpleNet::HostDisconnectReset == true)
			{
				SimpleNet::HostDisconnectReset = false;
				ProcessCommand("Reset");
			}
		}
		Sleep(10);
	}
	SimpleNet::ListenLoopContinue = false;
	SimpleNet::PlayerLoopExit = true;
	WSACancelBlockingCall();
	Sleep(1000);
	shutdown(SimpleNet::ListenSocket, 2);
	closesocket(SimpleNet::ListenSocket);

	for (auto& iter : SimpleNet::s_players)
	{
		shutdown(iter.second, 2);
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

