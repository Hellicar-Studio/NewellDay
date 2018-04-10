#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

class RTSPController {
public:
	string IPAddress;
	int port;
	string session;
	int client_port;
	int numMessages;

	RTSPController() {
		sock = 0;
		memset(buffer, 0, sizeof(buffer));
		numMessages = 0;
	}

	void setIPAndPort(string _IPAddress, int _port, int _clientPort) {
		IPAddress = _IPAddress;
		port = _port;
		client_port = _clientPort;
	}

	char* sendOptions() {
		ostringstream oss;
		oss << "OPTIONS rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\n\r\n";
		string message = oss.str();
		send(sock, message.c_str(), message.length(), 0);
		printf("\nSent Options Request\n");
		numMessages++;
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendDescribe() {
		ostringstream oss;
		oss << "DESCRIBE rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\nAccept: application/sdp\r\n\r\n";
		string message = oss.str();
		send(sock, message.c_str(), message.length(), 0);
		printf("\nSent Describe Request\n");
		numMessages++;
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendSetup() {
		ostringstream oss;
		oss << "SETUP rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nTransport: RTP/AVP;unicast;client_port=" << client_port << "-" << client_port+1<<"\r\n\r\n";
		string message = oss.str();
		send(sock, message.c_str(), message.length(), 0);
		printf("\nSent Setup Request\n");
		numMessages++;
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
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
		send(sock, message.c_str(), message.length(), 0);
		printf("\nSent Play Request\n");
		numMessages++;
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendPause() {
		ostringstream oss;
		oss << "PAUSE rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << "\r\n\r\n";
		string message = oss.str();
		send(sock, message.c_str(), message.length(), 0);
		printf("\nSent Pause Request\n");
		numMessages++;
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendTeardown() {
		ostringstream oss;
		oss << "TEARDOWN rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << "\r\n\r\n";
		string message = oss.str();
		send(sock, message.c_str(), message.length(), 0);
		printf("\nSent Teardown Request\n");
		numMessages = 0;
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	int connectToServer() {
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			return -1;
		}

		memset(&serv_addr, '0', sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);

		// Convert IPv4 and IPv6 addresses from text to binary form
		if(inet_pton(AF_INET, IPAddress.c_str(), &serv_addr.sin_addr)<=0) {
			printf("\nInvalid address/ Address not supported \n");
			return -1;
		}
		if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			printf("\nConnection Failed \n");
			return -1;
		}
		return 1;
	}

	void setSessionIDFromResponseBuffer(char* response) {
		string identifier = "Session: ";

		char *s = strstr(response, "Session: ");

		string sessionID = "";

		for(int i = s + identifier.length() - response; i < 1024; i++) {
			if(response[i] == ';')
				break;
			sessionID += response[i];
		}

		session = sessionID;
	}

private:
	struct sockaddr_in address;
	int sock, valread;
	struct sockaddr_in serv_addr;
	char buffer[1024];
};
