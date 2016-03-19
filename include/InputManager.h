#pragma once
#include <list>


class InputManager
{
public:
	InputManager();
	~InputManager();

	bool inputAvailable();

	void isExit(bool isExit);

	char getInput();

	void start();
	void clear();

private:
	bool isInputAvailable_;

	std::list<char> input_;

	bool isExit_;
};

