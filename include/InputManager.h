#pragma once
#include <list>


class InputManager
{
public:
	InputManager();
	~InputManager();

	bool inputAvailable();

	void isExit(bool isExit);
	bool isReturnPressed();

	char getInput();

	void start();
	void update();
	void clear();

private:
	bool isInputAvailable_;
	bool isReturnPressed_;

	std::list<char> input_;

	bool isExit_;
};

