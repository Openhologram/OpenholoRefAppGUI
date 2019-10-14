#pragma once
class Time
{
private:
	Time();
	~Time();
	static Time *instance;
	static void Destroy() {
		delete instance;
	}
public:
	static Time* getInstance() {
		if (instance == nullptr) {
			instance = new Time();
			atexit(Destroy);
		}
		return instance;
	}
	CString GetTime(CString szDate = L"");
	CString GetDate(CString szTime = L"");
};

