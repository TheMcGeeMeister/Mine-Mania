#pragma once
#include <iostream>
#include <sstream>

enum SPacketNames
{
	None, Sound, MultiPackets, SetHost, SetPlayer, GetWorld, World, SetTile, SetSelected, RemoveSelected, SendPlayer, SendHost, SendDefault,

	UpdatePlayerPosition, UpdateTile,

	UpdatePlayer,

	PlayerUpdate,
	///////////////////	
	Knockback,
	Health,
	Kill,
	Exp,
	Stats,
	PSync,
	///////////////////
	EntityDamage, EntityKill, EntityUpdatePosition,

	EntityAdd, EBullet, ETurret, ECore,

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

class SPacket
{
public:
	SPacket();
	~SPacket();

	uint16_t name;
	std::stringstream data;
};

