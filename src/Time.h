#pragma once
class Time
{
public:
	Time();
	~Time();
	static CString GetTime(CString szDate = L"");
	static CString GetDate(CString szTime = L"");
};

