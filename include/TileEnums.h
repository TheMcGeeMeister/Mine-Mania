#pragma once

enum SELECTED
{
	S_Box = COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_LVERTICAL | COMMON_LVB_GRID_RVERTICAL | COMMON_LVB_UNDERSCORE,
	S_Sides = COMMON_LVB_GRID_LVERTICAL | COMMON_LVB_GRID_RVERTICAL,
	S_Left = COMMON_LVB_GRID_LVERTICAL,
	S_Right = COMMON_LVB_GRID_RVERTICAL,
	S_Bottom = COMMON_LVB_UNDERSCORE,
	S_Top = COMMON_LVB_GRID_HORIZONTAL,
};

enum COLORS
{
	C_Black = 0, C_Blue = 1, C_Green = 2, C_Cyan = 3, C_White = 7
};

enum BCOLORS
{
    B_Black=0,
    B_LightGray=BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN,
    B_DarkGray=BACKGROUND_INTENSITY,
	B_Brown=0,
	B_Gold=224,
};

enum TileGraphics
{
    TG_Grass=176, TG_Stone=176, TG_Gold=176, TG_StoneFloor=' ',
	TG_DirtWall=' '
};

enum TileGraphicColor
{
    TGC_Grass=38,
    TGC_Stone=0,
    TGC_Gold=6,
    TGC_StoneFloor=0,
	TGC_DirtWall=0
};

enum TileGraphicBackground
{
    TGB_Grass=B_Black,
    TGB_Stone=B_DarkGray,
    TGB_Gold=B_DarkGray,
    TGB_StoneFloor=B_DarkGray,
	TGB_DirtWall=B_Brown
};

enum TileColor
{
    TC_Gray=7
};

/*
0 - Black
1 - Blue
2 - Green
3 - Cyan
4 - Red
5 - Magenta
6 - Brown
7 - Light Grey
8 - Dark Grey
9 - Light Blue
A - Light Green
B - Light Cyan
C - Light Red
D - Light Magenta
E - Yellow
F - White
*/
