#pragma once

#include <string>
#include <Position.h>
#include <PositionVariables.h>
#include <map>
#include <Timer.h>
#include <vector>
#include <InputManager.h>
#include <thread>

class Variable
{
public:
	Variable();
	Variable(int var);
	Variable(double var);
	Variable(std::string var);

	void setVar(int var);
	void setVar(double var);
	void setVar(std::string var);

	int getVarIndex();

	int getInt();
	double getDou();
	std::string getStr();

	void isValid(bool isValid);
	bool isValid();

	void operator()(int);
	void operator()(double);
	void operator()(std::string);
private:
	int iVar_;
	double dVar_;
	std::string sVar_;
	int varIndex_;
	bool isValid_;
};

class Section
{
public:
	Section();
	Section(uint16_t index, std::string text);
	Section(uint16_t index, std::string text, bool isSelectable);
	Section(uint16_t index, std::string text, bool isSelectable, bool isNumberShown);

	uint16_t getIndex();

	void isSelectable(bool isSelectable);
	void isISection(bool isISection);
	void isNumberHidden(bool isHidden);
	void isHidden(bool isHidden);
	void setText(std::string text);
	void setIndex(uint16_t index);
	void setMaxIText(int length);

	void addVar(int var);
	void addVar(double var);
	void addVar(std::string var);

	void addIText(std::string iText);

	void removeIText();

	void push_backVar(int var);
	void push_backVar(double var);
	void push_backVar(std::string var);

	void setVar(uint16_t varIndex, int var);
	void setVar(uint16_t varIndex, double var);
	void setVar(uint16_t varIndex, std::string var);
	void setIVar(std::string text);
	void setIText(std::string text);


	int getVarI(uint16_t varIndex);
	double getVarD(uint16_t varIndex);
	std::string getVarS(uint16_t varIndex);
	std::string getIVar();

	std::string operator()();


	bool isHidden();
	bool isSelectable();
	bool isNumberHidden();
	bool isISection();
	bool redraw();
private:
	uint16_t index_;
	uint16_t varIndex_;
	int maxIText_;
	bool isSelectable_;
	bool isNumberHidden_;
	bool isHidden_;
	bool isISection_;
	bool redraw_;
	std::string iText_;
	std::string text_;
	std::map <uint16_t, Variable> variables; // DFNC - Doesn't Follow Naming Convention

	void log(std::string text);
};

class UserInterface
{
public:
	UserInterface();
	UserInterface(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y);
	UserInterface(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y, unsigned int border_width);
	~UserInterface();

	void isHidden(bool isHidden);
	bool isHidden();

	void setSizeX(unsigned int size_x);
	void setSizeY(unsigned int size_y);
	void setOffsetX(unsigned int offset_x);
	void setOffsetY(unsigned int offset_y);
	void setBorderWidth(unsigned int border_width);
	void setAll(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y, unsigned int border_width);

	void addSection(Section newSection);
	void addSection(std::string text);
	void addSection(std::string text, bool isSelectable);
	void addSection(std::string text, bool isSelectable, bool isNumberHidden);

	void addSlide(std::string text);

	void push_back(Section newSection);
	void push_back(std::string text);
	void push_back(std::string text, bool isSelectable);
	void push_back(std::string text, bool isSelectable, bool isNumberHidden);

	void push_isection(std::string text);
	void push_isection(std::string text, int maxIText);

	void setPositionVariables(PositionVariables posV);

	Section& getSectionRef(uint16_t sectionIndex);
	Section getSection(uint16_t sectionIndex);

	bool getSectionRefT(uint16_t sectionIndex, Section** section); // Trys to get the section ref, but can fail if it doesn't exist without crashing

	bool isSectionActivated();
	int getActivatedSection();

	void initializeSlideUI();

	void moveSelectionUp();
	void moveSelectionDown();

	void moveSlideDown();

	void update();
	void reset();
	void draw(uint16_t index);
	void drawBorder();
	void hideBorder();
	void slideDraw(uint16_t index, std::string text);
	void slideClear(uint16_t index);
	void reDrawAll();
private:
	PositionVariables positionVars_;
	uint16_t sectionAmount_;
	uint16_t curSelected_;
	Timer inputCoolDown;
	bool isHidden_;
	bool isPosVariablesSet_;
	bool isSelectionActivated_;
	bool isInIMode_;
	bool isSlideUI_;

	InputManager iManager;
	InputManager input;

	std::map<uint16_t, Section> sections; // DFNC - Doesn't Follow Naming Convention
	std::vector<uint16_t> redraws; // DFNC - Doesn't Follow Naming Convention
	std::list<std::string> slideSectionAdd_;
	std::map<uint16_t, std::string> SlideSections; // DFNC - Doesn't Follow Naming Convention

	bool isSelectionAvailable_;

	void SetCursorPosition(int x, int y);
	void hide();
	void log(std::string text);
	std::string createClearLine(int x);
};

