#include "stdafx.h"
#include "ConnectionController.cpp"
#include <Windows.h>

class SunriseFinder : ConnectionController {
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
	string getHostnameFromURL(string URL) {
		string httpIndicator = "http://";
		size_t start = URL.find(httpIndicator);
		string hostZone = URL.substr(start + httpIndicator.length());

		size_t end = hostZone.find("/");
		string host = hostZone.substr(0, end);

		return host;
	}
};