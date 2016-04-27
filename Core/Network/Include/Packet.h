#pragma once
#include <iostream>
#include <string>
#include <sstream>

#define EndLine "\n"

enum PacketNames
{
	None, Sound, MultiPackets, SetHost, SetPlayer, GetWorld, World, SetTile, SetSelected, RemoveSelected, Message, SendPlayer, SendHost, SendDefault,

	UpdatePlayerPosition, UpdateTile, UpdateTileChar, UpdateTileColor, UpdateTileHealth, UpdateTileOverlay, UpdateTileClaim,

	UpdatePlayer, Health,

	EntityDamage, EntityKill, EntityUpdatePosition,
	
	EntityAdd, EBullet, ETurret, ECore, 

	DamagePlayer, HealPlayer, KillPlayer,

	AddPlayer, AddPlayerLocal,

	WaitingPlayer, Start,

	PlayerConnect,

	Lobby, LobbyAdd, LobbyName, LobbyReady, LobbyUnReady, LobbyStart, LobbyLeave, LobbyGetInfo,
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

