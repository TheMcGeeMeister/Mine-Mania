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
#include "..\Dungeon Keeper\include\Packet.h"

namespace game
{
	extern Display game;
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
			msg << msgBuffer;
			Packet nMsg;
			uint16_t name;
			msg >> name;
			stringstream tStream;
			tStream << msg.rdbuf();
			nMsg.data = tStream.str();
			Do(nMsg);
			//Log(msg.str());
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
	send(SimpleNet::ConnectSocket, msg.c_str(), msg.length(), NULL);
}

void SimpleNetClient::Do(std::string rMsg)
{
	Log(rMsg);
	int loopLimit = 1000;
	int curLoop = 0;
	std::stringstream msg;
	msg << rMsg;
	std::string key;
	msg >> key;
	if (rMsg.find("End", 0) == std::string::npos)
	{
		Log("Error:No End Found");
		return;
	}
	while (key != "End")
	{
		if (key == "World")
		{
			std::stringstream remainder;
			remainder << msg.rdbuf();
			game::game.loadWorldServer(msg);
			return;
		}
		else if (key == "SETHOST")
		{
			isHost_ = true;
			isHostChosen_ = true;
		}
		else if (key == "SETPLAYER")
		{
			isHost_ = false;
			isHostChosen_ = true;
		}
		else if (key == "MSG")
		{
			string text;
			msg >> text;
			game::SlideUI.addSlide(text);
		}
		else if (key == "GetWorld")
		{
			stringstream nMsg;
			nMsg << "SendHost\n";
			nMsg << "World\n";
			nMsg << game::game.getWorld();
			nMsg << "End";
			SendLiteral(nMsg.str());
		}
		else if (key == "Tile")
		{
			Tile tile;
			tile.deserialize(msg);
			game::game.setTileAtS(tile.getPos(), tile);
		}
		else if (key == "SetSelected")
		{
			int pos_x;
			int pos_y;
			msg >> pos_x;
			msg >> pos_y;
			Position pos;
			pos.setX(pos_x);
			pos.setY(pos_y);
			game::game.setTileAsSelectedS(pos);
		}
		else if (key == "RemoveSelected")
		{
			int pos_x;
			int pos_y;
			msg >> pos_x;
			msg >> pos_y;
			Position pos;
			pos.setX(pos_x);
			pos.setY(pos_y);
			game::game.removeSelectedAtTileS(pos);
		}
		key = "";
		msg >> key;
		curLoop++;
		if (loopLimit == curLoop)
		{
			Log("Error: Loop Limit Hit:\n");
			Log(rMsg);
			Log("----------------------------------");
			return;
		}
	}
	return;
}

void SimpleNetClient::Do(Packet msg)
{
	Log(msg.data);
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
