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
	bool LoggingEnabled = false;
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
			shutdown(SimpleNet::ListenSocket, 1);
			closesocket(SimpleNet::ListenSocket);
			return;
		}
		if (i == 0)
		{
			msg << SetHost << End << i;
			SimpleNet::s_threads[i] = std::thread(LoopRecieve, i);
			send(SimpleNet::s_players[i], msg.str().c_str(), msg.str().length(), NULL);
			msg.str(std::string());
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

void LogTry(std::string rmsg)
{
	return;
	std::stringstream msg;
	msg >> rmsg;
	int name;
	msg << name;
	if (name == Sound)
	{
		/*std::string name;
		msg >> name;
		game::m_sounds.PlaySoundR(name);*/

	}
	else if (name == SetSelected)
	{
		//int posX;
		//int posY;
		//msg >> posX;
		//msg >> posY;
		//Position pos;
		//pos.setX(posX);
		//pos.setY(posY);
		//game::game.setTileAsSelectedS(pos);
		SimpleNet::Log("SetSelected\n");
	}
	else if (name == RemoveSelected)
	{
		//int posX;
		//int posY;
		//msg >> posX;
		//msg >> posY;
		//Position pos;
		//pos.setX(posX);
		//pos.setY(posY);
		//game::game.removeSelectedAtTileS(pos);
		SimpleNet::Log("RemoveSelected\n");
	}
	else if (name == SetTile)
	{
		//Tile tile;
		//tile.deserialize(msg);
		//game::game.setTileAtS(tile.getPos(), tile);
		//Log("Tile\n");
		SimpleNet::Log("SetTile\n");
	}
	/* Updates */
	////////////////////////////////////////////
	else if (name == UpdatePlayer)
	{
		//sg >> name;
		//f (name == Health)
		//
		//	int health;
		//	string player_name;
		//	msg >> player_name;
		//	msg >> health;
		//	Player *player;
		//	if (game::pHandler.getPlayer(player_name, &player))
		//	{
		//		player->getHealthRef().setHealth(health);
		//	}
		//
		SimpleNet::Log("UpdatePlayer\n");
	}
	else if (name == UpdatePlayerPosition)
	{
		//string name;
		//msg >> name;
		//int x;
		//int y;
		//msg >> x;
		//msg >> y;
		//Player* player;
		//if (game::pHandler.getPlayer(name, &player) == true)
		//{
		//	Position pos(x, y);
		//	player->forceHandPosition(pos, game::game);
		//}
		//else
		//	Log("UpdatePlayerPosition Failed - \n" + name);
		//Log("UpdatePlayerPosition\n");
		SimpleNet::Log("UpdatePlayerPostition\n");
	}
	else if (name == UpdateTile)
	{
		//Tile tile;
		//string null;
		//msg >> null;
		//tile.deserialize(msg);
		//game::game.setTileAt(tile.getPos(), tile);
		//Log("UpdateTile\n");
		SimpleNet::Log("UpdateTile\n");
	}
	////////////////////////////////////////////

	/* Adds */
	////////////////////////////////////////////
	else if (name == AddPlayer)
	{
		////Log("//////////////////\n" + msg.str() + "///////////////////");
		//Player player;
		//std::string null;
		//msg >> null;
		//player.deserialize(msg);
		//player.updateHandPos();
		//game::pHandler.addPlayer(player);
		////Log("Player Added - " + player.getName());
		//Log("AddPlayer\n");
		SimpleNet::Log("AddPlayer\n");
	}
	else if (name == AddPlayerLocal)
	{
		////Log("//////////////////\n" + msg.str() + "///////////////////");
		//Player player;
		//std::string null;
		//msg >> null;
		//player.deserialize(msg);
		//player.updateHandPos();
		//game::pHandler.addLocalPlayer(player);
		////Log("Local Player Added - " + player.getName());
		//Log("AddPlayerLocal\n");
		SimpleNet::Log("AddPlayerLocal\n");
	}
	////////////////////////////////////////////

	/* Entity*/
	////////////////////////////////////////////
	else if (name == EntityDamage)
	{
		//int x;
		//int y;
		//int damage;
		//std::string name;
		//msg >> x
		//	>> y
		//	>> damage
		//	>> name;
		//Entity *entity;
		//if (game::system.getEntityAt(Position(x, y), &entity))
		//{
		//	entity->damage(damage, name);
		//}
		SimpleNet::Log("EntityDamage\n");
	}
	else if (name == EntityAdd)
	{
		//msg >> name;
		if (name == EBullet)
		{
			//shared_ptr<Bullet> bullet = make_shared<Bullet>();
			//int none;
			//msg >> none;
			//bullet->deserialize(msg);
			//bullet->setToNoUpdate();
			//game::system.addEntity(bullet);
			//Log("EntityAdd\EBullet\n");
			SimpleNet::Log("EntityAdd\EBullet\n");
		}
		else if (name == ETurret)
		{
			//shared_ptr<Turret> turret = make_shared<Turret>();
			//int none;
			//msg >> none;
			//turret->deserialize(msg);
			//turret->setToNoUpdate();
			//game::system.addEntity(turret);
			//Log("EntityAdd\ETurret\n");
			SimpleNet::Log("EntityAdd\ETurret\n");
		}
		else if (name == ECore)
		{
			//shared_ptr<Core> core = make_shared<Core>();
			//int none;
			//msg >> none;
			//core->deserialize(msg);
			//core->setToNoUpdate();
			//game::system.addEntity(core);
			//Log("EntityAdd\ECore\n");
			SimpleNet::Log("EntityAdd\ECore\n");
		}
	}
	else if (name == EntityUpdatePosition)
	{
		//int id;
		//int x;
		//int y;
		//msg >> id;
		//msg >> x;
		//msg >> y;
		//Entity* entity;
	//	if (game::system.getEntity(id, &entity))
		//{
			//entity->setPos(Position(x, y));
		//}
		SimpleNet::Log("EntityUpdatePosition\n");
	}
	////////////////////////////////////////////

	else if (name == GetWorld)
	{
		//std::string world = game::game.getWorld();
		//std::stringstream wMsg; // World Msg
		//wMsg << SendDefault << endl;
		//wMsg << World << endl;
		//wMsg << world << endl;
		//SendLiteral(wMsg.str());
		//Log("GetWorld\n");
		SimpleNet::Log("GetWorld\n");
	}
	else if (name == World)
	{
		//game::game.loadWorldServer(msg);
		//Log("World\n");
		SimpleNet::Log("World\n");
	}
	else if (name == SetHost)
	{
		//msg >> id_;
		//isHost_ = true;
		//isHostChosen_ = true;
		//Log("SetHost\n");
		SimpleNet::Log("SetHost\n");
	}
	else if (name == SetPlayer)
	{
		//msg >> id_;
		//isHost_ = false;
		//isHostChosen_ = true;
		//Log("SetPlayer\n");
		SimpleNet::Log("SetPlayer\n");
	}
	else if (name == WaitingPlayer)
	{
		SimpleNet::Log("WaitingPlayer\n");
	}
	else if (name == PlayerConnect)
	{
		//isPlayerConnected_ = true;
		//Log("PlayerConnect\n");
		SimpleNet::Log("PlayerConnect\n");
	}
	else if (name == Start)
	{
		//isWaiting_ = false;
		//Log("Start\n");
		SimpleNet::Log("Start\n");
	}
	else if (name == Lobby)
	{
		msg >> name;
		if (name == LobbyAdd)
		{
			//int id;
			//bool isReady;
			//std::string name;
			//msg >> name;
			//msg >> isReady;
			//msg >> id;
			//game::lobby.AddPlayer(id, name, isReady);
			//Log("Lobby Add\n");
			SimpleNet::Log("Lobby Add\n");
		}
		else if (name == LobbyName)
		{
			//int id;
			//std::string name;
			//msg >> name;
			//msg >> id;
			//game::lobby.PlayerChangeName(id, name);
			//Log("Lobby Name\n");
			SimpleNet::Log("Lobby Name\n");
		}
		else if (name == LobbyReady)
		{
			//int id;
			//msg >> id;
			//game::lobby.PlayerReady(id);
			//Log("Lobby Ready\n");
			SimpleNet::Log("Lobby Ready\n");
		}
		else if (name == LobbyUnReady)
		{
			//int id;
			//msg >> id;
			//game::lobby.PlayerUnReady(id);
			//Log("Lobby UnReady\n");
			SimpleNet::Log("Lobby UnReady\n");
		}
		else if (name == LobbyLeave)
		{
			//std::string name;
			//msg >> name;
			//game::lobby.RemovePlayer(name);
			//Log("Lobby Leave\n");
			SimpleNet::Log("Lobby Leave\n");
		}
		else if (name == LobbyGetInfo)
		{
			//int id;
			//msg >> id;
			//std::stringstream nMsg;
			//nMsg << id << EndLine
			//	<< Lobby << EndLine
			//	<< LobbyAdd << EndLine
			//	<< game::pHandler.getLocalPlayer().getName() << EndLine
			//	<< game::lobby.isReady() << EndLine
			//	<< id_ << EndLine;
			//SendLiteral(nMsg.str());
			//Log("Lobby GetInfo\n");
			SimpleNet::Log("Lobby GetInfo\n");
		}
		else if (name == LobbyStart)
		{
			//game::lobby.Start();
			//Log("Lobby Start\n");
			SimpleNet::Log("Lobby Start\n");
		}
	}
	else if (name == SendDefault)
	{
	//	Log("Multiple Msg's Sent At Once\n");
		SimpleNet::Log("SendDefault??\n");
	}
	else
	{
	//	std::stringstream log;
	//	log << "Server: Unknown Packet Name: " << name;
	//	Log(log.str());
		SimpleNet::Log("Thee Unknown mann\n");
	}
	name = None;
	msg >> name;
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
			shutdown(SimpleNet::s_players[id], 1);
			std::cout << "Server:Forced Disconnection..." << End;
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
	else if (cmd == "log")
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
		Sleep(10);
	}
	SimpleNet::ListenLoopContinue = false;
	SimpleNet::PlayerLoopExit = true;
	WSACancelBlockingCall();
	Sleep(1000);
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

