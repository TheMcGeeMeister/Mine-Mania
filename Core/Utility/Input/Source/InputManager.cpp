#include <time.h>
#include <conio.h>
#include <random>
#include <windows.h>
#include "InputManager.h"

InputManager::InputManager()
{
	isExit_ = false;
	isInputAvailable_ = false;
	isReturnPressed_ = false;
}


InputManager::~InputManager()
{
	isExit_ = true;
	input_.clear();
}

bool InputManager::inputAvailable()
{
	return	isInputAvailable_;
}

char InputManager::getInput()
{
	char key;
	key = input_.front();
	input_.pop_front();
	if (input_.empty() == true)
	{
		isInputAvailable_ = false;
	}
	return key;
}

void InputManager::start()
{
	//while (isExit_ == false)
	//{
		while (kbhit())
		{
			char key = getch();

			switch (key)
			{
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case ',':
			case '-':
			case ' ':
			case '.':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':input_.push_back(key); isInputAvailable_ = true;  break;
			case 13: isReturnPressed_ = true; break;
			default: break;
			}
		}
	//}
}

void InputManager::update()
{
	if (GetActiveWindow() != GetFocus()) return;
	while (kbhit())
	{
		char key = getch();

		switch (key)
		{
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case ',':
		case '-':
		case ' ':
		case '.':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':input_.push_back(key); isInputAvailable_ = true;  break;
		case 13: isReturnPressed_ = true; break;
		default: break;
		}
	}
}

void InputManager::clear()
{
	input_.clear();
	while (_kbhit())
	{
		_getch();
	}
}

void InputManager::isExit(bool isExit)
{
	isExit_ = isExit;
}

bool InputManager::isReturnPressed()
{
	if (isReturnPressed_ == true)
	{
		isReturnPressed_ = false;
		return true;
	}
	else
	{
		return false;
	}
}
