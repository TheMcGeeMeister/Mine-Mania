#pragma once
#include <string>
#include "Packet.h"

class SimpleNetClient
{
public:
	SimpleNetClient();
	~SimpleNetClient();

	bool Start();
	bool Connect(std::string ip);
	void Loop();
	void SendLiteral(std::string& msg);
	void SetId(int id);
	void Do(std::string rMsg);
	void Close();
	void Pause();
	void Continue();
	void isExit(bool isExit);
	bool Initialize();
	bool isConnected();
	bool isPaused();
	bool isWaiting();
	bool isPlayerConnected();
	bool isHost();
	void UpdateTile(int x, int y);
	void Log(std::string text);

	int getId();
private:
	bool isExit_;
	bool isConnected_;
	bool isHost_;
	bool isHostChosen_;
	bool isPaused_;
	bool isWaiting_;
	bool isPlayerConnected_;

	int id_;
};
