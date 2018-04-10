// RTSPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <WinSock2.h>
#include "Ws2tcpip.h"

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib") //Winsock Library

#define _WINSOCK_DEPRECATED_NO_WARNINGS
using namespace std;

class RTSPController {
public:

	RTSPController() {
	}

	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	string IPAddress;
	int port;
	int client_port;

	int initializeWinSock() {
		printf("\nInitialising Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			printf("Failed. Error Code: %d", WSAGetLastError());
			return 1;
		}
		printf("Initialised.");

		return 0;
	}

	void createSocket() {
		if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
		}

		printf("Socket created.\n");
	}

	void setServerIPAndPort(string IP, int _port, int _client_port) {
		IPAddress = IP;
		port = _port;
		client_port = _client_port;
		server.sin_addr.s_addr = inet_addr(IP.c_str());
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
	}

	int connectToRemoteServer() {
		if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
			puts("connect error");
			return 1;
		}

		puts("Connected");

		return 0;
	}
};