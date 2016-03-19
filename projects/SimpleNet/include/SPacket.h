#pragma once
#include <iostream>
#include <sstream>

enum SPacketNames
{
	None, MultiPackets, SetHost, SetPlayer, GetWorld, World, SetTile, SetSelected, RemoveSelected, SendPlayer, SendHost, SendDefault
};

class SPacket
{
public:
	SPacket();
	~SPacket();

	uint16_t name;
	std::stringstream data;
};

