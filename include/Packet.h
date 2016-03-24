#pragma once
#include <iostream>
#include <string>
#include <sstream>

#define End "\n"

enum PacketNames
{
	None, MultiPackets, SetHost, SetPlayer, GetWorld, World, SetTile, SetSelected, RemoveSelected, Message, SendPlayer, SendHost, SendDefault,

	UpdatePlayerPosition, UpdateTile, UpdateTileChar, UpdateTileColor, UpdateTileHealth,

	AddPlayer, AddPlayerLocal,

	WaitingPlayer, Start,

	PlayerConnect,

	PacketEnd = -255
};

class Packet
{
public:
	Packet();
	~Packet();

	uint16_t name;
	uint16_t send;
	std::string data;
};

