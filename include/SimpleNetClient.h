#pragma once
#include <string>
#include "..\Dungeon Keeper\include\Packet.h"

class SimpleNetClient
{
public:
	SimpleNetClient();
	~SimpleNetClient();

	bool Start();
	bool Connect(std::string ip);
	void Loop();
	void Send(std::string msg);
	void Send(Packet msg);
	void SendLiteral(std::string msg);
	void Do(std::string rMsg);
	void Do(Packet msg);
	void Close();
	void Pause();
	void Continue();
	void isExit(bool isExit);
	bool Initialize();
	bool isConnected();
	bool isPaused();
	void UpdateTile(int x, int y);
	void Log(std::string text);
private:
	bool isExit_;
	bool isConnected_;
	bool isHost_;
	bool isHostChosen_;
	bool isPaused_;
};

