#include <sstream>
#include <Windows.h>
#include "Lobby.h"
#include "SimpleNetClient.h"
#include "PlayerHandler.h"
#include "Common.h"
#include "Display.h"

namespace game
{
	extern SimpleNetClient server;
	extern PlayerHandler pHandler;
	extern Display game;
}

Lobby::Lobby() : ui(30, 10, 0, 0, 1)
{
	started_ = false;
	isReady_ = false;
	isHost_ = false;
	player_amount = 1;
}

Lobby::~Lobby()
{
}

void Lobby::Initialize(bool isHost)
{
	isHost_ = isHost;
	ui.push_isection("Name:");
	ui.push_back("Ready Up", true, true);
	if (isHost)
	{
		ui.push_back("Start", true, true);
		//ui.push_back("Load", true, true);
	}

	ui.getSectionRef(1).setIVar(game::pHandler.getLocalPlayer().getName());

	m_players_t[game::server.getId()].first = game::pHandler.getLocalPlayer().getName();
	m_players_t[game::server.getId()].second = false;
}

bool Lobby::Go()
{
	DrawList();
	if (isHost_ == false)
	{
		std::stringstream msg;
		msg << SendDefault << EndLine
			<< PacketNames::Lobby << EndLine
			<< LobbyGetInfo << EndLine
			<< game::server.getId() << EndLine;
		game::server.SendLiteral(msg.str());
		msg.str(string());
		msg << SendDefault << EndLine
			<< PacketNames::Lobby << EndLine
			<< LobbyAdd << EndLine
			<< game::pHandler.getLocalPlayer().getName() << EndLine
			<< false << EndLine
			<< game::server.getId() << EndLine;
		game::server.SendLiteral(msg.str());
	}
	Sleep(250);
	bool exitFlag = false;
	ui.isHidden(false);
	while (exitFlag == false)
	{
		ui.update();
		if (ui.isSectionActivated())
		{
			int selected = ui.getActivatedSection();
			if (selected == 1)
			{
				PlayerChangeName(game::server.getId(), ui.getSectionRef(1).getIVar());
				std::stringstream msg;
				msg << SendDefault << EndLine
					<< PacketNames::Lobby << EndLine
					<< LobbyName << EndLine
					<< ui.getSectionRef(1).getIVar() << EndLine
					<< game::server.getId() << EndLine;
				game::server.SendLiteral(msg.str());
				game::pHandler.getLocalPlayer().setName(ui.getSectionRef(1).getIVar());
				DrawList();
			}
			else if (selected == 2)
			{
				std::stringstream msg;
				if (isReady_)
				{
					msg << SendDefault << EndLine
						<< PacketNames::Lobby << EndLine
						<< LobbyUnReady << EndLine
						<< game::server.getId() << EndLine;
					game::server.SendLiteral(msg.str());
					ui.getSectionRef(2).setText("Ready Up");
					isReady_ = false;
					m_players_t[game::server.getId()].second = false;
				}
				else
				{
					msg << SendDefault << EndLine
						<< PacketNames::Lobby << EndLine
						<< LobbyReady << EndLine
						<< game::server.getId() << EndLine;
					game::server.SendLiteral(msg.str());
					ui.getSectionRef(2).setText("UnReady");
					isReady_ = true;
					m_players_t[game::server.getId()].second = true;
				}
				DrawList();
			}
			else if (selected == 3)
			{
				bool isStartReady = true;
				for (auto& iter : m_players_t)
				{
					if (iter.second.second == false)
					{
						isStartReady = false;
					}
				}
				if (isStartReady)
				{
					std::string names[4];
					int x = 0;
					for (auto& iter : m_players_t)
					{
						names[x] = iter.second.first;
						x++;
					}
					if (game::game.isLoaded() == false || game::game.isLoadedMultiplayer() == false)
					{
						CreateMultiplayerWorld(player_amount, names);
						std::stringstream world;
						world << SendDefault << EndLine << World << EndLine << game::game.getWorld();
						SendServerLiteral(world.str());
						std::stringstream startMsg;
						startMsg << SendDefault << EndLine << PacketNames::Lobby << EndLine << LobbyStart << EndLine;
						SendServerLiteral(startMsg.str());
						started_ = true;
					}
					else
					{
						/* Load Players */
						//////////////////////////
						Player* player = &game::pHandler.getLocalPlayer();
						std::string local = game::pHandler.getLocalPlayer().getName();
						Common::SendPlayer(player, player_amount, 0);
						for (auto& iter : m_players_t)
						{
							if (iter.second.first == local) continue;
							if (game::pHandler.getPlayer(iter.second.first, &player))
							{
								Common::SendPlayer(player, player_amount, iter.first);
							}
						}
						//////////////////////////
						std::stringstream msg;
						msg << SendDefault << EndLine << World << EndLine << game::game.getWorld();
						SendServerLiteral(msg.str());
						msg.str(string());
						msg << SendDefault << EndLine << PacketNames::Lobby << EndLine << LobbyStart << EndLine;
						SendServerLiteral(msg.str());
						started_ = true;
					}
				}
			}
			/*else if (selected == 4) Load
			{
			}*/
		}
		if (started_ == true)
		{
			return true;
		}
		Sleep(10);
	}
}

void Lobby::AddPlayer(int id, std::string name, bool isReady)
{
	player_amount++;
	m_players_t[id].first = name;
	m_players_t[id].second = isReady;
	DrawList();
}

void Lobby::RemovePlayer(std::string name)
{
	// TO:DO Implement
	DrawList();
}

void Lobby::PlayerChangeName(int id, std::string nName)
{
	if (m_players_t.count(id))
	{
		m_players_t[id].first = nName;
	}
	DrawList();
}

void Lobby::PlayerSetName(int id, std::string name)
{
	if (m_players_t.count(id))
	{
		m_players_t[id].first = name;
	}
	DrawList();
}

void Lobby::PlayerReady(int id)
{
	if (m_players_t.count(id))
	{
		m_players_t[id].second = true;
	}
	DrawList();
}

void Lobby::PlayerUnReady(int id)
{
	if (m_players_t.count(id))
	{
		m_players_t[id].second = false;
	}
	DrawList();
}

void Lobby::Start()
{
	started_ = true;
}

void Lobby::DrawList()
{
	int y = 11;
	std::stringstream txt;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD length;
	DWORD output;
	COORD pos;
	COORD nPos;
	WORD Attribute = 7;
	pos.X = 0;
	nPos.X = 0;
	nPos.Y = 11;
	pos.Y = 11;
	for (auto& iter : m_players_t)
	{
		pos.Y = y;
		nPos.Y = y;
		txt << iter.second.first;
		if (iter.second.second == true)
		{
			txt << ": Ready      ";
		}
		else
		{
			txt << ": Not Ready       ";
		}
		WriteConsoleOutputCharacter(h, txt.str().c_str(), txt.str().length(), pos, &length);
		for (int x = 0; x < length; x++)
		{
			nPos.X = x;
			WriteConsoleOutputAttribute(h, &Attribute, 1, nPos, &output);
		}
		y++;
		txt.str(string());
	}
}

bool Lobby::isReady()
{
	return isReady_;
}

void Lobby::isReady(bool is)
{
	isReady_ = is;
}