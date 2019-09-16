#pragma once
class Time
{
public:
	Time();
	~Time();
	CString GetTime(CString szDate = L"");
	CString GetDate(CString szTime = L"");
};

