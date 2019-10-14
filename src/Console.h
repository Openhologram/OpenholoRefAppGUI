#pragma once
class Console
{
private:
	Console();
	~Console();
	static Console *instance;
	static void Destroy() {
		delete instance;
	}
public:
	enum class Color : WORD {
		BLACK,
		DARK_BLUE,
		DARK_GREEN,
		DARK_CYAN,
		DARK_RED,
		DARK_MAGENTA,
		DARK_YELLOW,
		GRAY,
		DARK_GRAY,
		BLUE,
		GREEN,
		CYAN,
		RED,
		MAGENTA,
		YELLOW,
		WHITE
	};
public:
	static Console* getInstance() {
		if (instance == nullptr) {
			instance = new Console();
			atexit(Destroy);
		}
		return instance;
	}
	BOOL SetColor(Color text, Color background);
	BOOL ResetColor();
private:
	WORD m_textColor;
	WORD m_bgColor;
};

