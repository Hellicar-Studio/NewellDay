#include "stdafx.h"
#ifdef _WIN32
#include <WinSock2.h>
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib") //Winsock Library
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <iostream>
#include <sstream>

using namespace std;

class RTSPController {
public:
	string IPAddress, session;
	int port, client_port;
	int valread, numMessages;
	char buffer[1024];
	struct sockaddr_in server;

#ifdef _WIN32
	WSADATA wsa;
	SOCKET sock;
#else
	int sock;
#endif

	RTSPController() {
#ifndef _WIN32
		sock = 0;
#endif
		memset(buffer, 0, sizeof(buffer));
		numMessages = 0;
	}

#ifdef _WIN32
	int initializeWinSock() {
		printf("\nInitialising Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			printf("Failed. Error Code: %d", WSAGetLastError());
			return 1;
		}
		printf("Initialised.");

		return 0;
	}
#endif

	void setIPAndPort(string _IPAddress, int _port, int _clientPort) {
		IPAddress = _IPAddress;
		port = _port;
		client_port = _clientPort;
	}

	int connectToServer() {
#ifdef _WIN32
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
		}
#else
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			return 1;
		}
		memset(&server, '0', sizeof(server));
#endif

		server.sin_addr.s_addr = inet_addr(IPAddress.c_str()); // REMEMBER THIS! IT MIGHT CAUSE AN ERROR!
		server.sin_family = AF_INET;
		server.sin_port = htons(port);

#ifndef _WIN32
		// Convert IPv4 and IPv6 addresses from text to binary form
		if (inet_pton(AF_INET, IPAddress.c_str(), &server.sin_addr) <= 0) {
			printf("\nInvalid address/ Address not supported \n");
			return 1;
		}
#endif

		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
			printf("\nConnection Failed \n");
			return 1;
		}
		return 0;
	}

	char* sendMessage(string message) {
		send(sock, message.c_str(), message.length(), 0);
		numMessages++;
#ifdef _WIN32
		valread = recv(sock, buffer, 1024, 0);
#else
		valread = read(sock, buffer, 1024);
#endif
		printf("%s\n", buffer);
		return buffer;
	}

	char * sendHttpMessageFast(string message) {
		send(sock, message.c_str(), message.length(), 0);
		return "Fast Return";
	}

	char* sendActivateVirtualPort(int virtualPort) {
		ostringstream oss;
		oss << "GET /axis-cgi/virtualinput/activate.cgi?schemaversion=1&port=" << virtualPort << " HTTP/1.1\r\nHost: "<< IPAddress << "\r\nCache-Control: max-age=0\r\nAuthorization: Basic cm9vdDpwYXNzd29yZA==\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Sunrise Recorder\r\n\r\n";
		string message = oss.str();
		printf("\n%s", message.c_str());
		printf("\nSending Virtual Port Activate Request\n");
		return sendMessage(message);
	}

	char* sendDeactivateVirtualPort(int virtualPort) {
		ostringstream oss;
		oss << "GET /axis-cgi/virtualinput/deactivate.cgi?schemaversion=1&port=" << virtualPort << " HTTP/1.1\r\nHost: " << IPAddress << "\r\nCache-Control: max-age=0\r\nAuthorization: Basic cm9vdDpwYXNzd29yZA==\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Sunrise Recorder\r\n\r\n";
		string message = oss.str();
		printf("\nSending Virtual Port Deactivate Request\n");
		return sendMessage(message);
	}

	char* sendOptions() {
		ostringstream oss;
		oss << "OPTIONS rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\n\r\n";
		string message = oss.str();
		printf("\nSending Options Request\n");
		return sendMessage(message);
	}

	char* sendDescribe() {
		ostringstream oss;
		oss << "DESCRIBE rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\nAccept: application/sdp\r\n\r\n";
		string message = oss.str();
		printf("\nSending Describe Request\n");
		return sendMessage(message);
	}

	char* sendSetup() {
		ostringstream oss;
		oss << "SETUP rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nTransport: RTP/AVP;unicast;client_port=" << client_port << "-" << client_port + 1 << "\r\n\r\n";
		string message = oss.str();
		printf("\nSending Setup Request\n");
		return sendMessage(message);
	}

	char* setupSession() {
		char* resp = sendSetup();
		setSessionIDFromResponseBuffer(resp);
		return resp;
	}

	char* sendPlay() {
		ostringstream oss;
		oss << "PLAY rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << session << "\r\nRange: npt=0.000-\r\n\r\n";
		string message = oss.str();
		printf("\nSending Play Request\n");
		return sendMessage(message);
	}

	char* sendPause() {
		ostringstream oss;
		oss << "PAUSE rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << session << "\r\n\r\n";
		string message = oss.str();
		printf("\nSending Pause Request\n");
		return sendMessage(message);
	}

	char* sendTeardown() {
		ostringstream oss;
		oss << "TEARDOWN rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << session << "\r\n\r\n";
		string message = oss.str();
		printf("\nSending Teardown Request\n");
		return sendMessage(message);
	}

	void setSessionIDFromResponseBuffer(char* response) {
		string identifier = "Session: ";

		char *s = strstr(response, "Session: ");

		string sessionID = "";

		for (int i = s + identifier.length() - response; i < 1024; i++) {
			if (response[i] == ';')
				break;
			sessionID += response[i];
		}

		session = sessionID;
	}
};
