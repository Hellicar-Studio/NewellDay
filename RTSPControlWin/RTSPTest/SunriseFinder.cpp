#include "stdafx.h"
#include <Windows.h>

class SunriseFinder {
public:
	SunriseFinder() {};
	~SunriseFinder() {};
	SYSTEMTIME st, lt;
	void setTime() {
		GetSystemTime(&st);
		GetLocalTime(&lt);
		printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
		printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);
	}
private:

};