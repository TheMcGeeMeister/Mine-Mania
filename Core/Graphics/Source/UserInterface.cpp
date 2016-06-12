#include <Windows.h>
#include <iostream>
#include <sstream>
#include <functional>
#include <fstream>
#include "BorderEnums.h"
#include "UserInterface.h"
#include "SoundManager.h"
#include "TileEnums.h"
#include "Common.h"

using namespace std;

extern void clearInput();

namespace game
{
	extern void Log(std::string txt);
	extern SoundManager m_sounds;
}


UserInterface::UserInterface()
{
	isHidden_ = false;
	isPosVariablesSet_ = false;
	isSelectionAvailable_ = false;
	isSelectionActivated_ = false;
	isInIMode_ = false;
	isSlideUI_ = false;
	curSelected_ = 0;
	sectionAmount_ = 0;
}

UserInterface::UserInterface(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y)
{
	isHidden_ = false;
	isPosVariablesSet_ = true;
	isSelectionAvailable_ = false;
	isSelectionActivated_ = false;
	isInIMode_ = false;
	isSlideUI_ = false;
	curSelected_ = 0;
	sectionAmount_ = 0;
	positionVars_.size_x = size_x;
	positionVars_.size_y = size_y;
	positionVars_.offset_x = offset_x;
	positionVars_.offset_y = offset_y;
	positionVars_.border_width = 0;
}

UserInterface::UserInterface(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y, unsigned int border_width)
{
	isHidden_ = false;
	isPosVariablesSet_ = true;
	isSelectionAvailable_ = false;
	isSelectionActivated_ = false;
	isInIMode_ = false;
	isSlideUI_ = false;
	curSelected_ = 0;
	sectionAmount_ = 0;
	positionVars_.size_x = size_x;
	positionVars_.size_y = size_y;
	positionVars_.offset_x = offset_x;
	positionVars_.offset_y = offset_y;
	positionVars_.border_width = border_width;
}


UserInterface::~UserInterface()
{
	iManager.isExit(true);
	isHidden(true);
}

void UserInterface::isHidden(bool isHidden)
{
	isHidden_ = isHidden;
	if (isHidden == false)
	{
		reDrawAll();
		if (positionVars_.border_width > 0)
		{
			drawBorder();
		}
	}
	else
	{
		hide();
	}
}

bool UserInterface::isHidden()
{
	return isHidden_;
}

void UserInterface::setSizeX(unsigned int size_x)
{
	positionVars_.size_x = size_x;
}

void UserInterface::setSizeY(unsigned int size_y)
{
	positionVars_.size_y = size_y;
}

void UserInterface::setOffsetX(unsigned int offset_x)
{
	positionVars_.offset_x = offset_x;
}

void UserInterface::setOffsetY(unsigned int offset_y)
{
	positionVars_.offset_y = offset_y;
}

void UserInterface::setBorderWidth(unsigned int border_width)
{
	positionVars_.border_width = border_width;
}

void UserInterface::setAll(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y, unsigned int border_width)
{
	isPosVariablesSet_ = true;
	positionVars_.size_x = size_x;
	positionVars_.size_y = size_y;
	positionVars_.offset_x = offset_x;
	positionVars_.offset_y = offset_y;
	positionVars_.border_width = border_width;
}

void UserInterface::addSection(Section newSection)
{
	sectionAmount_++;

	newSection.setIndex(sectionAmount_);

	sections_[sectionAmount_] = newSection;

	redraws_.push_back(sectionAmount_);

	if (newSection.isSelectable())
	{
		if (isSelectionAvailable_ == false)
		{
			isSelectionAvailable_ = true;
			curSelected_ = sectionAmount_;
		}
	}
}

void UserInterface::addSection(string text)
{
	sectionAmount_++;
	
	Section newSection(sectionAmount_, text, false, true);

	sections_[sectionAmount_] = newSection;

	redraws_.push_back(sectionAmount_);
}

void UserInterface::addSection(string text, bool isSelectable)
{
	sectionAmount_++;

	Section newSection(sectionAmount_, text, isSelectable, false);

	sections_[sectionAmount_] = newSection;

	redraws_.push_back(sectionAmount_);

	if (newSection.isSelectable())
	{
		if (isSelectionAvailable_ == false)
		{
			isSelectionAvailable_ = true;
			curSelected_ = sectionAmount_;
		}
	}
}

void UserInterface::addSection(string text, bool isSelectable, bool isNumberHidden)
{
	sectionAmount_++;

	Section newSection(sectionAmount_, text, isSelectable, isNumberHidden);

	sections_[sectionAmount_] = newSection;

	redraws_.push_back(sectionAmount_);

	if (newSection.isSelectable())
	{
		if (isSelectionAvailable_ == false)
		{
			isSelectionAvailable_ = true;
			curSelected_ = sectionAmount_;
		}
	}
}

void UserInterface::addSlide(string text)
{
	slideSectionAdd_.push_back(text);
}

void UserInterface::push_back(Section newSection)
{
	sectionAmount_++;

	newSection.setIndex(sectionAmount_);

	sections_[sectionAmount_] = newSection;

	redraws_.push_back(sectionAmount_);

	if (newSection.isSelectable())
	{
		if (isSelectionAvailable_ == false)
		{
			isSelectionAvailable_ = true;
			curSelected_ = sectionAmount_;
		}
	}
}

void UserInterface::push_back(string text)
{
	sectionAmount_++;

	Section newSection(sectionAmount_, text, false, true);

	sections_[sectionAmount_] = newSection;

	redraws_.push_back(sectionAmount_);
}

void UserInterface::push_back(string text, bool isSelectable)
{
	sectionAmount_++;

	Section newSection(sectionAmount_, text, isSelectable, false);

	sections_[sectionAmount_] = newSection;

	redraws_.push_back(sectionAmount_);

	if (newSection.isSelectable())
	{
		if (isSelectionAvailable_ == false)
		{
			isSelectionAvailable_ = true;
			curSelected_ = sectionAmount_;
		}
	}
}

void UserInterface::push_back(string text, bool isSelectable, bool isNumberHidden)
{
	sectionAmount_++;

	Section newSection(sectionAmount_, text, isSelectable, isNumberHidden);

	sections_[sectionAmount_] = newSection;

	redraws_.push_back(sectionAmount_);

	if (newSection.isSelectable())
	{
		if (isSelectionAvailable_ == false)
		{
			isSelectionAvailable_ = true;
			curSelected_ = sectionAmount_;
		}
	}
}

void UserInterface::push_isection(string text)
{
	sectionAmount_++;
	Section iSection;
	iSection.isISection(true);
	iSection.setText(text);
	iSection.setIndex(sectionAmount_);
	iSection.isSelectable(true);
	sections_[sectionAmount_] = iSection;
	if (isSelectionAvailable_ == false)
	{
		isSelectionAvailable_ = true;
		curSelected_ = sectionAmount_;
	}

	redraws_.push_back(sectionAmount_);
}

void UserInterface::push_isection(string text, int maxIText)
{
	sectionAmount_++;
	Section iSection;
	iSection.isISection(true);
	iSection.setIText(text);
	iSection.setMaxIText(maxIText);
	iSection.setIndex(sectionAmount_);
	iSection.isSelectable(true);
	sections_[sectionAmount_] = iSection;
	if (isSelectionAvailable_ == false)
	{
		isSelectionAvailable_ = true;
		curSelected_ = sectionAmount_;
	}

	redraws_.push_back(sectionAmount_);
}

void UserInterface::setPositionVariables(PositionVariables posV)
{
	positionVars_ = posV;

	isPosVariablesSet_ = true;
}

Section& UserInterface::getSectionRef(uint16_t sectionIndex)
{
	if (sections_.count(sectionIndex))
	{
		return sections_[sectionIndex];
	}
}

Section UserInterface::getSection(uint16_t sectionIndex)
{
	if (sections_.count(sectionIndex))
	{
		return sections_[sectionIndex];
	}
}

bool UserInterface::getSectionRefT(uint16_t sectionIndex, Section ** section)
{
	if (sections_.count(sectionIndex))
	{
		*section = &sections_[sectionIndex];
		return true;
	}
	*section = nullptr;
	return false;
}

bool UserInterface::isSectionActivated()
{
	if (isSelectionActivated_ == true)
	{
		isSelectionActivated_ = false;
		return true;
	}
	return false;
}

int UserInterface::getActivatedSection()
{
	return (int)curSelected_;
}

void UserInterface::initializeSlideUI()
{
	isSlideUI_ = true;
}

void UserInterface::moveSelectionUp()
{
	redraws_.push_back(curSelected_);
	if (isSelectionAvailable_ == false)
		return;

	curSelected_--;
	if (curSelected_ == 0)
		curSelected_ = sectionAmount_;

	while (sections_[curSelected_].isSelectable() != true && sections_[curSelected_].isHidden() == false)
	{
		curSelected_--;
		if (curSelected_ == 0)
			curSelected_ = sectionAmount_;
	}
	redraws_.push_back(curSelected_);
}

void UserInterface::moveSelectionDown()
{
	redraws_.push_back(curSelected_);
	if (isSelectionAvailable_ == false)
		return;

	curSelected_++;
	if (curSelected_ > sectionAmount_)
		curSelected_ = 1;

	while (sections_[curSelected_].isSelectable() != true && sections_[curSelected_].isHidden() == false)
	{
		curSelected_++;
		if (curSelected_ > sectionAmount_)
			curSelected_ = 1;
	}
	redraws_.push_back(curSelected_);
}

void UserInterface::moveSlideDown()
{
	auto iter = slideSections_.rbegin();
	auto end = slideSections_.rend();
	bool deleteLast = false;
	uint16_t theDelete;
	for (; iter != end; iter++)
	{
		if (iter->first == positionVars_.size_y - positionVars_.border_width * 2 - 1)
		{
			deleteLast = true;
			theDelete = iter->first;
		}
		else
		{
			slideSections_[iter->first + 1] = iter->second;
			slideSections_[iter->first] = "";
		}
	}
	if (deleteLast == true)
	{
		slideSections_.erase(theDelete);
	}
}

void UserInterface::update()
{
	if (isPosVariablesSet_ == false)
		return;
	if (isHidden_ == false)
	{
		if (isSlideUI_ == false)
		{
			for (auto& iter : sections_)
			{
				if (iter.second.redraw() == true)
					redraws_.push_back(iter.second.getIndex());
			}
			if (redraws_.empty() == false)
			{
				for (auto& iter : redraws_)
				{
					draw(iter);
				}
				redraws_.clear();
			}

			if (isSelectionAvailable_)
			{
				if (inputCoolDown.Update() == true)
				{
					if (isInIMode_ == false)
					{
						input.update();
						if (input.inputAvailable())
						{
							char i = input.getInput();
							if (i == 'w')
							{
								moveSelectionUp();
								game::m_sounds.PlaySoundR("MenuSelection");
								inputCoolDown.StartNewTimer(0.100);
								return;
							}
							else if (i == 's')
							{
								moveSelectionDown();
								game::m_sounds.PlaySoundR("MenuSelection");
								inputCoolDown.StartNewTimer(0.100);
								return;
							}
						}
						if (input.isReturnPressed())
						{
							if (sections_[curSelected_].isISection() == true)
							{
								isInIMode_ = true;
								iManager.clear();
								iManager.isExit(false);
								FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
								clearInput();
								redraws_.push_back(curSelected_);
							}
							else
							{
								isSelectionActivated_ = true;
								game::m_sounds.PlaySoundR("MenuSelection");
							}
							inputCoolDown.StartNewTimer(0.175);
							return;
						}
					}
					else
					{
						if (iManager.isReturnPressed())
						{
							isInIMode_ = false;
							iManager.clear();
							iManager.isExit(true);
							draw(curSelected_);
							isSelectionActivated_ = true;
							Sleep(250);
							return;
						}
						if (GetAsyncKeyState(VK_ESCAPE))
						{
							isInIMode_ = false;
							iManager.clear();
							iManager.isExit(true);
							draw(curSelected_);
							isSelectionActivated_ = true;
							Sleep(250);
							return;
						}
						iManager.update();
						while (iManager.inputAvailable())
						{
							char key = iManager.getInput();
							string skey;
							skey.push_back(key);
							sections_[curSelected_].addIText(skey);
						}
						if (GetAsyncKeyState(VK_BACK))
						{
							sections_[curSelected_].removeIText();
							inputCoolDown.StartNewTimer(0.100);
						}
					}
				}
			}
		}
		else
		{
			if (inputCoolDown.Update() == true)
			{
				if (slideSections_.empty() == false)
				{
					moveSlideDown();
					inputCoolDown.StartNewTimer(0.100);
				}
				if (slideSectionAdd_.empty() == false)
				{
					slideSections_[0] = slideSectionAdd_.front();
					slideSectionAdd_.pop_front();
				}
				for (auto& iter : slideSections_)
				{
					slideDraw(iter.first, iter.second);
				}
			}
		}
	}
}

void UserInterface::reset()
{
	sections_.clear();
	redraws_.clear();
	slideSectionAdd_.clear();
	slideSections_.clear();
	curSelected_ = 0;
	isSelectionActivated_ = false;
	isSelectionAvailable_ = false;
	isHidden_ = false;
	isInIMode_ = false;
	isSlideUI_ = false;
	inputCoolDown.StartNewTimer(0.0001);
	sectionAmount_ = 0;
	positionVars_.size_x = 0;
	positionVars_.size_y = 0;
	positionVars_.offset_x = 0;
	positionVars_.offset_y = 0;
}

void UserInterface::resetDisplay()
{
	hide(false);
	sections_.clear();
	redraws_.clear();
	slideSectionAdd_.clear();
	slideSections_.clear();
	curSelected_ = 0;
	isSelectionActivated_ = false;
	isSelectionAvailable_ = false;
	isInIMode_ = false;
	sectionAmount_ = 0;
}

void UserInterface::draw(uint16_t index)
{
	if (sections_.count(index))
	{
		unsigned int offset_y = positionVars_.offset_y + (positionVars_.border_width); 
		offset_y += index - 1;
		SetCursorPosition(positionVars_.offset_x+positionVars_.border_width, offset_y);
		stringstream text;
		if (curSelected_ == index)
		{
			if (sections_[index].isISection() == true)
			{
				if (isInIMode_ == true)
				{
					text << sections_[index]() + "_";
				}
				else
				{
					text << sections_[index]() + " <-";
				}
			}
			else
			{
				text << sections_[index]() + " <-";
			}
		}
		else
		{
			text << sections_[index]();
		}
		int length = text.str().length();
		int spaces = 0;
		if (length <= positionVars_.size_x)
		{
			if (positionVars_.border_width > 0)
				spaces = positionVars_.size_x - length;
			else
				spaces = positionVars_.size_x - length;
		}
		else
		{
			text.str(text.str().substr(0, positionVars_.size_x));
		}
		if(spaces > 0)
			text << string(spaces, ' ');
		//cout << text.str();
		/* Add Color Output 
		if (sections[index].isColored())
		{
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			int x_ = positionVars_.offset_x + positionVars_.border_width;
			DWORD output;
			COORD pos;
			pos.Y = offset_y;
			pos.X = x_;
			WORD a = sections[index].getColor();
			WriteConsoleOutputCharacter(h, text.str().c_str(), text.str().length(), pos, &output);
			WriteConsoleOutputAttribute(h, &a, text.str().length(), pos, &output);
		}
		else
		{
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			int x_ = positionVars_.offset_x + positionVars_.border_width;
			DWORD output;
			COORD pos;
			pos.Y = offset_y;
			pos.X = x_;
			vector<WORD> attributes(text.str().length(), C_White);
			WriteConsoleOutputCharacter(h, text.str().c_str(), text.str().length(), pos, &output);
			WriteConsoleOutputAttribute(h, &attributes[0], text.str().length(), pos, &output);
			//WriteConsoleOutputAttribute(h, &a, text.str().length(), pos, &output);
		}*/
		if (index == curSelected_)
		{
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			int x_ = positionVars_.offset_x + positionVars_.border_width;
			DWORD output;
			COORD pos;
			pos.Y = offset_y;
			pos.X = x_;
			vector<WORD> attributes(text.str().length(), B_Blue | C_White);
			WriteConsoleOutputCharacter(h, text.str().c_str(), text.str().length(), pos, &output);
			WriteConsoleOutputAttribute(h, &attributes[0], text.str().length(), pos, &output);
		}
		else
		{
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			int x_ = positionVars_.offset_x + positionVars_.border_width;
			DWORD output;
			COORD pos;
			pos.Y = offset_y;
			pos.X = x_;
			vector<WORD> attributes(text.str().length(), C_White);
			WriteConsoleOutputCharacter(h, text.str().c_str(), text.str().length(), pos, &output);
			WriteConsoleOutputAttribute(h, &attributes[0], text.str().length(), pos, &output);
		}
	}
}

void UserInterface::drawBorder()
{
	positionVars_.border_width = 1;
	stringstream txt;
	for (int y = 0; y < positionVars_.size_y; y++)
	{
		SetCursorPosition(positionVars_.offset_x, positionVars_.offset_y+y);
		if (y == 0)
		{
			txt << (char)B_TOP_LEFT_CORNER
				<< string((positionVars_.size_x), B_HORIZONTAL)
				<< (char)B_TOP_RIGHT_CORNER;
			//std::cout << txt.str();
			int output_x = positionVars_.offset_x;
			for (int i = 0; i < txt.str().length(); i++)
			{
				Common::DisplayLetterAt(Position(output_x, positionVars_.offset_y + y), string() = txt.str()[i]);
				output_x++;
			}
			txt.str(string());
			continue;
		}
		else if (y == (positionVars_.size_y - 1))
		{
			txt << (char)B_BOTTOM_LEFT_CORNER
				<< string((positionVars_.size_x), B_HORIZONTAL)
				<< (char)B_BOTTOM_RIGHT_CORNER;
			//std::cout << txt.str();
			int output_x = positionVars_.offset_x;
			for (int i = 0; i < txt.str().length(); i++)
			{
				Common::DisplayLetterAt(Position(output_x, positionVars_.offset_y + y), string() = txt.str()[i]);
				output_x++;
			}
			txt.str(string());
			continue;
		}
		//std::cout << (char)B_VERTICAL;
		Common::DisplayLetterAt(Position(positionVars_.offset_x, positionVars_.offset_y + y), string() = (char)B_VERTICAL);
		SetCursorPosition(positionVars_.offset_x + positionVars_.border_width + positionVars_.size_x, positionVars_.offset_y + y);
		Common::DisplayLetterAt(Position(positionVars_.offset_x + positionVars_.border_width + positionVars_.size_x, positionVars_.offset_y + y), string() = (char)B_VERTICAL);
		//std::cout << (char)B_VERTICAL;
	}
}

void UserInterface::hideBorder()
{
	positionVars_.border_width = 1;
	stringstream txt;
	for (int y = 0; y < positionVars_.size_y; y++)
	{
		SetCursorPosition(positionVars_.offset_x, positionVars_.offset_y + y);
		if (y == 0)
		{
			txt << " "
				<< string((positionVars_.size_x), ' ')
				<< " ";
			cout << txt.str();
			txt.str(string());
			continue;
		}
		else if (y == (positionVars_.size_y - 1))
		{
			txt << " "
				<< string((positionVars_.size_x), ' ')
				<< " ";
			cout << txt.str();
			txt.str(string());
			continue;
		}
		cout << " ";
		SetCursorPosition(positionVars_.offset_x + positionVars_.border_width + positionVars_.size_x, positionVars_.offset_y + y);
		cout << " ";
	}
}

extern void setCursorPos(int x, int y);

void UserInterface::slideDraw(uint16_t index, string text)
{
	std::stringstream line;
	std::string output;
	setCursorPos(positionVars_.offset_x + positionVars_.border_width, index + positionVars_.offset_y + positionVars_.border_width);
	line << text;
	int length = line.str().length();
	if (length > positionVars_.size_y)
	{
		output = line.str().substr(0, positionVars_.size_x);
	}
	else
	{
		int spaces = positionVars_.size_x - length;
		if (spaces > 0)
		{
			line << std::string(spaces, ' ');
		}
		output = line.str();
	}
	std::cout << output;
}

void UserInterface::slideClear(uint16_t index)
{
	setCursorPos(positionVars_.offset_x, index);
	cout << "          ";
}

void UserInterface::reDrawAll()
{
	for (auto& iter: sections_)
	{
		redraws_.push_back(iter.second.getIndex());
	}
}

void UserInterface::SetCursorPosition(int x, int y)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(h, pos);
}

void UserInterface::hide(bool borderHide)
{
	for (auto& iter : sections_)
	{
		unsigned int offset_y = positionVars_.offset_y + positionVars_.border_width;
		offset_y += iter.second.getIndex() - 1;
		SetCursorPosition(positionVars_.offset_x + positionVars_.border_width, offset_y);
		string text = iter.second();
		if (iter.second.getIndex() == curSelected_)
			text += " <-";
		int length = text.length();
		int spaces = 0;
		if (length <= positionVars_.size_x)
		{
			spaces = positionVars_.size_x - length;
		}
		text += string(spaces, ' ');
		cout << createClearLine(text.length());
	}
	if (borderHide)
	{
		if (positionVars_.border_width > 0)
		{
			hideBorder();
		}
	}
}

void UserInterface::log(string text)
{
	fstream file("Logs\\Log.txt", ios::app);

	file << text << endl;

	file.close();
}

string UserInterface::createClearLine(int x)
{
	string text;
	for (int i = 0; i < x; i++)
	{
		text += " ";
	}
	return text;
}


/* Section */
/////////////////////////////////////////////////////
Section::Section()
{
	index_ = 0;
	varIndex_ = 0;
	text_ = "";
	isSelectable_ = false;
	isNumberHidden_ = true;
	isHidden_ = false;
	isISection_ = false;
	isColored_ = false;
	redraw_ = false;
	maxIText_ = 20;
	color_ = 0;
}

Section::Section(uint16_t index, string text)
{
	index_ = index;
	text_ = text;
	varIndex_ = 0;
	isSelectable_ = false;
	isNumberHidden_ = true;
	isHidden_ = false;
	redraw_ = false;
	isISection_ = false;
	isColored_ = false;
	maxIText_ = 20;
	color_ = 0;
}

Section::Section(uint16_t index, string text, bool isSelectable)
{
	index_ = index;
	text_ = text;
	varIndex_ = 0;
	isSelectable_ = isSelectable;
	isNumberHidden_ = true;
	isHidden_ = false;
	redraw_ = false;
	isISection_ = false;
	isColored_ = false;
	maxIText_ = 20;
	color_ = 0;
}

Section::Section(uint16_t index, string text, bool isSelectable, bool isNumberHidden)
{
	index_ = index;
	text_ = text;
	varIndex_ = 0;
	isSelectable_ = isSelectable;
	isNumberHidden_ = isNumberHidden;
	isHidden_ = false;
	redraw_ = false;
	isISection_ = false;
	isColored_ = false;
	maxIText_ = 20;
	color_ = 0;
}

uint16_t Section::getIndex()
{
	return index_;
}

void Section::isSelectable(bool isSelectable)
{
	isSelectable_ = isSelectable;
}

void Section::isNumberHidden(bool isHidden)
{
	isNumberHidden_ = isHidden;
}

void Section::isHidden(bool isHidden)
{
	isHidden_ = isHidden;
}

void Section::isColored(bool isColored)
{
	isColored_ = isColored;
	redraw_ = true;
}

void Section::setText(string text)
{
	text_ = text;
	redraw_ = true;
}

void Section::setIndex(uint16_t index)
{
	index_ = index;
}

void Section::setMaxIText(int length)
{
	maxIText_ = length;
}

void Section::addVar(int var)
{
	varIndex_++;

	Variable newVar(var);

	variables[varIndex_] = newVar;

	redraw_ = true;

	return;
}

void Section::addVar(double var)
{
	varIndex_++;

	Variable newVar(var);

	variables[varIndex_] = newVar;

	redraw_ = true;

	return;
}

void Section::addVar(string var)
{
	varIndex_++;

	Variable newVar(var);

	variables[varIndex_] = newVar;
	
	redraw_ = true;

	return;
}

void Section::addIText(string iText)
{
	iText_ += iText;
	/*if (iText_.length() > maxIText_)
	{
		iText_ = iText_.substr(0, maxIText_);
	}*/
	redraw_ = true;
}

void Section::removeIText()
{
	if (iText_.empty()==false)
	{
		iText_.pop_back();
		redraw_ = true;
	}
}

void Section::push_backVar(int var)
{
	varIndex_++;

	Variable newVar(var);

	variables[varIndex_] = newVar;

	redraw_ = true;

	return;
}

void Section::push_backVar(double var)
{
	varIndex_++;

	Variable newVar(var);

	variables[varIndex_] = newVar;

	redraw_ = true;

	return;
}

void Section::push_backVar(string var)
{
	varIndex_++;

	Variable newVar(var);

	variables[varIndex_] = newVar;

	redraw_ = true;

	return;
}

void Section::setVar(uint16_t varIndex, int var)
{
	if (variables.count(varIndex))
	{
		if (variables[varIndex].getInt() == var)
		{
			return;
		}
		else
		{
			variables[varIndex].setVar(var);
			redraw_ = true;
		}
	}
}

void Section::setVar(uint16_t varIndex, double var)
{
	if (variables.count(varIndex))
	{
		if (variables[varIndex].getDou() == var)
		{
			return;
		}
		else
		{
			variables[varIndex].setVar(var);
			redraw_ = true;
		}
	}
}

void Section::setVar(uint16_t varIndex, string var)
{
	if (variables.count(varIndex))
	{
		if (variables[varIndex].getStr() == var)
		{
			return;
		}
		else
		{
			variables[varIndex].setVar(var);
			redraw_ = true;
		}
	}
}

void Section::setIVar(string text)
{
	iText_ = text;
	if (iText_.length() > maxIText_)
	{
		iText_ = iText_.substr(0, maxIText_);
	}
	redraw_ = true;
}

void Section::setIText(string text)
{
	text_ = text;
	redraw_ = true;
}

void Section::isISection(bool isI)
{
	isISection_ = isI;
	if (isI)
	{
		maxIText_ = 20;
	}
	redraw_ = true;
}

int Section::getVarI(uint16_t varIndex)
{
	if (variables.count(varIndex))
	{
		return variables[varIndex].getInt();
	}
	return 0;
}

double Section::getVarD(uint16_t varIndex)
{
	if (variables.count(varIndex))
	{
		return variables[varIndex].getDou();
	}
	return 0.0;
}

string Section::getVarS(uint16_t varIndex)
{
	if (variables.count(varIndex))
	{
		return variables[varIndex].getStr();
	}
	return "ERROR:VARDOESNOTEXIST";
}

string Section::getIVar()
{
	if (isISection_)
	{
		return iText_;
	}
	else
	{
		return "ERROR:NOTISECTION";
	}
}

string Section::operator()()
{
	stringstream text;
	if (isHidden_ == true)
		return "               ";
	if (isISection_ == false)
	{
		if (isNumberHidden_ == false)
		{
			text << index_ << ".";
		}
		text << text_;
		if (variables.empty() == false)
		{
			for (auto &iter : variables)
			{
				text << " <";
				switch (iter.second.getVarIndex())
				{
				case 1: text << iter.second.getInt(); break;
				case 2: text << iter.second.getDou(); break;
				case 3: text << iter.second.getStr(); break;
				}
				text << ">";
			}
		}
	}
	else
	{
		text << text_ << " " << iText_;
	}
	return text.str();
}

void Section::setColor(WORD color)
{
	color_ = color;
	redraw_ = true;
}

WORD Section::getColor()
{
	return color_;
}

bool Section::isHidden()
{
	return isHidden_;
}

bool Section::isSelectable()
{
	return isSelectable_;
}

bool Section::isISection()
{
	return isISection_;
}

bool Section::isColored()
{
	return isColored_;
}

bool Section::redraw()
{
	if (redraw_ == true)
	{
		redraw_ = false;
		return true;
	}
	return false;
}

void Section::log(string text)
{
	fstream file("Logs\\Log.txt", ios::app);

	file << text << endl;
}

bool Section::isNumberHidden()
{
	return isNumberHidden_;
}
/////////////////////////////////////////////////////


/* Variable */
/////////////////////////////////////////////////////
Variable::Variable()
{
	iVar_ = 0;
	dVar_ = 0.0;
	sVar_ = "";
	isValid_ = true;
}

Variable::Variable(int var)
{
	iVar_ = var;
	dVar_ = 0.0;
	sVar_ = "";
	isValid_ = true;
	varIndex_ = 1;
}

Variable::Variable(double var)
{
	iVar_ = 0;
	dVar_ = var;
	sVar_ = "";
	isValid_ = true;
	varIndex_ = 2;
}

Variable::Variable(string var)
{
	iVar_ = 0;
	dVar_ = 0.0;
	sVar_ = var;
	isValid_ = true;
	varIndex_ = 3;
}

void Variable::setVar(int var)
{
	iVar_ = var;
	varIndex_ = 1;
}

void Variable::setVar(double var)
{
	sVar_ = var;
	varIndex_ = 2;
}

void Variable::setVar(string var)
{
	sVar_ = var;
	varIndex_ = 3;
}

int Variable::getVarIndex()
{
	return varIndex_;
}

int Variable::getInt()
{
	return iVar_;
}

double Variable::getDou()
{
	return dVar_;
}

string Variable::getStr()
{
	return sVar_;
}

void Variable::isValid(bool isValid)
{
	isValid_ = isValid;
}

bool Variable::isValid()
{
	return isValid_;
}

void Variable::operator()(int var)
{
	iVar_ = var;
}

void Variable::operator()(double var)
{
	dVar_ = var;
}

void Variable::operator()(string var)
{
	sVar_ = var;
}
/////////////////////////////////////////////////////