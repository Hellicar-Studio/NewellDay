#include "stdafx.h"
#include "ConnectionController.cpp"
#include <Windows.h>

class SunriseFinder : public ConnectionController {
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
	string getHostnameFromURL(string URL) {
		string httpsIndicator = "https://";
		size_t start = URL.find(httpsIndicator);
		string hostZone = URL.substr(start + httpsIndicator.length());

		size_t end = hostZone.find("/");
		string host = hostZone.substr(0, end);

		return host;
	}
	string getIPAddressFromHostname(string host) {
		hostent *record = gethostbyname(host.c_str());
		if (record == NULL) {
			cout << "\n" << host << " is unavailable. Error: " << WSAGetLastError() << endl;
			return "";
		}
		in_addr *address = (in_addr*)record->h_addr;
		string ip_address = inet_ntoa(*address);
		return ip_address;
	}

	void setIPAndPort(string _url, int _port) {
		IPAddress = getIPAddressFromHostname(getHostnameFromURL(_url));
		port = _port;
		cout << "IPAddress: " << IPAddress << endl;
	}

	string sendGetSunrise() {
		ostringstream oss;
		oss << "GET /json?lat=36.7201600&lng=-4.4203400 HTTP/1.1\r\nHost: api.sunrise-sunset.org\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Sunrise Master\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\nAccept-encoding: gzip, defalt\r\nAccept-Languafe: en-US,en;q=0.9\r\n\r\n";
		string message = oss.str();
		string resp = sendMessage(message);
		cout << resp << endl;
		return message;
	}
private:

};