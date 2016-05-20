#pragma once
#include <iostream>
#include <string>
#include <sstream>

#define EndLine "\n"

enum PacketNames
{
	None, Sound, MultiPackets, SetHost, SetPlayer, GetWorld, World, SetTile, SetSelected, RemoveSelected, Message, SendPlayer, SendHost, SendDefault,

	UpdatePlayerPosition, 
	
	UpdateTile,

	UpdatePlayer,

	TileUpdate,
	///////////////////	
	Claim,
	///////////////////	

	PlayerUpdate,
	///////////////////	
	Knockback,
	Health,
	Kill,
	PSync,
	///////////////////
	EntityDamage, EntityKill, EntityUpdatePosition,

	EntityAdd,
	///////////////////
	EBullet,
	ETurret,
	ECore,
	EGoldSpawn,
	///////////////////

	AddPlayer, AddPlayerLocal,

	WaitingPlayer, Start,

	Win,

	PlayerConnect, ServerDisconnected,

	Lobby,
	///////////////////
	LobbyAdd,
	LobbyName, 
	LobbyReady, 
	LobbyUnReady, 
	LobbyStart, 
	LobbyLeave, 
	LobbyGetInfo,
	///////////////////
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
