#include <sstream>
#include <Windows.h>
#include "Lobby.h"
#include "SimpleNetClient.h"
#include "PlayerHandler.h"
#include "Common.h"
#include "Display.h"
#include "Entity.h"
#include "Message.h"

namespace game
{
	extern SimpleNetClient server;
	extern PlayerHandler pHandler;
	extern Display game;
	extern System system;
}

Lobby::Lobby() : UI(30, 10, 0, 0, 1)
{
	started_ = false;
	isReady_ = false;
	isHost_ = false;
	isDrawListCalled_ = false;
	player_amount = 1;
}

Lobby::~Lobby()
{
}

void Lobby::Initialize(bool isHost)
{
	isHost_ = isHost;
	UI.push_isection("Name:");
	UI.push_back("Ready Up", true, true);
	if (isHost)
	{
		UI.push_back("Start", true, true);
	}

	UI.getSectionRef(1).setIVar(game::pHandler.getLocalPlayer().getName());

	m_players[game::server.getId()].first = game::pHandler.getLocalPlayer().getName();
	m_players[game::server.getId()].second = false;
}

bool Lobby::Go()
{
	UI.isHidden(false);
	DrawList();
	if (isHost_ == false)
	{
		std::stringstream msg;
		msg << SendDefault << EndLine
			<< PacketNames::Lobby << EndLine
			<< LobbyGetInfo << EndLine
			<< game::server.getId();
		SendServerLiteral(msg.str());
		msg.str(string());
		msg << SendDefault << EndLine
			<< PacketNames::Lobby << EndLine
			<< LobbyAdd << EndLine
			<< game::pHandler.getLocalPlayer().getName() << EndLine
			<< false << EndLine
			<< game::server.getId();
		SendServerLiteral(msg.str());
	}

	bool exitFlag = false;
	while (exitFlag == false)
	{
		UI.update();
		if (UI.isSectionActivated())
		{
			int selected = UI.getActivatedSection();
			if (selected == 1)
			{
				PlayerChangeName(game::server.getId(), UI.getSectionRef(1).getIVar());
				std::stringstream msg;
				msg << SendDefault << EndLine
					<< PacketNames::Lobby << EndLine
					<< LobbyName << EndLine
					<< UI.getSectionRef(1).getIVar() << EndLine
					<< game::server.getId() << EndLine;
				game::server.SendLiteral(msg.str());
				game::pHandler.getLocalPlayer().setName(UI.getSectionRef(1).getIVar());
				DrawList();
			} // Name
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
					UI.getSectionRef(2).setText("Ready Up");
					isReady_ = false;
					m_players[game::server.getId()].second = false;
				}
				else
				{
					msg << SendDefault << EndLine
						<< PacketNames::Lobby << EndLine
						<< LobbyReady << EndLine
						<< game::server.getId() << EndLine;
					game::server.SendLiteral(msg.str());
					UI.getSectionRef(2).setText("UnReady");
					isReady_ = true;
					m_players[game::server.getId()].second = true;
				}
				DrawList();
			} // Ready up
			else if (selected == 3)
			{
				bool isStartReady = true;
				for (auto& iter : m_players)
				{
					if (iter.second.second == false)
					{
						isStartReady = false;
					}
				} // Check if all players are ready
				if (isStartReady)
				{
					std::string names[4];
					int x = 0;
					for (auto& iter : m_players)
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
						std::fstream stream("Logs\\Log.txt");
						for (auto& iter : m_players)
						{
							stream << "LOOKING FOR:" << iter.second.first << "\n";
							if (iter.second.first == local) continue;
							if (game::pHandler.getPlayer(iter.second.first, &player))
							{
								Common::SendPlayer(player, player_amount, iter.first);
							}
							else
							{
								stream << "ERROR: " << iter.second.first << " NOT FOUND\n";
							}
						}
						//////////////////////////

						/* World Sync */
						//////////////////////
						std::stringstream msg;
						msg << SendDefault << EndLine << World << EndLine << game::game.getWorld();
						SendServerLiteral(msg.str());
						msg.str(string());
						//////////////////////

						/* Entity Sync */
						//////////////////////
						game::system.sendAll();
						//////////////////////

						msg << SendDefault << EndLine << PacketNames::Lobby << EndLine << LobbyStart << EndLine;
						SendServerLiteral(msg.str());
						started_ = true;
					}
				}
			} // Start(Host Only)
		}
		if (started_ == true)
		{
			std::stringstream log_msg;
			log_msg << "Multiplayer Started With ID:" << game::pHandler.getLocalPlayer().getID() << EndLine;
			Common::Log(log_msg.str());
			return true;
		}
		if (isDrawListCalled_ == true)
		{
			isDrawListCalled_ = false;
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
			for (auto& iter : m_players)
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
		Sleep(10);
	}
	return true;
}

void Lobby::AddPlayer(int id, std::string name, bool isReady)
{
	player_amount++;
	m_players[id].first = name;
	m_players[id].second = isReady;
	DrawList();
}

void Lobby::RemovePlayer(std::string name)
{
	// TO:DO Implement
	DrawList();
}

void Lobby::PlayerChangeName(int id, std::string nName)
{
	if (m_players.count(id))
	{
		m_players[id].first = nName;
	}
	DrawList();
}

void Lobby::PlayerSetName(int id, std::string name)
{
	if (m_players.count(id))
	{
		m_players[id].first = name;
	}
	DrawList();
}

void Lobby::PlayerReady(int id)
{
	if (m_players.count(id))
	{
		m_players[id].second = true;
	}
	DrawList();
}

void Lobby::PlayerUnReady(int id)
{
	if (m_players.count(id))
	{
		m_players[id].second = false;
	}
	DrawList();
}

void Lobby::Start()
{
	started_ = true;
}

void Lobby::DrawList()
{
	isDrawListCalled_ = true;
}

bool Lobby::isReady()
{
	return isReady_;
}

void Lobby::isReady(bool is)
{
	isReady_ = is;
}