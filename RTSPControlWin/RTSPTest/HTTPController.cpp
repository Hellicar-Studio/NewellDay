#include "stdafx.h"
#include "ConnectionController.cpp"

class HTTPController : public ConnectionController {
	char * sendHttpMessageFast(string message) {
		send(sock, message.c_str(), message.length(), 0);
		return "Fast Return";
	}

	char* sendActivateVirtualPort(int virtualPort) {
		ostringstream oss;
		oss << "GET /axis-cgi/virtualinput/activate.cgi?schemaversion=1&port=" << virtualPort << " HTTP/1.1\r\nHost: " << IPAddress << "\r\nCache-Control: max-age=0\r\nAuthorization: Basic cm9vdDpwYXNzd29yZA==\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Sunrise Recorder\r\n\r\n";
		string message = oss.str();
		cout << "\n"<< message.c_str() << "\n";
		cout << "\nSending Virtual Port Activate Request\n";
		return sendHttpMessageFast(message);
	}

	char* sendDeactivateVirtualPort(int virtualPort) {
		ostringstream oss;
		oss << "GET /axis-cgi/virtualinput/deactivate.cgi?schemaversion=1&port=" << virtualPort << " HTTP/1.1\r\nHost: " << IPAddress << "\r\nCache-Control: max-age=0\r\nAuthorization: Basic cm9vdDpwYXNzd29yZA==\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Sunrise Recorder\r\n\r\n";
		string message = oss.str();
		cout << "\nSending Virtual Port Deactivate Request\n";
		return sendHttpMessageFast(message);
	}
};