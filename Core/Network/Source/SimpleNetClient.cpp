#include "SimpleNetClient.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atomic>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Display.h"
#include "Lobby.h"
#include "Bullet.h"
#include "Entity.h"
#include "Packet.h"
#include "UserInterface.h"
#include "PlayerHandler.h"
#include "SoundManager.h"
#include "Core.h"
#include "GoldSpawn.h"

namespace game
{
	extern Display game;
	extern PlayerHandler pHandler;
	extern UserInterface SlideUI;
	extern UserInterface ServerUI;
	extern UserInterface lobbyinfo;
	extern UserInterface lobbylist;
	extern System system;
	extern SoundManager m_sounds;
	extern std::string winnerName;
	extern bool lobbyStart;
	extern bool gameWon;
	extern atomic<bool> exitFromDisconnect;
	extern class Lobby lobby;
}

namespace SimpleNet
{
	SOCKET ConnectSocket = INVALID_SOCKET;
	bool isInitialized_ = false;
}

SimpleNetClient::SimpleNetClient()
{
	isExit_ = false;
	isConnected_ = false;
	isHost_ = false;
	isHostChosen_ = false;
	isWaiting_ = true;
	isPlayerConnected_ = false;
	isSynced_ = false;
	SimpleNet::isInitialized_ = false;
	SimpleNet::ConnectSocket = INVALID_SOCKET;
	id_ = 0;
	sync_amount_ = 0;
}

SimpleNetClient::~SimpleNetClient()
{
	if (isConnected_)
	{
		closesocket(SimpleNet::ConnectSocket);
	}
}

bool SimpleNetClient::Start()
{
	if (isConnected_ == true)
		return true;
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	int iResult;
	int recvbuflen = 512;

	// Validate the parameters

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", "15015", &hints, &result);
	if (iResult != 0)
	{
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		SimpleNet::ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (SimpleNet::ConnectSocket == INVALID_SOCKET)
		{
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(SimpleNet::ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(SimpleNet::ConnectSocket);
			SimpleNet::ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (SimpleNet::ConnectSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	isConnected_ = true;
	return true;
}

bool SimpleNetClient::Connect(std::string ip)
{
	if (isConnected_ == true)
	{
		Disconnect();
	}

	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	int iResult;
	int recvbuflen = 512;

	// Validate the parameters

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip.c_str(), "15015", &hints, &result);
	if (iResult != 0)
	{
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		SimpleNet::ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (SimpleNet::ConnectSocket == INVALID_SOCKET)
		{
			return false;
		}

		// Connect to server.
		iResult = connect(SimpleNet::ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(SimpleNet::ConnectSocket);
			SimpleNet::ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (SimpleNet::ConnectSocket == INVALID_SOCKET)
	{
		return false;
	}

	game::ServerUI.getSectionRef(1).setVar(1, "True");
	isConnected_ = true;
	return true;
}

void SimpleNetClient::Disconnect()
{
	if (isConnected_ == false) return;
	isConnected_ = false;
	isExit_ = true;
	closesocket(SimpleNet::ConnectSocket);
	game::ServerUI.getSectionRef(1).setVar(1, "False");
}

void SimpleNetClient::Loop()
{
	int BufferLength = 150000;
	std::stringstream msg;
	char msgBuffer[150000];
	while (isExit_ == false)
	{
		while (isPaused_ == true && isExit_ == false)
		{
			Sleep(5);
		}

		if (isExit_ == true)
		{
			continue;
		}
		
		memset(msgBuffer, 0x00, sizeof(msgBuffer));

		int iResult = recv(SimpleNet::ConnectSocket, msgBuffer, BufferLength, NULL);

		if (iResult == 0)
		{
			isConnected_ = false;
			game::ServerUI.getSectionRef(1).setVar(1, "False");
			Log("Server Disconnected\n");
			isExit_ = true;
			continue;
		}
		else if (iResult > 0)
		{
			std::string msg = msgBuffer;
			Do(msg);
		}

		msg.str(string());
	}

	isExit_ = false;
}

void SimpleNetClient::SendLiteral(std::string& rmsg)
{
	if (isConnected_ == false) return;
	std::stringstream msg;
	char t = '♥';
	msg << rmsg << t;
	int amount = msg.str().length();
	int amountSent = 0;
	int result;
	std::string cur;
	result = send(SimpleNet::ConnectSocket, msg.str().c_str(), msg.str().length(), NULL);
	if (result != amount)
	{
		amountSent += result;
		while (amountSent < amount)
		{
			cur = msg.str().substr(amountSent, msg.str().length());
			result = send(SimpleNet::ConnectSocket, cur.c_str(), cur.length(), NULL);
			amountSent += result;
		}
		Log("INFO: NOT ALL DATA SENT AT ONCE\n");
	}
}

void SimpleNetClient::SetId(int id)
{
	id_ = id;
}

void SimpleNetClient::Do(std::string rMsg)
{
	std::stringstream msg;
	msg << rMsg;
	int name;
	msg >> name;
	while (msg)
	{
		if (name == Sound)
		{
			std::string name;
			msg >> name;
			game::m_sounds.PlaySoundR(name);
		}
		else if (name == SetSelected)
		{
			int posX;
			int posY;
			msg >> posX;
			msg >> posY;
			Position pos;
			pos.setX(posX);
			pos.setY(posY);
			game::game.setTileAsSelectedS(pos);
		}
		else if (name == RemoveSelected)
		{
			int posX;
			int posY;
			msg >> posX;
			msg >> posY;
			Position pos;
			pos.setX(posX);
			pos.setY(posY);
			game::game.removeSelectedAtTileS(pos);
		}
		else if (name == SetTile)
		{
			Tile tile;
			tile.deserialize(msg);
			game::game.setTileAtS(tile.getPos(), tile);
			Log("Tile\n");
		}
		/* Updates */
		////////////////////////////////////////////
		else if (name == UpdatePlayer)
		{
			std::string name;
			msg >> name;
			Player* player;
			if(game::pHandler.getPlayer(name, &player))
			{
				player->deserialize(msg);
			}
			sync_amount_++;
		}
		else if (name == UpdatePlayerPosition)
		{
			string name;
			int x;
			int y;
			msg >> name;
			msg >> x;
			msg >> y;
			Player* player;
			if (game::pHandler.getPlayer(name, &player) == true)
			{
				Position pos(x, y);
				player->forceHandPosition(pos, game::game);
			}
			else
				Log("UpdatePlayerPosition Failed - \n" + name);
		}
		else if (name == PacketNames::UpdateTile)
		{
			Tile tile;
			string null;
			msg >> null;
			tile.deserialize(msg);
			game::game.setTileAt(tile.getPos(), tile);
		}
		////////////////////////////////////////////

		/* Tile Update*/
		////////////////////////////////////////////
		else if (name == TileUpdate)
		{
			msg >> name;
			if (name == Claim)
			{
				std::string name;
				Position pos;
				WORD color;
				pos.deserialize(msg);
				msg >> color;
				msg >> name;
			}
		}
		////////////////////////////////////////////

		/* Adds */
		////////////////////////////////////////////
		else if (name == AddPlayer)
		{
			//Log("//////////////////\n" + msg.str() + "///////////////////");
			Player player;
			std::string null;
			msg >> null;
			player.deserialize(msg);
			player.updateHandPos();
			game::pHandler.addPlayer(player);
			//Log("Player Added - " + player.getName());
			Log("AddPlayer\n");
		}
		else if (name == AddPlayerLocal)
		{
			//Log("//////////////////\n" + msg.str() + "///////////////////");
			Player player;
			std::string null;
			msg >> null;
			player.deserialize(msg);
			player.updateHandPos();
			game::pHandler.addLocalPlayer(player);
			//Log("Local Player Added - " + player.getName());
			Log("AddPlayerLocal\n");
		}
		////////////////////////////////////////////

		/* Entity*/
		////////////////////////////////////////////
		else if (name == EntityDamage)
		{
			/*int id;
			int damage;
			msg >> id;
			msg >> damage;*/
			Position pos;
			std::string name;
			double damage;

			pos.deserialize(msg);
			msg >> damage;
			msg >> name;
			Entity *entity;
			if (game::system.getEntityAt(pos, &entity))
			{
				entity->damage(damage, name, true);
				//Log("EntityDamage\n");
			}
			else
			{
				std::stringstream logMsg;
				logMsg << "EntityDamage - Failed - " << pos.getX() << "," << pos.getY() << EndLine;
				Log(logMsg.str());
			}
		}
		else if (name == EntityAdd)
		{
			msg >> name;
			if (name == EBullet)
			{
				shared_ptr<Bullet> bullet = make_shared<Bullet>();
				int none;
				msg >> none;
				bullet->deserialize(msg);
				bullet->setToNoUpdate();
				bullet->render();
				game::system.addEntityServer(bullet);
				Log("EntityAdd\EBullet\n");
			}
			else if (name == ETurret)
			{
				std::stringstream stream;
				shared_ptr<Turret> turret = make_shared<Turret>();
				int none;
				msg >> none;
				turret->deserialize(msg);
				turret->setToNoUpdate();
				turret->render();
				game::system.addEntityServer(turret);
				Log(stream.str());
			}
			else if (name == ECore)
			{
				shared_ptr<Core> core = make_shared<Core>();
				int none;
				msg >> none;
				core->deserialize(msg);
				core->setToNoUpdate();
				core->render();
				game::system.addEntityServer(core);
				Log("EntityAdd\ECore\n");
			}
			else if (name == EGoldSpawn)
			{
				shared_ptr<GoldSpawn> goldSpawn = make_shared<GoldSpawn>();
				int none;
				msg >> none;
				goldSpawn->deserialize(msg);
				goldSpawn->setToNoUpdate();
				goldSpawn->render();
				game::system.addEntityServer(goldSpawn);
				Log("EntityAdd\EGoldSpawn\n");
			}
		}
		else if (name == EntityKill)
		{
			int id;
			msg >> id;
			if (game::system.deleteEntityServer(id))
			{
				Log("EntityKill\n");
			}
			else
			{
				Log("EntityKill - Failed\n");
			}
		}
		else if (name == EntityUpdatePosition)
		{
			int id;
			int x;
			int y;
			msg >> id
				>> x
				>> y;
			Entity* entity;
			if (game::system.getEntityServer(id, &entity))
			{
				entity->setPos(Position(x, y));
				//Log("EntityUpdatePosition\n");
			}
			else
			{
				Log("EntityUpdatePosition - Failed\n");
			}
		}
		////////////////////////////////////////////

		/* Player Updates */
		////////////////////////////////////////////
		else if (name == PlayerUpdate)
		{
			msg >> name;
			if (name == Knockback)
			{
				double stunTime;
				std::string name;
				Position pos;
				msg >> name;
				msg >> stunTime;
				pos.deserialize(msg);
				Player* player;
				if (game::pHandler.getPlayer(name, &player))
				{
					player->forceHandPosition(pos);
					player->disableMovementFor(stunTime);
				}
			}
			else if (name == Health)
			{
				std::string name;
				msg >> name;
				int health;
				msg >> health;
				Player* player;
				if (game::pHandler.getPlayer(name, &player))
				{
					player->setHealth(health);
				}
			}
			else if (name == Kill)
			{
				std::string name;
				msg >> name;
				Player* player;
				if (game::pHandler.getPlayer(name, &player))
				{
					player->killS();
				}
			}
			else if (name == PSync)
			{
				std::stringstream msg;
				msg << SendDefault << EndLine << UpdatePlayer << EndLine << game::pHandler.getLocalPlayer().getName() << EndLine;
				game::pHandler.getLocalPlayer().serialize(msg);
				SendLiteral(msg.str());
			}
		}
		////////////////////////////////////////////

		else if (name == GetWorld)
		{
			std::string world = game::game.getWorld();
			std::stringstream wMsg; // World Msg
			wMsg << SendDefault << endl;
			wMsg << World << endl;
			wMsg << world << endl;
			SendLiteral(wMsg.str());
			//Log("GetWorld\n");
		}
		else if (name == World)
		{
			game::game.loadWorldServer(msg);
			//Log("World\n");
		}
		else if (name == SetHost)
		{
			msg >> id_;
			isHost_ = true;
			isHostChosen_ = true;
			//Log("SetHost\n");
		}
		else if (name == SetPlayer)
		{
			msg >> id_;
			isHost_ = false;
			isHostChosen_ = true;
			//Log("SetPlayer\n");
		}
		else if (name == WaitingPlayer)
		{

		}
		else if (name == PlayerConnect)
		{
			isPlayerConnected_ = true;
			//Log("PlayerConnect\n");
		}
		else if (name == PacketNames::Start)
		{
			isWaiting_ = false;
			//Log("Start\n");
		}
		else if (name == Win)
		{
			std::string name;
			msg >> name;
			game::winnerName = name;
			game::gameWon = true;
		}
		else if (name == Lobby)
		{
			msg >> name;
			if (name == LobbyAdd)
			{
				int id;
				bool isReady;
				std::string name;
				msg >> name;
				msg >> isReady;
				msg >> id;
				game::lobby.AddPlayer(id, name, isReady);
				Log("Lobby Add\n");
			}
			else if (name == LobbyName)
			{
				int id;
				std::string name;
				msg >> name;
				msg >> id;
				game::lobby.PlayerChangeName(id, name);
				Log("Lobby Name\n");
			}
			else if (name == LobbyReady)
			{
				int id;
				msg >> id;
				game::lobby.PlayerReady(id);
				Log("Lobby Ready\n");
			}
			else if (name == LobbyUnReady)
			{
				int id;
				msg >> id;
				game::lobby.PlayerUnReady(id);
				Log("Lobby UnReady\n");
			}
			else if (name == LobbyLeave)
			{
				/* TO:DO */
				int id;
				msg >> id;
				Log("Lobby Leave\n");
			}
			else if (name == LobbyGetInfo)
			{
				int id;
				msg >> id;
				std::stringstream nMsg;
				nMsg << id << EndLine
					<< Lobby << EndLine
					<< LobbyAdd << EndLine
					<< game::pHandler.getLocalPlayer().getName() << EndLine
					<< game::lobby.isReady() << EndLine
					<< id_ << EndLine;
				SendLiteral(nMsg.str());
				Log("Lobby GetInfo\n");
			}
			else if (name == LobbyStart)
			{
				game::lobby.Start();
				Log("Lobby Start\n");
			}
		}
		else if (name == SendDefault)
		{
			Log("SERVER ERROR: RECIEVED TWO MESSAGES AT ONCE\n");
		}
		else if (name == ServerDisconnected)
		{
			game::exitFromDisconnect = true;
			Log("ServerDisconnect\n");
		}
		else
		{
			std::stringstream log;
			log << "SERVER ERROR: PACKET NAME UNKNOWN -> " << name << std::endl;
			Log(log.str());
		}
		name = None;
		msg >> name;
	}
	return;
}

void SimpleNetClient::Close()
{
	if (isConnected_ == true)
	{
		shutdown(SimpleNet::ConnectSocket, SD_SEND);
		closesocket(SimpleNet::ConnectSocket);
		isConnected_ = false;
	}
	WSACleanup();
}

void SimpleNetClient::Pause()
{
	isPaused_ = true;
}

void SimpleNetClient::Continue()
{
	isPaused_ = false;
}

void SimpleNetClient::isExit(bool isExit)
{
	isExit_ = isExit;
}

bool SimpleNetClient::Initialize()
{
	if (SimpleNet::isInitialized_ == true) return true;
	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		return false;
		SimpleNet::isInitialized_ = false;
	}
	SimpleNet::isInitialized_ = true;
	return true;
}

bool SimpleNetClient::isConnected()
{
	return isConnected_;
}

bool SimpleNetClient::isPaused()
{
	return isPaused_;
}

bool SimpleNetClient::isWaiting()
{
	return isWaiting_;
}

bool SimpleNetClient::isPlayerConnected()
{
	return isPlayerConnected_;
}

bool SimpleNetClient::isHost()
{
	return isHost_;
}

bool SimpleNetClient::isHostChoosen()
{
	return isHostChosen_;
}

void SimpleNetClient::SyncPlayers()
{
	Timer time;
	sync_amount_ = 0;
	std::stringstream msg;
	msg << SendDefault << EndLine << PlayerUpdate << EndLine << PSync << EndLine;
	SendLiteral(msg.str());
	int goal = game::game.getPlayerAmount() - 1;
	time.StartNewTimer(5);
	while (sync_amount_ != goal)
	{
		if (time.Update() == true)
		{
			return;
		}
		Sleep(1);
	}
	return;
}

bool SimpleNetClient::isSynced()
{
	if (isSynced_)
	{
		isSynced_ = false;
		return true;
	}
	else
	{
		return false;
	}
}

void SimpleNetClient::UpdateTile(int x, int y)
{
	Position pos;
	pos.setX(x);
	pos.setY(y);
	game::game.updateTileServer(pos);
}

void SimpleNetClient::Log(std::string text)
{
	std::fstream file("Logs\\Log.txt", ios::app);
	file << text << endl;
	file.close();
	return;
}

int SimpleNetClient::getId()
{
	return id_;
}