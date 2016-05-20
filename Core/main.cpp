#include <iostream>
#include <random>
#include <thread>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <time.h>
#include <atomic>
#include "game.h"
#include "LoadEnums.h"
#include "Common.h"
#include "GameManager.h"
#include "Timer.h" // Timer utility class
#include "Player.h" // Player Class
#include "Tile.h" // Tile Class
#include "Lobby.h" // Multiplayer Lobby Class
#include "Wave.h" // Sound Class
#include "Display.h" // Main Class For Display, Loading/Saving
#include "TileEnums.h" // Tile Colors, Graphics..etc..
#include "RegenManager.h" // Idk if this is even used but it was used for handling regen
#include "SoundManager.h" // Sound Mananger Class
#include "UserInterface.h" // UI used by all selections, and menus
#include "PlayerHandler.h" // Updates all the players
#include "SimpleNetClient.h" // Server class
#include "PositionVariables.h" // Holds on size, and offset
#include "TileChangeManager.h" // Updates the Display with changes
#include "LoadParser.h"

#define DEFAULT_CLEAR_WIDTH 100
#define DEFAULT_CLEAR_HEIGHT 36

using namespace std;

bool pauseGameMenu(Display& game);

namespace game
{
	System system;
    TileChangeManager TileHandler;
    RegenManager RegenHandler;
	PlayerHandler pHandler;
	GameManager GameHandler;
	Player enemy;
	Display game;
	UserInterface tileUI(30, 8, 0, 30, 1);
	UserInterface SlideUI(25, 15, 75, 0, 1);
	UserInterface ServerUI(18, 3, 75, 27, 1);
	UserInterface statsUI(16, 5, 32, 30, 1);
	SimpleNetClient server;
	SoundManager m_sounds;
	LoadParser Parser;
	std::string winnerName;
	class Lobby lobby;
	bool threadExit;
	bool lobbyStart = false;
	bool gameWon = false;
	std::atomic<bool> exitFromDisconnect = false;
	int curFont;
	void Log(string txt)
	{
		fstream file("Logs\\Log.txt", ios::app);
		file << txt << endl;
	}

	int CoreDestroyedAmount = 0;
}

namespace gametiles
{
	Tile stone_wall;
	Tile stone_wall_gold;
	Tile stone_floor;
	Tile dirt_wall;
}

void clearInput()
{
	while (_kbhit())
	{
		_getch();
	}
}

void updateTile(Position pos)
{
	game::game.updateTileServer(pos);
}

bool isExit()
{
    if(GetAsyncKeyState(VK_ESCAPE))
        return true;
    else
        return false;
}

bool isExitGame(Display& game)
{
	/* Any Extra Checks for Exiting*/
	///////////////////////////////////////////
	if (game::exitFromDisconnect == true)
	{
		game::exitFromDisconnect = false;
		return true;
	}
	if (game::gameWon == true)
	{
		game::pHandler.getLocalPlayer().stopSounds();
		game::ServerUI.isHidden(true);
		game::tileUI.isHidden(true);
		game::SlideUI.isHidden(true);
		game::statsUI.isHidden(true);
		game::pHandler.getLocalPlayer().getUIRef().isHidden(true);
		game::game.isHidden(true);
		std::stringstream winnerTxt;
		winnerTxt << "Player " << game::winnerName << " won the game!";
		//Common::DisplayTextCentered(game::game.getWidth(), game::game.getHeight() / 2, winnerTxt.str());
		Common::SetCursorPosition(0, 0);
		cout << winnerTxt.str();
		if (game::pHandler.getLocalPlayer().getName() == game::winnerName)
		{
			game::m_sounds.PlaySoundR("Winner");
		}
		else
		{
			game::m_sounds.PlaySoundR("Loser");
		}
		game::game.unloadWorld();
		Sleep(4000);
		game::gameWon = false;
		return true;
	}
	///////////////////////////////////////////
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		bool exit = false;
		game::ServerUI.isHidden(true);
		game::tileUI.isHidden(true);
		game::SlideUI.isHidden(true);
		game::statsUI.isHidden(true);
		game::pHandler.getLocalPlayer().getUIRef().isHidden(true);
		exit = pauseGameMenu(game);
		if (exit == false)
		{
			game::ServerUI.isHidden(false);
			game::tileUI.isHidden(false);
			game::SlideUI.isHidden(false);
			game::statsUI.isHidden(false);
			game::pHandler.getLocalPlayer().updateMiningUI();
			clearInput();
		}
		return exit;
	}
	else
	{
		return false;
	}
}

void fillLine(int x, int y)
{
    COORD pos;
	pos.X = 0;
	pos.Y = y;
	DWORD nlength = x;
	DWORD output;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	TCHAR graphic = ' ';
	WORD color = 0;
	FillConsoleOutputCharacter(h, graphic, nlength, pos, &output);
	FillConsoleOutputAttribute(h, color, nlength, pos, &output);
}

void clearScreenPart(int _x, int _y)
{
    for(int y=0;y<_y;y++)
    {
        fillLine(_x, y);
    }
}

void clearScreenCertain(int _x, int _y, int start_y)
{
	for (int y = start_y; y < _y; y++)
	{
		fillLine(_x, y);
	}
}

void setCursorPos(int x, int y)
{
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos={x,y};
    SetConsoleCursorPosition(h, pos);
}

void setFontInfo(int FontSize, int font)
{
	if (FontSize < 5 || FontSize > 72)
		FontSize = 28;
	if (font == 0)
	{
		CONSOLE_FONT_INFOEX info = { 0 };
		info.cbSize = sizeof(info);
		info.dwFontSize.Y = FontSize; // leave X as zero
		info.FontWeight = FW_NORMAL;
		wcscpy(info.FaceName, L"Lucida Console");
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
	}
	else
	{
		CONSOLE_FONT_INFOEX info = { 0 };
		info.cbSize = sizeof(info);
		info.dwFontSize.Y = FontSize; // leave X as zero
		info.FontWeight = FW_NORMAL;
		wcscpy(info.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
	}
}

void setFullsreen()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD flags = CONSOLE_FULLSCREEN_MODE;
	COORD pos;
	SetConsoleDisplayMode(h, flags, &pos);
}

void swapConsoleFullScreen()
{
	DWORD flags;
	GetConsoleDisplayMode(&flags);
	if (flags != CONSOLE_FULLSCREEN_MODE)
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD flags = CONSOLE_FULLSCREEN_MODE;
		COORD pos;
		SetConsoleDisplayMode(h, flags, &pos);
	}
	else
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD flags = CONSOLE_FULLSCREEN_HARDWARE;
		COORD pos;
		SetConsoleDisplayMode(h, flags, &pos);
	}
}

void initializeWindowProperties()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD flags = CONSOLE_FULLSCREEN_MODE;
	COORD pos;
	SetConsoleDisplayMode(h, flags, &pos);
}

void LOGIC(Timer& InputCoolDown, Display& game)
{
	Player& player = game::pHandler.getLocalPlayer();
	HANDLE h = GetActiveWindow();
	HANDLE focus = GetFocus();
	if (h != focus) { return; }
    if(InputCoolDown.Update()==true)
    {
		/* Movement */
		////////////////////////////////////////////////
		for (int x = 0; x < 3; x++)
		{
			if (kbhit())
			{
				int key = getch();
				if (key == 224)
					key = getch();
				switch (key)
				{
				case 'w':player.moveHand(DIRECTION_UP); InputCoolDown.StartNewTimer(0.075); break;
				case 's':player.moveHand(DIRECTION_DOWN); InputCoolDown.StartNewTimer(0.075); break;
				case 'a':player.moveHand(DIRECTION_LEFT); InputCoolDown.StartNewTimer(0.075); break;
				case 'd':player.moveHand(DIRECTION_RIGHT); InputCoolDown.StartNewTimer(0.075); break;
				case 'f':player.switchMode(); break;
				case 'c':player.purchaseTurret(); break;
				case 'v':player.claimOnHand(); InputCoolDown.StartNewTimer(0.075); break;
				case 'b':player.purchaseBullet(); break;
				case 72:player.mine(DIRECTION_UP); InputCoolDown.StartNewTimer(0.075); break;
				case 80:player.mine(DIRECTION_DOWN); InputCoolDown.StartNewTimer(0.075); break;
				case 75:player.mine(DIRECTION_LEFT); InputCoolDown.StartNewTimer(0.075); break;
				case 77:player.mine(DIRECTION_RIGHT); InputCoolDown.StartNewTimer(0.075); break;
				case '1':player.switchModeTo(0); break;
				case '2':player.switchModeTo(1); break;
				case '3':player.switchModeTo(2); break;
				case 'j':game::system.cleanAndUpdateOverlays(); break;
				default: break;
				}
			}
			else
				break;
		}
	}
}

void loadScreen(int time, string text)
{
	clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
	setCursorPos(0, 0);
	cout << text;
	Sleep(time);
	clearScreenPart(DEFAULT_CLEAR_WIDTH, 1);
	setCursorPos(0, 0);
	return;
}

void updateGameInterface()
{
	Player& player = game::pHandler.getLocalPlayer();
	Display& game = game::game;
	UserInterface& TileInfo = game::tileUI;
	std::stringstream health;
	Tile& tile = game.getTileRefAt(player.getHandPosition());
	health << game::pHandler.getLocalPlayer().getHealth() << "/" << game::pHandler.getLocalPlayer().getMaxHealth();
	TileInfo.getSectionRef(1).setVar(1, health.str());

	TileInfo.getSectionRef(2).setVar(1, tile.getClaimedBy());

	stringstream claimed;
	claimed << tile.getClaimedPercentage() << "%";
	TileInfo.getSectionRef(3).setVar(1, claimed.str());

	TileInfo.getSectionRef(4).setVar(1, player.getAmmoAmount());

	std::stringstream gold;
	gold << player.getGoldAmount();
	TileInfo.getSectionRef(5).setVar(1, gold.str());

	TileInfo.update();

	std::stringstream level;
	level << player.getLevel();
	game::statsUI.getSectionRef(2).setVar(1, level.str());

	std::stringstream exp;
	exp << player.getExp() << "/" << player.getMaxExp();
	game::statsUI.getSectionRef(3).setVar(1, exp.str());
}

void lobbyMenu()
{
	game::lobby.Go();
}

void connectMenu(thread& sThread, bool& threadStarted)
{
	Sleep(250);
	UserInterface ui(30, 9, 0, 0, 1);
	ui.drawBorder();
	ui.push_isection("Address:");
	ui.push_back("Continue" , true, true);
	ui.push_back("Return", true, true);

	ui.getSectionRef(1).setIVar("127.0.0.1");

	bool exitFlag = false;
	while (exitFlag == false)
	{
		ui.update();
		if (ui.isSectionActivated())
		{
			switch (ui.getActivatedSection())
			{
			case 1: continue; Sleep(250); break;
			case 2: {ui.isHidden(true); game::server.Initialize(); thread load(loadScreen, 500, "Loading..."); game::server.Connect(ui.getSectionRef(1).getIVar());
				setCursorPos(0, 0); load.join(); ui.isHidden(false);
				continue; break;}
			case 3: exitFlag = true; Sleep(250); break;
			}
		}
		Sleep(10);
	}
	ui.isHidden(true);
	if (game::server.isConnected() == true)
	{
		if (threadStarted == false)
		{
			game::server.isExit(false);
			sThread = thread(bind(&SimpleNetClient::Loop, &game::server));
			threadStarted = true;
		}
		while (game::server.isHostChoosen() == false)
		{
			Sleep(10);
		}
		game::lobby.Initialize(game::server.isHost());
		game::lobby.Go();
	}
	ui.isHidden(true);
	Sleep(50);
}

bool loadMenu(Display& game)
{
	clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
	Sleep(500);
	UserInterface menu(30, 9, 0, 0, 1);
	int worldAmount = game.getSaveAmount();
	bool exitFlag = false;
	if (worldAmount == 0)
	{
		menu.setBorderWidth(1); menu.setSizeX(30); menu.setSizeY(9);
		menu.drawBorder();
		menu.addSection("No Saves...", false, true);
		menu.addSection("Exit", true, true);
		while (exitFlag == false)
		{
			menu.update();
			if (menu.isSectionActivated())
			{
				clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
				exitFlag = true;
			}
		}
	}
	else
	{
		for (int i = 1; i <= worldAmount; i++)
		{
			stringstream filename;
			if (i < 10)
			{
				filename << "World" << "0" << i << ".dat";
				std::stringstream rFilename;
				rFilename << "Saves\\World" << "0" << i << ".dat";
				fstream file(rFilename.str());
				int isMulti = 0;
				file >> isMulti;
				file.close();
				if (isMulti == L_Multi)
				{
					filename << " (Multiplayer)";
				}
			}
			else
			{
				filename << "World" << i << ".dat";
				std::stringstream rFilename;
				rFilename << "Saves\\World" << i << ".dat";
				fstream file(rFilename.str());
				int isMulti = 0;
				file >> isMulti;
				file.close();
				if (isMulti == L_Multi)
				{
					filename << " (Multiplayer)";
				}
			}
			menu.addSection(filename.str(), true, true);
		}
		menu.addSection("Exit", true, true);
		if (worldAmount < 6) menu.setSizeY(9);
		else menu.setSizeY(worldAmount + 3);

	}
	menu.drawBorder();
	while (exitFlag == false)
	{
		menu.update();
		if (menu.isSectionActivated())
		{
			if (menu.getActivatedSection() == worldAmount + 1)
			{
				clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
				exitFlag = true;
				Sleep(250);
				return false;
				continue;
			}
			stringstream filename;
			if (menu.getActivatedSection()<10)
			{
				clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
				filename << "Saves\\" << "World" << "0" << menu.getActivatedSection() << ".dat";
				game.loadWorld(filename.str());
				exitFlag = true;
				Sleep(250);
				return true;
			}
			else
			{
				clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
				filename << "Saves\\" << "World" << menu.getActivatedSection() << ".dat";
				game.loadWorld(filename.str());
				exitFlag = true;
				Sleep(250);
				return true;
 			}
		}
		Sleep(10);
	}
	menu.isHidden(true);
	Sleep(250);
	return false;
}

void saveMenu(Display& game)
{
	Sleep(500);
	UserInterface menu(30, 9, 0, 0, 1);
	int worldAmount = game.getSaveAmount();
	bool exitFlag = false;
	if (worldAmount == 0)
	{
		menu.drawBorder();
		menu.addSection("New Save", true, true);
		menu.addSection("1.Exit", true, true);
		while (exitFlag == false)
		{
			menu.update();
			if (menu.isSectionActivated())
			{
				switch (menu.getActivatedSection())
				{
				case 1: game.saveWorld(); 
				case 2: exitFlag = true; break;
				}
			}
			Sleep(10);
		}
	}
	else
	{
		for (int i = 1; i <= worldAmount; i++)
		{
			stringstream filename;
			if (i < 10)
			{
				filename << "World" << "0" << i << ".dat";
			}
			else
			{
				filename << "World" << i << ".dat";
			}
			menu.addSection(filename.str(), true, true);
		}
		menu.addSection("New Save", true, true);
		menu.addSection("Exit", true, true);
		if(worldAmount > 5)
			menu.setSizeY(worldAmount + 4);
	}
	menu.drawBorder();
	while (exitFlag == false)
	{
		menu.update();
		if (menu.isSectionActivated())
		{
			if (menu.getActivatedSection() == worldAmount + 1)
			{
				exitFlag = true;
				game.saveWorld();
				continue;
			}
			if (menu.getActivatedSection() == worldAmount + 2)
			{
				exitFlag = true;
				continue;
			}
			stringstream filename;
			if (menu.getActivatedSection()<10)
			{
				filename << "Saves\\" << "World" << "0" << menu.getActivatedSection() << ".dat";
				game.saveWorld(filename.str());
				exitFlag = true;
			}
			else
			{
				filename << "Saves\\" << "World" << menu.getActivatedSection() << ".dat";
				game.saveWorld(filename.str());
				exitFlag = true;
			}
		}
		Sleep(10);
	}
	menu.isHidden(true);
	Sleep(250);
	return;
}

void settingsMenu()
{
	Sleep(250);
	UserInterface menu(30, 9, 0, 0, 1);
	menu.drawBorder();
	menu.push_isection("Font Size:");
	menu.addSection("Font:", true, false);
	menu.getSectionRef(2).addVar("");
	menu.addSection("Fullscreen:", true, false);
	menu.getSectionRef(3).addVar("");
	menu.push_isection("Name:");
	menu.addSection("Volume", true, false);
	menu.getSectionRef(5).addVar(game::game.getVolume());
	menu.addSection("Exit", true, false);

	menu.getSectionRef(4).setIVar(game::pHandler.getLocalPlayer().getName());

	bool exitFlag = false;
	
	bool isFullScreen = game::game.isFullScreen();
	int font = game::game.getFont();
	int fontSize = game::game.getFontSize();
	int volume = game::game.getVolume();
	string fontTxt;
	if (font == 0) { fontTxt = "Consolas"; }
	else { fontTxt = "Lucida Console"; }

	stringstream txt; txt << fontSize;
	menu.getSectionRef(1).setIVar(txt.str());
	menu.getSectionRef(2).setVar(1, fontTxt);

	if (isFullScreen) { menu.getSectionRef(3).setVar(1, "True"); }
	else { menu.getSectionRef(3).setVar(1, "False"); }
	
	while (exitFlag == false)
	{
		menu.update();
		if (menu.isSectionActivated())
		{
			switch (menu.getActivatedSection())
			{
			case 1:
				fontSize = atoi(menu.getSectionRef(1).getIVar().c_str());
				setFontInfo(fontSize, font); break;
			case 2:	
				if (font == 0) { fontTxt = "Lucida Console"; font = 1; setFontInfo(fontSize, font); }
				else { fontTxt = "Consolas"; font = 0; setFontInfo(fontSize, font); }
				menu.getSectionRef(2).setVar(1, fontTxt); break;
			case 3:
				if (isFullScreen) { isFullScreen = false; menu.getSectionRef(3).setVar(1, "False"); swapConsoleFullScreen(); }
				else { isFullScreen = true; menu.getSectionRef(3).setVar(1, "True"); setFullsreen(); menu.reDrawAll(); }
				break;
			case 4:
				game::pHandler.getLocalPlayer().setName(menu.getSectionRef(4).getIVar());
				break;
			case 5:
				volume == 100 ? volume = 0 : volume++;
				menu.getSectionRef(5).setVar(1, volume);
				game::m_sounds.SetVolume((double)volume / 100);
				break;
			case 6:
				exitFlag = true; break;
			default:
				break;
			}
		}
		Sleep(10);
	}
	menu.isHidden(true);
	game::game.setFont(font);
	game::game.setFontSize(fontSize);
	game::game.setVolume(volume);
	Sleep(250);
}

bool newWorldMenu(Display& game)
{
	Sleep(250);
	UserInterface NewWorld(30, 9, 0, 0, 1);
	NewWorld.drawBorder();
	NewWorld.push_isection("Name:");
	NewWorld.push_back("Continue", true, true);
	NewWorld.addSection("Exit", true, true);
	NewWorld.getSectionRef(1).setIVar(game::pHandler.getLocalPlayer().getName());
	bool exitFlag = false;
	while (exitFlag == false)
	{
		NewWorld.update();
		if (NewWorld.isSectionActivated())
		{
			switch (NewWorld.getActivatedSection())
			{
			case 2: {
				thread load(loadScreen, 500, "Loading...");
				game.newWorld();
				game::pHandler.getLocalPlayer().setName(NewWorld.getSectionRef(1).getIVar());
				Player &player = game::pHandler.getLocalPlayer();
				game::pHandler.addLocalPlayer(player);
				load.join();
				exitFlag = true; break;
				NewWorld.isHidden(true);
				return true;
			}
			case 3: exitFlag = true; NewWorld.isHidden(true); return false; break;
			default: break;
			}
		}
		Sleep(50);
	}
}

void gameNotLoadedMenu(Display& game)
{
	Sleep(250);
	clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
	UserInterface menu(30, 9, 0, 0, 1);
	menu.drawBorder();
	menu.addSection("No Game Loaded", false, true);
	menu.addSection("1.New Game", true, true);
	menu.addSection("2.Load Game", true, true);
	menu.addSection("3.Return", true, true);
	bool exitFlag = false;
	while (exitFlag == false)
	{
		menu.update();
		if (menu.isSectionActivated())
		{
			switch (menu.getActivatedSection())
			{
			case 2: menu.isHidden(true); newWorldMenu(game); Sleep(250); return; break;
			case 3: loadMenu(game); menu.isHidden(true); Sleep(250); return; break;
			case 4: menu.isHidden(true);  Sleep(250); return; break;
			}
		}
		Sleep(10);
	}
}

bool pauseGameMenu(Display& game)
{
	game.isHidden(true);
	game::tileUI.isHidden(true);
	UserInterface ui(30, 5, 0, 0, 1);
	ui.drawBorder();
	ui.push_back("Continue", true, true);
	ui.push_back("Save", true, true);
	ui.push_back("Exit", true, true);
	bool exitFlag = false;
	while (exitFlag == false)
	{
		ui.update();
		if (ui.isSectionActivated())
		{
			switch (ui.getActivatedSection())
			{
			case 1: game.reloadAll(); FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); return false;
			case 2: {ui.isHidden(true); thread load(loadScreen, 500, "Saving..."); game.saveWorld(); load.join(); ui.isHidden(false); break; }
			case 3: game.reloadAll(); FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); return true;
			}
		}
		Sleep(10);
	}
	game.isHidden(false);
	FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	return true;
}

bool pauseMenu(Display &game, thread& sThread, bool& threadStarted)
{
    clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
    UserInterface menu;
	PositionVariables pVar(30, 9, 0, 0);
	menu.setPositionVariables(pVar);
	menu.drawBorder();
    menu.addSection("Continue", true, false);
    menu.addSection("Save", true, false);
    menu.addSection("Load", true, false);
	menu.addSection("Settings", true, false);
    menu.addSection("New World", true, false);
	menu.addSection("Connect", true, false);
    menu.addSection("Exit", true, false);
    menu.update();
    bool exitFlag=false;
    while(exitFlag==false)
    {
        if(menu.isSectionActivated())
        {
            switch(menu.getActivatedSection())
            {
            case 1: // Continue
				return false; break;
            case 2: // Save
				if (game.isLoaded() == false) continue;
				menu.isHidden(true);
				saveMenu(game);
				Sleep(250);
				menu.isHidden(false);
				menu.reDrawAll();
				break;
            case 3: // Load
				menu.isHidden(true);
				if (loadMenu(game) == true)
				{
					if (game::game.isLoadedMultiplayer() == true)
					{
						menu.isHidden(true);
						connectMenu(sThread, threadStarted);
						if (game::server.isConnected() == true)
						{
							return false;
						}
						menu.isHidden(false);
					}
					else
					{
						return false;
					}
				}
				else{
					menu.isHidden(false);
					menu.reDrawAll();
				}
				break;
			case 4: // Settings
				menu.isHidden(true);
				settingsMenu();
				menu.isHidden(false);
				break;
            case 5: // New
				menu.isHidden(true); if(newWorldMenu(game) == true) return false; Sleep(100); menu.isHidden(false); continue; break;
			case 6: // Connect
				menu.isHidden(true); connectMenu(sThread, threadStarted); if (game::server.isConnected() == true) { return false; }
				else menu.isHidden(false); break;
            case 7: // Exit
				return true; break;
            }
        }
        menu.update();
		Sleep(10);
    }
    return false;
}

void gameLoop()
{
	Timer InputCoolDown;
	Player& player = game::pHandler.getLocalPlayer();

	game::SlideUI.isHidden(false);
	game::ServerUI.isHidden(false);
	game::tileUI.isHidden(false);
	game::statsUI.isHidden(false);
	player.getUIRef().isHidden(true);

	clearInput();

	game::m_sounds.StopSound("Menu");
	game::m_sounds.PlaySoundR("Ambient");

	game::pHandler.updateAllPositions();

	while (isExitGame(game::game) == false)
	{
		LOGIC(InputCoolDown, game::game);
		game::game.update();
		game::RegenHandler.update(game::game);
		game::TileHandler.update(game::game);
		player.updateMiningUI();
		updateGameInterface();
		game::SlideUI.update();
		game::ServerUI.update();
		game::statsUI.update();
		game::system.update();
		game::pHandler.update();
		game::GameHandler.Update();
		Sleep(10);
	}
	game::m_sounds.StopSound("Ambient");
}

void preGameLoop()
{
	game::game.loadSettings();

	game::m_sounds.SetVolume((double)game::game.getVolume() / 100);

    Timer InputCoolDown;
    Position newPos(1,1);

	thread sThread;

    Tile core;
    core.setGraphic('C');
    core.setColor(TC_Gray);
    core.setBackground(B_Black);
    core.isWall(false);
    core.isWalkable(false);
    core.isDestructable(true);
    core.isClaimable(false);
    core.setMaxHealth(2500);
    core.setHealth(2500);
    core.forceClaim(game::pHandler.getLocalPlayer().getName());

    Display& game=game::game;
	
    game.setSizeX(75);
    game.setSizeY(30);

    setCursorPos(0,20);
    game.update();
    bool exitFlag=false;
	bool threadStarted = false;

	game::m_sounds.PlaySoundR("Menu");

	game::system.cleanAndUpdateOverlays();

	while (exitFlag == false)
	{
		exitFlag = pauseMenu(game, sThread, threadStarted);

		if (exitFlag == true)
			continue;

		game.reloadAll();

		clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);

		if (game.isLoaded() == false)
			gameNotLoadedMenu(game);

		if (game.isLoaded() == false)
			continue;

		game::tileUI.isHidden(false);

		if (game::server.isConnected() == true && threadStarted == false)
		{
			game::server.isExit(false);
			sThread = thread(bind(&SimpleNetClient::Loop, &game::server));
			threadStarted = true;
		}

		if (game::server.isPaused())
		{
			game::server.Continue();
		}

		Player& player = game::pHandler.getLocalPlayer();

		gameLoop();

		game::m_sounds.PlaySoundR("Menu");

		game::server.Pause();
		clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
		Sleep(250);
	}
	if (threadStarted)
	{
		game::server.isExit(true);
		Sleep(20);
		game::server.Close();
		Sleep(20);
		sThread.join();
	}
	loadScreen(500, "Exiting...");
	return;
}

void initializeMenus()
{
	game::SlideUI.initializeSlideUI();

	game::ServerUI.addSection("Connected:", false, true);
	game::ServerUI.getSectionRef(1).addVar("False");

	UserInterface& TileInfo = game::tileUI;
	TileInfo.addSection("Health:");
	TileInfo.getSectionRef(1).push_backVar("100/100");
	TileInfo.addSection("Tile Owner:");
	TileInfo.getSectionRef(2).push_backVar(" ");
	TileInfo.addSection("Claimed:");
	TileInfo.getSectionRef(3).push_backVar(" ");
	TileInfo.addSection("Ammo:");
	TileInfo.getSectionRef(4).push_backVar((int)0);
	TileInfo.addSection("Player Gold:");
	TileInfo.getSectionRef(5).push_backVar(" ");
	TileInfo.addSection("Mode:");
	TileInfo.getSectionRef(6).push_backVar("Mining");

	game::statsUI.addSection("Player", false, true);
	game::statsUI.addSection("Level:", false, true);
	game::statsUI.addSection("Exp:", false, true);
	game::statsUI.getSectionRef(2).push_backVar(" ");
	game::statsUI.getSectionRef(3).push_backVar(" ");

}

void initializeStdTiles()
{
	using namespace gametiles;

	stone_wall.setGraphic(TG_Stone);
	stone_wall.setColor(TGC_Stone);
	stone_wall.setBackground(TGB_Stone);
	stone_wall.isWalkable(false);
	stone_wall.isDestructable(true);
	stone_wall.setMaxHealth(100);
	stone_wall.setHealth(100);
	stone_wall.isWall(true);

	stone_wall_gold.setGraphic(TG_Gold);
	stone_wall_gold.setColor(TGC_Gold);
	stone_wall_gold.setBackground(TGB_Gold);
	stone_wall_gold.isDestructable(true);
	stone_wall_gold.isWall(true);
	stone_wall_gold.isWalkable(false);
	stone_wall_gold.isClaimable(false);
	stone_wall_gold.setHealth(150);
	stone_wall_gold.setMaxHealth(150);

	stone_floor.setColor(TGC_StoneFloor);
	stone_floor.setGraphic(TG_StoneFloor);
	stone_floor.setBackground(TGB_StoneFloor);
	stone_floor.isDestructable(false);
	stone_floor.isWall(false);
	stone_floor.isWalkable(true);
	stone_floor.isClaimable(true);

	dirt_wall.setColor(TGC_DirtWall);
	dirt_wall.setGraphic(TG_DirtWall);
	dirt_wall.setBackground(TGB_DirtWall);
	dirt_wall.isDestructable(true);
	dirt_wall.isWall(true);
	dirt_wall.isWalkable(false);
	dirt_wall.isClaimable(false);
}

void loadSounds()
{
	game::m_sounds.AddSound("Menu", "Sounds//Loop.wav");
	game::m_sounds.AddSound("Mining", "Sounds//Mining.wav");
	game::m_sounds.AddSound("Bullet", "Sounds//Hit.wav");
	game::m_sounds.AddSound("Place", "Sounds//Tap.wav");
	game::m_sounds.AddSound("Break", "Sounds//Break.wav");
	game::m_sounds.AddSound("Destroy", "Sounds//Destroy.wav");
	game::m_sounds.AddSound("Money", "Sounds//Money.wav");
	game::m_sounds.AddSound("MetalBreak", "Sounds//MetalBreak.wav");
	game::m_sounds.AddSound("Notification", "Sounds//Notification.wav");
	game::m_sounds.AddSound("Ambient", "Sounds//Game.wav");
	game::m_sounds.AddSound("TurretShoot", "Sounds//TurretShoot.wav");
	game::m_sounds.AddSound("NoAmmo", "Sounds//NoAmmo.wav");
	game::m_sounds.AddSound("TurretPlayerHit", "Sounds//TurretPlayerHit.wav");
	game::m_sounds.AddSound("LevelUp", "Sounds//LevelUp.wav");
	game::m_sounds.AddSound("MenuSelection", "Sounds//MenuSelection.wav");
	game::m_sounds.AddSound("Winner", "Sounds//Winner.wav");
	game::m_sounds.AddSound("Loser", "Sounds//Loser.wav");
	game::m_sounds.AddSound("Repair", "Sounds//Repair.wav");
	game::m_sounds.AddSound("Swap", "Sounds//Swap.wav");

	game::m_sounds.SetInfinite("Mining");
	game::m_sounds.SetInfinite("Ambient");
	game::m_sounds.SetInfinite("TurretPlayerHit");
	game::m_sounds.SetInfinite("Menu");
	game::m_sounds.SetInfinite("Repair");
}

int main()
{
    CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 1;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorInfo(h, &cursorInfo);

    srand(time(NULL));

	initializeStdTiles();
	initializeMenus();

	setFullsreen();

	game::m_sounds.Initialize();

	loadSounds();

	preGameLoop();

	game::game.saveSettings();

	setCursorPos(0, 0);

	//release the engine
	m_sound::g_engine->Release();

	//again, for COM
	CoUninitialize();

    return 0;
}