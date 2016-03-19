#include "..\include\SimpleNetClient.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>

SimpleNetClient::SimpleNetClient()
{
	isExit_ = false;
	ConnectSocket = INVALID_SOCKET;
}


SimpleNetClient::~SimpleNetClient()
{
}

bool SimpleNetClient::Start()
{
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf = "this is a test";
	char recvbuf[512];
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
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) 
		{
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) 
	{
		WSACleanup();
		return false;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) 
	{
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}

	return 0;
}

void SimpleNetClient::Loop()
{
	int BufferLength = 512;
	std::stringstream msg;
	char msgBuffer[512];
	while (isExit_ == false)
	{
		memset(msgBuffer, 0x00, sizeof(msgBuffer));
		int iResult = recv(ConnectSocket, msgBuffer, 512, NULL);
		msg << msgBuffer;
		Do(msg.str());
		msg.str(std::string());
	}
}

void SimpleNetClient::Send(std::string msg)
{
	char msgBuffer[512];
	msg.copy(msgBuffer, 0, msg.length());
	send(ConnectSocket, msgBuffer, 512, NULL);
}

void SimpleNetClient::Do(std::string rMsg)
{
	std::stringstream msg;
	msg << rMsg;
	std::string key;
	msg >> key;
	while (key != "End")
	{
		if (key == "Tile")
		{

		}
		else if (key == "Underlord")
		{

		}
	}
	return;
}
