#include <sstream>
#include "Message.h"
#include "Packet.h"
#include "Common.h"


Message::Message()
{
}

Message::Message(bool isSendDefault)
{
	if(isSendDefault == true)
	{
		msg << SendDefault << EndLine;
	}
}

Message::Message(int sendTo)
{
	msg << sendTo << EndLine;
}

Message::~Message()
{

}

Message& Message::operator<<(int value)
{
	msg << value << EndLine;
	return *this;
}

Message& Message::operator<<(std::string value)
{
	msg << value << EndLine;
	return *this;
}

Message& Message::operator<<(double value)
{
	msg << value << EndLine;
	return *this;
}

Message& Message::operator<<(WORD value)
{
	msg << value << EndLine;
	return *this;
}

Message& Message::operator<<(char value)
{
	msg << (int)value << EndLine;
	return *this;
}

Message & Message::operator<<(bool value)
{
	msg << value << EndLine;
	return *this;
}

void Message::SendAndReset()
{
	SendServerLiteral(msg.str());
	msg.clear();
}

void Message::Send()
{
	SendServerLiteral(msg.str());
}

std::string Message::getMessage()
{
	return msg.str();
}

std::stringstream & Message::get()
{
	return msg;
}

void Message::clear()
{
	msg.clear();
}
