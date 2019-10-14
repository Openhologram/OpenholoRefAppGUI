#include "stdafx.h"
#include "Console.h"

Console* Console::instance = nullptr;
Console::Console()
{
	m_textColor = (WORD)Color::GRAY;
	m_bgColor = (WORD)Color::BLACK;
}


Console::~Console()
{
}

BOOL Console::SetColor(Color text, Color background)
{
	WORD t = (WORD)text;
	WORD b = (WORD)background;
	return SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), t | (b << 4));
}

BOOL Console::ResetColor()
{
	return SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_textColor | (m_bgColor << 4));
}