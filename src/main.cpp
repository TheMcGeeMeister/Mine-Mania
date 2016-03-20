#include <iostream>
#include <Display.h>
#include <random>
#include <time.h>
#include <Timer.h>
#include <sstream>
#include <TileChangeManager.h>
#include <RegenManager.h>
#include <TileEnums.h>
#include <Underlord.h>
#include <Tile.h>
#include <thread>
#include <UserInterface.h>
#include <PositionVariables.h>
#include <fstream>
#include <conio.h>
#include <SimpleNetClient.h>

#define DEFAULT_CLEAR_WIDTH 100
#define DEFAULT_CLEAR_HEIGHT 36

using namespace std;

bool pauseGameMenu(Display& game);

namespace game
{
	System system;
    TileChangeManager TileHandler;
    RegenManager RegenHandler;
    Underlord player;
	Underlord enemy;
	Display game;
	UserInterface tileUI(30, 10, 0, 30);
	UserInterface SlideUI(0, 10, 75, 0);
	UserInterface ServerUI(0, 10, 75, 28);
	SimpleNetClient server;
	bool threadExit;
	int curFont;
	void Log(string txt)
	{
		fstream file("Logs\\Log.txt", ios::app);
		file << txt << endl;
	}
}

namespace gametiles
{
	Tile stone_wall;
	Tile stone_wall_gold;
	Tile stone_floor;
	Tile dirt_wall;
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
	if (GetAsyncKeyState(VK_ESCAPE))
		return pauseGameMenu(game);
	else
		return false;
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

void removeScrollBar()
{
	HANDLE hOut;
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	COORD NewSBSize;
	int Status;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(hOut, &SBInfo);
	NewSBSize.X = SBInfo.dwSize.X - 2;
	NewSBSize.Y = SBInfo.dwSize.Y;

	Status = SetConsoleScreenBufferSize(hOut, NewSBSize);

	string txt;
	txt = Status;
	txt += " :Status";
	game::SlideUI.addSlide(txt);
}

void initializeWindowProperties()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD flags = CONSOLE_FULLSCREEN_MODE;
	COORD pos;
	SetConsoleDisplayMode(h, flags, &pos);
}

void LOGIC(Timer& InputCoolDown, Display& game, Tile& core)
{
    if(InputCoolDown.Update()==true)
    {
		/* Movement */
		////////////////////////////////////////////////
        /*if(GetAsyncKeyState('W'))
        {
			game::player.moveHandUp(game);
            InputCoolDown.StartNewTimer(0.075);
        }else if(GetAsyncKeyState('S'))
        {
			game::player.moveHandDown(game);
            InputCoolDown.StartNewTimer(0.075);
        }else if(GetAsyncKeyState('A'))
        {
			game::player.moveHandLeft(game);
            InputCoolDown.StartNewTimer(0.075);
        }else if(GetAsyncKeyState('D'))
        {
			game::player.moveHandRight(game);
            InputCoolDown.StartNewTimer(0.075);
        }*/
		for (int x = 0; x < 3; x++)
		{
			if (kbhit())
			{
				int key = getch();
				if (key == 224)
					key = getch();
				switch (key)
				{
				case 'w':game::player.moveHandUp(game); InputCoolDown.StartNewTimer(0.075); break;
				case 's':game::player.moveHandDown(game); InputCoolDown.StartNewTimer(0.075); break;
				case 'a':game::player.moveHandLeft(game); InputCoolDown.StartNewTimer(0.075); break;
				case 'd':game::player.moveHandRight(game); InputCoolDown.StartNewTimer(0.075); break;
				case 'j':game::SlideUI.addSlide("Testing"); InputCoolDown.StartNewTimer(0.075); break;
				case 't':swapConsoleFullScreen(); removeScrollBar(); InputCoolDown.StartNewTimer(0.075); break;
				case 'm':game::server.SendLiteral("12\n9\nTesting"); InputCoolDown.StartNewTimer(0.075); break;
				case 72:game::player.mineUp(game); InputCoolDown.StartNewTimer(0.075); break;
				case 80:game::player.mineDown(game); InputCoolDown.StartNewTimer(0.075); break;
				case 75:game::player.mineLeft(game); InputCoolDown.StartNewTimer(0.075); break;
				case 77:game::player.mineRight(game); InputCoolDown.StartNewTimer(0.075); break;
				default: break;
				}
			}
			else
				x = 3;
		}
		////////////////////////////////////////////////
		/* Mining */
		////////////////////////////////////////////////
		/*if (GetAsyncKeyState(VK_UP))
		{
			game::player.mineUp(game);
			InputCoolDown.StartNewTimer(0.075);
		}else if (GetAsyncKeyState(VK_DOWN))
		{
			game::player.mineDown(game);
			InputCoolDown.StartNewTimer(0.075);
		}else if (GetAsyncKeyState(VK_LEFT))
		{
			game::player.mineLeft(game);
			InputCoolDown.StartNewTimer(0.075);
		}else if (GetAsyncKeyState(VK_RIGHT))
		{
			game::player.mineRight(game);
			InputCoolDown.StartNewTimer(0.075);
		}*/
		////////////////////////////////////////////////
		Position pos = game::player.getHandPosition();
        if(GetAsyncKeyState('F'))
        {
			game.getTileRefAt(pos).isFortified(true);
            InputCoolDown.StartNewTimer(0.075);
        }
        if(GetAsyncKeyState(VK_SPACE))
        {
			if (game.isWalkableTileNear(pos))
			{
				game.getTileRefAt(pos).mine(10, game::player);
				InputCoolDown.StartNewTimer(0.075);
			}
        }
        if(GetAsyncKeyState('C'))
        {
            core.setPos(pos);
            game.setTileAt(pos, core);
            InputCoolDown.StartNewTimer(0.075);
        }
        if(GetAsyncKeyState('V'))
        {
			game::player.claimOnHand();
			InputCoolDown.StartNewTimer(0.075);
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

void updateTileInfo()
{
	Display& game = game::game;
	UserInterface& TileInfo = game::tileUI;
	std::stringstream health;
	Tile& tile = game.getTileRefAt(game::player.getHandPosition());
	health << tile.getHealth() << "/" << tile.getMaxHealth();
	TileInfo.getSectionRef(1).setVar(1, health.str());

	TileInfo.getSectionRef(2).setVar(1, tile.getClaimedBy());

	stringstream claimed;
	claimed << tile.getClaimedPercentage() << "%";
	TileInfo.getSectionRef(3).setVar(1, claimed.str());

	if (tile.hasGold())
	{
		TileInfo.getSectionRef(4).isHidden(false);
		TileInfo.getSectionRef(4).setVar(1, tile.getGold());
	}
	else
	{
		TileInfo.getSectionRef(4).isHidden(true);
	}

	std::stringstream gold;
	gold << game::player.getGoldAmount();
    TileInfo.getSectionRef(5).setVar(1, gold.str());

    TileInfo.update();
}

void connectMenu(thread& sThread, bool& threadStarted)
{
	Sleep(250);
	UserInterface ui(0, 0, 0, 0);
	ui.push_isection("Address:");
	ui.push_back("Continue" , true, true);
	ui.push_back("Return", true, true);
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
		stringstream msg;
		msg << GetWorld << End;
		game::server.SendLiteral(msg.str());
		while (game::game.isLoaded() == false)
		{
			loadScreen(100, "Waiting...");
			Sleep(10);
		}
	}
}

void loadMenu(Display& game)
{
	clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
	Sleep(500);
	UserInterface menu(0, 0, 0, 0);
	int worldAmount = game.getSaveAmount();
	bool exitFlag = false;
	if (worldAmount == 0)
	{
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
			}
			else
			{
				filename << "World" << i << ".dat";
			}
			menu.addSection(filename.str(), true, true);
		}
		menu.addSection("Exit", true, true);
	}
	while (exitFlag == false)
	{
		menu.update();
		if (menu.isSectionActivated())
		{
			if (menu.getActivatedSection() == worldAmount + 1)
			{
				clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
				exitFlag = true;
				continue;
			}
			stringstream filename;
			if (menu.getActivatedSection()<10)
			{
				clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
				filename << "Saves\\" << "World" << "0" << menu.getActivatedSection() << ".dat";
				game.loadWorld(filename.str());
				exitFlag = true;
			}
			else
			{
				clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
				filename << "Saves\\" << "World" << menu.getActivatedSection() << ".dat";
				game.loadWorld(filename.str());
				exitFlag = true;
 			}
		}
	}
	menu.isHidden(true);
	Sleep(250);
	return;
}

void saveMenu(Display& game)
{
	Sleep(500);
	UserInterface menu(0, 0, 0, 0);
	int worldAmount = game.getSaveAmount();
	bool exitFlag = false;
	if (worldAmount == 0)
	{
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
	}
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
	}
	menu.isHidden(true);
	Sleep(250);
	return;
}

void settingsMenu()
{
	Sleep(250);
	UserInterface menu(0, 0, 0, 0);
	menu.push_isection("Font Size:");
	menu.addSection("Font:", true, false);
	menu.getSectionRef(2).addVar("");
	menu.addSection("Fullscreen:", true, false);
	menu.getSectionRef(3).addVar("");
	menu.addSection("Exit", true, false);

	bool exitFlag = false;
	
	bool isFullScreen = game::game.isFullScreen();
	int font = game::game.getFont();
	int fontSize = game::game.getFontSize();
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
				exitFlag = true; break;
			default:
				break;
			}
		}
	}
	menu.isHidden(true);
	game::game.setFont(font);
	game::game.setFontSize(fontSize);
	Sleep(250);
}

void newWorldMenu(Display& game)
{
	Sleep(250);
	UserInterface NewWorld(0, 0, 0, 0);
	NewWorld.push_isection("Name:");
	NewWorld.push_back("Continue", true, true);
	NewWorld.addSection("Exit", true, true);
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
				game::player.setName(NewWorld.getSectionRef(1).getIVar());
				load.join();
				exitFlag = true; break;
			}
			case 3: exitFlag = true; break;
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
	UserInterface menu(0, 0, 0, 0);
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
			case 2: menu.isHidden(true); newWorldMenu(game); clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT); Sleep(250); return; break;
			case 3: loadMenu(game); clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT); Sleep(250); return; break;
			case 4: clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT); Sleep(250); return; break;
			}
		}
	}
}

bool pauseGameMenu(Display& game)
{
	game.isHidden(true);
	game::tileUI.isHidden(true);
	UserInterface ui(0, 0, 0, 0);
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
			case 1: game.reloadAll(); FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); game::tileUI.isHidden(false); return false;
			case 2: {thread load(loadScreen, 500, "Saving..."); game.saveWorld(); ui.reDrawAll(); load.join(); break; }
			case 3: game.reloadAll(); FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); return true;
			}
		}
	}
	game.isHidden(false);
	FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	return true;
}

bool pauseMenu(Display &game, thread& sThread, bool& threadStarted)
{
    clearScreenPart(DEFAULT_CLEAR_WIDTH, DEFAULT_CLEAR_HEIGHT);
    UserInterface menu;
	PositionVariables pVar(0, 0, 0, 0);
	menu.setPositionVariables(pVar);
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
				loadMenu(game);
				menu.isHidden(false);
				menu.reDrawAll();
				if (game.isLoaded())
					return false;
				break;
			case 4: // Settings
				menu.isHidden(true);
				settingsMenu();
				menu.isHidden(false);
				break;
            case 5: // New
				menu.isHidden(true); newWorldMenu(game); exitFlag = true; continue; break;
			case 6: // Connect
				menu.isHidden(true); connectMenu(sThread, threadStarted); exitFlag = true; break;
            case 7: // Exit
				return true; break;
            }
        }
        menu.update();
    }
    return false;
}

namespace AI
{
	void log(string text);
	Position getRandomValidPosition(Position pos)
	{
		Position curPos = pos;
		vector<Position> posList;
		curPos.up();
		if (game::game.isValidPosition(curPos, true))
		{
			posList.push_back(curPos);
		}
		curPos = pos;
		curPos.down();
		if (game::game.isValidPosition(curPos, true))
		{
			posList.push_back(curPos);
		}
		curPos = pos;
		curPos.left();
		if (game::game.isValidPosition(curPos, true))
		{
			posList.push_back(curPos);
		}
		curPos = pos;
		curPos.right();
		if (game::game.isValidPosition(curPos, true))
		{
			posList.push_back(curPos);
		}
		if (posList.size() > 0)
		{
			int num = rand() % posList.size();
			return posList[num];
		}
		else
		{
			return Position(1, 1);
		}
	}
	void log(string text)
	{
		fstream file("Logs\\log.txt", ios::app);
		file << text << endl;
		file.close();
	}
	void testAI(Underlord enemy)
	{
		Timer coolDown;
		int action = 1;
		int mineDirect = 1;
		Position minePos;
		/*
		1 - Finding
		2 - Mining
		3 - Claiming

		1 - up
		2 - down
		3 - left
		4 - rigth
		*/
		while (game::threadExit == false)
		{
			if (coolDown.Update() == true)
			{
				if (action == 1)
				{
					/* Find Mining */
					///////////////////////////////
					{
						Position newPos = enemy.getHandPosition();
						newPos.up();
						if (game::game.isValidPosition(newPos))
						{
							if (game::game.getTileRefAt(newPos).isWall() == true)
							{
								action = 2;
								mineDirect = 1;
								minePos = newPos;
								continue;
							}
						}
						newPos = enemy.getHandPosition();
						newPos.down();
						if (game::game.isValidPosition(newPos))
						{
							if (game::game.getTileRefAt(newPos).isWall() == true)
							{
								action = 2;
								mineDirect = 2;
								minePos = newPos;
								continue;
							}
						}
						newPos = enemy.getHandPosition();
						newPos.left();
						if (game::game.isValidPosition(newPos))
						{
							if (game::game.getTileRefAt(newPos).isWall() == true)
							{
								action = 2;
								mineDirect = 3;
								minePos = newPos;
								continue;
							}
						}
						newPos = enemy.getHandPosition();
						newPos.right();
						if (game::game.isValidPosition(newPos))
						{
							if (game::game.getTileRefAt(newPos).isWall() == true)
							{
								action = 2;
								mineDirect = 4;
								minePos = newPos;
								continue;
							}
						}
					}
					///////////////////////////////
					/* Move */
					///////////////////////////////
					{
						if (game::game.isWalkableTileNear(enemy.getHandPosition()))
						{
							Position newPos = getRandomValidPosition(enemy.getHandPosition());
							enemy.forceHandPosition(newPos, game::game);
							coolDown.StartNewTimer(0.100);
						}
					}
					///////////////////////////////
				}
				if (action == 2)
				{
					switch (mineDirect)
					{
					case 1: enemy.mineUp(game::game); break;
					case 2: enemy.mineDown(game::game); break;
					case 3: enemy.mineLeft(game::game); break;
					case 4: enemy.mineRight(game::game); break;
					}
					coolDown.StartNewTimer(0.100);
				}
				if (game::game.getTileRefAt(minePos).isWall() == false)
				{
					action = 1;
				}
			}
			Sleep(10);
		}
	}
}

void gameLoop()
{
	game::game.loadSettings();

    Timer InputCoolDown;
    Position newPos(1,1);
	game::SlideUI.initializeSlideUI();

	game::ServerUI.addSection("Connected:", false, true);
	game::ServerUI.getSectionRef(1).addVar("False");

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
    core.forceClaim(game::player.getName());

    Display& game=game::game;

	UserInterface& TileInfo = game::tileUI;
    TileInfo.addSection("Health:");
    TileInfo.getSectionRef(1).push_backVar("100/100");
    TileInfo.addSection("Owner:");
    TileInfo.getSectionRef(2).push_backVar(" ");
    TileInfo.addSection("Claimed:");
    TileInfo.getSectionRef(3).push_backVar(" ");
    TileInfo.addSection("Gold:");
    TileInfo.getSectionRef(4).push_backVar((int)0);
    TileInfo.addSection("Player Gold:");
    TileInfo.getSectionRef(5).push_backVar("0");

    game.setSizeX(75);
    game.setSizeY(30);

    setCursorPos(0,20);
    game.update();
    bool exitFlag=false;
	bool threadStarted = false;
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
		FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
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
		while (isExitGame(game) == false)
		{
			LOGIC(InputCoolDown, game, core);
			game.update();
			game::RegenHandler.update(game);
			game::TileHandler.update(game);
			game::player.updateMiningUI();
			updateTileInfo();
			game::SlideUI.update();
			game::ServerUI.update();
			if (game::server.isConnected() == true)
			{
				if (game::game.isPacketsAvailable())
				{
					//list<Packet> packets = game::game.getPackets();
					for (auto& iter : game::game.getPackets())
					{
						stringstream data;
						data << iter.name << endl;
						data << iter.data;
						AI::log("Sending:" + data.str());
						game::server.SendLiteral(data.str());
					}
					game::game.clearPackets();
				}
			}
			Sleep(10);
		}
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

void initializeStdTiles()
{
	using namespace gametiles;

	stone_wall.setGraphic(TG_Stone);
	stone_wall.setColor(TGC_Stone);
	stone_wall.setBackground(TGB_Stone);
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

int main()
{
    CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 1;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorInfo(h, &cursorInfo);

    srand(time(NULL));

	initializeStdTiles();

	setFullsreen();

    gameLoop();

	game::game.saveSettings();

	setCursorPos(0, 0);
	/*while (isExit() == false)
	{
		if (kbhit())
		{
			getch();
			stringstream msg;
			msg << "SendHost\n";
			msg << "MSG\n" ;
			msg << "Testing\n";
			msg << "Ending\n";
			string key;
			string remainder;
			string newKey;
			msg >> key;
			cout << "Key:" << key << endl;
			cout << "Remainder:" << msg.rdbuf() << endl;

 		}
	}*/ // For Testing Char Keys

    return 0;
}
