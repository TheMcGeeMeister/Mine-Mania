#pragma once
#include <iostream>
#include <sstream>

enum SPacketNames
{
	None, MultiPackets, SetHost, SetPlayer, GetWorld, World, SetTile, SetSelected, RemoveSelected, Message, SendPlayer, SendHost, SendDefault,

	UpdatePlayerPosition, UpdateTile, UpdateTileChar, UpdateTileColor,

	AddPlayer, AddPlayerLocal,

	WaitingPlayer, Start,

	PlayerConnect,

	PacketEnd = -255
};

class SPacket
{
public:
	SPacket();
	~SPacket();

	uint16_t name;
	std::stringstream data;
};

