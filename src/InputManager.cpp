#include "..\include\InputManager.h"
#include <time.h>
#include <conio.h>
#include <random>
#include <windows.h>


InputManager::InputManager()
{
	isExit_ = false;
	isInputAvailable_ = false;
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
			case 'a':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'b':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'c':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'd':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'e':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'f':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'g':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'h':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'i':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'j':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'k':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'l':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'm':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'n':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'o':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'p':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'q':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'r':input_.push_back(key); isInputAvailable_ = true;  break;
			case 's':input_.push_back(key); isInputAvailable_ = true;  break;
			case 't':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'u':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'v':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'w':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'x':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'y':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'z':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'A':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'B':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'C':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'D':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'E':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'F':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'G':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'H':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'I':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'J':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'K':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'L':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'M':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'N':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'O':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'P':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'Q':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'R':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'S':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'T':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'U':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'V':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'W':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'X':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'Y':input_.push_back(key); isInputAvailable_ = true;  break;
			case 'Z':input_.push_back(key); isInputAvailable_ = true;  break;
			case ',':input_.push_back(key); isInputAvailable_ = true;  break;
			case '-':input_.push_back(key); isInputAvailable_ = true;  break;
			case ' ':input_.push_back(key); isInputAvailable_ = true;  break;
			case '.':input_.push_back(key); isInputAvailable_ = true;  break;
			case '1':input_.push_back(key); isInputAvailable_ = true;  break;
			case '2':input_.push_back(key); isInputAvailable_ = true;  break;
			case '3':input_.push_back(key); isInputAvailable_ = true;  break;
			case '4':input_.push_back(key); isInputAvailable_ = true;  break;
			case '5':input_.push_back(key); isInputAvailable_ = true;  break;
			case '6':input_.push_back(key); isInputAvailable_ = true;  break;
			case '7':input_.push_back(key); isInputAvailable_ = true;  break;
			case '8':input_.push_back(key); isInputAvailable_ = true;  break;
			case '9':input_.push_back(key); isInputAvailable_ = true;  break;
			case '0':input_.push_back(key); isInputAvailable_ = true;  break;
			default: break;
			}
		}
	//}
}

void InputManager::clear()
{
	input_.clear();
}

void InputManager::isExit(bool isExit)
{
	isExit_ = isExit;
}
