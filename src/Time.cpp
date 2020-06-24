#include "stdafx.h"
#include "Time.h"


Time* Time::instance = nullptr;
Time::Time()
{
}


Time::~Time()
{
}

CString Time::GetTime(CString szTime)
{
	CString szRet = szTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	szRet.AppendFormat(L"_%04d-%02d-%02d %02d.%02d.%02d.%03d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return szRet;
}

CString Time::GetDate(CString szDate)
{
	CString szRet = szDate;
	SYSTEMTIME st;
	GetLocalTime(&st);
	szRet.AppendFormat(L"_%04d-%02d-%02d",
		st.wYear, st.wMonth, st.wDay);
	return szRet;
}