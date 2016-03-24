#include "..\include\SimpleNetClient.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <Display.h>
#include <UserInterface.h>
#include <PlayerHandler.h>
#include "..\Mine-Mania\include\Packet.h"

namespace game
{
	extern Display game;
	extern PlayerHandler pHandler;
	extern UserInterface SlideUI;
	extern UserInterface ServerUI;
}

namespace SimpleNet
{
	SOCKET ConnectSocket = INVALID_SOCKET;
}

SimpleNetClient::SimpleNetClient()
{
	isExit_ = false;
	isConnected_ = false;
	isHost_ = false;
	isHostChosen_ = false;
	isWaiting_ = true;
	isPlayerConnected_ = false;
	SimpleNet::ConnectSocket = INVALID_SOCKET;
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

	// Send an initial buffer
	//iResult = send(SimpleNet::ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	/*if (iResult == SOCKET_ERROR) 
	{
		closesocket(SimpleNet::ConnectSocket);
		WSACleanup();
		return false;
	}*/

	// shutdown the connection since no more data will be sent
	/*iResult = shutdown(SimpleNet::ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		closesocket(SimpleNet::ConnectSocket);
		WSACleanup();
		return false;
	}*/

	isConnected_ = true;
	return true;
}

bool SimpleNetClient::Connect(std::string ip)
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
	iResult = getaddrinfo(ip.c_str(), "15015", &hints, &result);
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

	// Send an initial buffer
	//iResult = send(SimpleNet::ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	/*if (iResult == SOCKET_ERROR)
	{
	closesocket(SimpleNet::ConnectSocket);
	WSACleanup();
	return false;
	}*/

	// shutdown the connection since no more data will be sent
	/*iResult = shutdown(SimpleNet::ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
	closesocket(SimpleNet::ConnectSocket);
	WSACleanup();
	return false;
	}*/

	game::ServerUI.getSectionRef(1).setVar(1, "True");
	isConnected_ = true;
	return true;
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
		memset(msgBuffer, 0x00, sizeof(msgBuffer));
		int iResult = recv(SimpleNet::ConnectSocket, msgBuffer, BufferLength, NULL);
		if (iResult == 0)
		{
			game::SlideUI.addSlide("MSG:Disconnected");
			isConnected_ = false;
			game::ServerUI.getSectionRef(1).setVar(1, "False");
			Log("Server Disconnected");
			isExit_ = true;
		}else if (iResult > 0)
		{
			/*msg << msgBuffer;
			Packet nMsg;
			msg >> nMsg.name;
			stringstream tStream;
			tStream << msg.rdbuf();
			nMsg.data = tStream.str();*/
			std::string msg = msgBuffer;
			Do(msg);
		}
		/*else
		{
			game::SlideUI.addSlide("MSG:Server Error");
			game::ServerUI.getSectionRef(1).setVar(1, "False");
			Log("Error:Server Closed");
			isConnected_ = false;
			return;
		}*/
		msg.str(string());
	}
	game::SlideUI.addSlide("MSG:Network Loop Ended");
}

void SimpleNetClient::Send(std::string msg)
{
	stringstream cMsg;
	if (isHost_)
	{
		cMsg << SendPlayer << endl;
	}
	else
	{
		cMsg << SendHost << endl;
	}
	cMsg << msg;
	send(SimpleNet::ConnectSocket, cMsg.str().c_str(), cMsg.str().length(), NULL);
}

void SimpleNetClient::Send(Packet msg)
{
	stringstream sMsg;
	if (msg.name == SendPlayer)
	{
		sMsg << msg.name << endl;
		sMsg << msg.data;
	}
	else if (msg.name == SendHost)
	{
		sMsg << msg.send << endl;
		sMsg << msg.name << endl;
		sMsg << msg.data;
	}
	else if (msg.name == SendDefault)
	{
		sMsg << msg.send << endl;
		sMsg << msg.name << endl;
		sMsg << msg.data;
		send(SimpleNet::ConnectSocket, sMsg.str().c_str(), sMsg.str().length(), NULL);
	}
}

void SimpleNetClient::SendLiteral(std::string msg)
{
	if (isConnected_ == false) return;
	int amount = msg.length();
	int amountSent = 0;
	int result;
	std::string cur;
	result = send(SimpleNet::ConnectSocket, msg.c_str(), msg.length(), NULL);
	if (result != amount)
	{
		amountSent += result;
		while (amountSent < amount)
		{
			cur = msg.substr(amountSent, msg.length());
			result = send(SimpleNet::ConnectSocket, cur.c_str(), cur.length(), NULL);
			amountSent += result;
		}
	}
}

void SimpleNetClient::Do(std::string rMsg)
{
	int loopLimit = 1000;
	int curLoop = 0;
	std::stringstream msg;
	msg << rMsg;
	int name;
	msg >> name;
	while (msg)
	{
		if (name == SetSelected)
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
		else if (name == UpdatePlayerPosition)
		{
			string name;
			msg >> name;
			int x;
			int y;
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
			Log("UpdatePlayerPosition\n");
		}
		else if (name == PacketNames::UpdateTile)
		{
			Tile tile;
			string null;
			msg >> null;
			tile.deserialize(msg);
			game::game.setTileAt(tile.getPos(), tile);
			Log("UpdateTile\n");
		}
		else if (name == AddPlayer)
		{
			//Log("//////////////////\n" + msg.str() + "///////////////////");
			Player player;
			std::string null;
			msg >> null;
			player.deserialize(msg);
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
			game::pHandler.addLocalPlayer(player);
			//Log("Local Player Added - " + player.getName());
			Log("AddPlayerLocal\n");
		}
		else if (name == GetWorld)
		{
			std::string world = game::game.getWorld();
			std::stringstream wMsg; // World Msg
			wMsg << SendDefault << endl;
			wMsg << World << endl;
			wMsg << world << endl;
			wMsg << "End" << endl;
			SendLiteral(wMsg.str());
			Log("GetWorld\n");
		}
		else if (name == World)
		{
			game::game.loadWorldServer(msg);
			Log("World\n");
		}
		else if (name == SetHost)
		{
			isHost_ = true;
			isHostChosen_ = true;
			Log("SetHost\n");
		}
		else if (name == SetPlayer)
		{
			isHost_ = false;
			isHostChosen_ = true;
			Log("SetPlayer\n");
		}
		else if (name == WaitingPlayer)
		{

		}
		else if (name == PlayerConnect)
		{
			isPlayerConnected_ = true;
			Log("PlayerConnect\n");
		}
		else if (name == PacketNames::Start)
		{
			isWaiting_ = false;
			Log("Start\n");
		}
		else
		{
			Log("SimpleNet: Unknown Packet Name: " + name);
		}
		name = None;
		msg >> name;
	}
	return;
}

void SimpleNetClient::Do(Packet msg)
{
	Log("Network Recieved:" + msg.data + "\nEnd\n");
	if (msg.name == SetSelected)
	{
		stringstream data; data << msg.data;
		int posX;
		int posY;
		data >> posX;
		data >> posY;
		Position pos;
		pos.setX(posX);
		pos.setY(posY);
		game::game.setTileAsSelectedS(pos);
	}
	else if (msg.name == RemoveSelected)
	{
		stringstream data; data << msg.data;
		int posX;
		int posY;
		data >> posX;
		data >> posY;
		Position pos;
		pos.setX(posX);
		pos.setY(posY);
		game::game.removeSelectedAtTileS(pos);
	}
	else if (msg.name == SetTile)
	{
		stringstream data; data << msg.data;
		Tile tile;
		tile.deserialize(data);
		game::game.setTileAtS(tile.getPos(), tile);
	}
	else if (msg.name == GetWorld)
	{

	}
	else if (msg.name == World)
	{

	}
	else if (msg.name == SetHost)
	{
		isHost_ = true;
		isHostChosen_ = true;
	}
	else if (msg.name == SetPlayer)
	{
		isHost_ = false;
		isHostChosen_ = true;
	}
	else
	{
		Log("Error: Unknown Packet Name : " + msg.name);
	}
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
