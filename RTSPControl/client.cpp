#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 80

using namespace std;

class socketConnection {
public:
	string IPAddress;
	int port;
	socketConnection() {
		sock = 0;
		memset(buffer, 0, sizeof(buffer));
	}
	int createSocket() {
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\ncreateSocket: Socket creation error \n");
			return -1;
		}
		return 1;
	}
	void setIPAndPort(string _IPAddress, int _port) {
		IPAddress = _IPAddress;
		port = _port;
		options = "OPTIONS rtsp://" + _IPAddress + ":" + to_string(port) + "RTSP/1.0\r\nCSeq: 1";
		describe = "DESCRIBE rtsp://" + _IPAddress + ":" + to_string(port) + "RTSP/1.0\r\nCSeq: 2";
		setup = "SETUP rtsp://" + _IPAddress + ":" + to_string(port) + "RTSP/1.0\r\nCSeq: 3\r\nTransport: RTP/AVP;unicast;client_port=";
		play = "PLAY rtsp://" + _IPAddress + ":" + to_string(port) + "RTSP/1.0\r\nCSeq: 4\r\nSession: ";
		pause = "PAUSE rtsp://" + _IPAddress + ":" + to_string(port) + "RTSP/1.0\r\nCSeq: 5\r\nSession: ";
		teardown = "TEARDOWN rtsp://" + _IPAddress + ":" + to_string(port) + "RTSP/1.0\r\nCSeq: 6\r\nSession: ";
	}

	char* sendOptions() {
		const char *stringToSend = options.c_str();
		send(sock, stringToSend, strlen(stringToSend), 0);
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendDescribe() {
		const char *stringToSend = describe.c_str();
		send(sock, stringToSend, strlen(stringToSend), 0);
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendSetup() {
		const char *stringToSend = setup.c_str();
		send(sock, stringToSend, strlen(stringToSend), 0);
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendPlay() {
		const char *stringToSend = (play + to_string(session)+"\r\n\r\n").c_str();
		send(sock, stringToSend, strlen(stringToSend), 0);
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendPause() {
		const char *stringToSend = (pause + to_string(session)+"\r\n\r\n").c_str();
		send(sock, stringToSend, strlen(stringToSend), 0);
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

	char* sendTeardown() {
		const char *stringToSend = (teardown + to_string(session)+"\r\n\r\n").c_str();
		send(sock, stringToSend, strlen(stringToSend), 0);
		valread = read( sock, buffer, 1024);
		printf("%s\n", buffer );
		return buffer;
	}

private:
	struct sockaddr_in address;
	int sock, valread;
	struct sockaddr_in serv_addr;
	char buffer[1024];
	int session;

	string options;
	string describe;
	string setup;
	string play;
	string pause;
	string teardown;

};

int main(int argc, char const  *argv[]) {

	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	string options = "OPTIONS rtsp://184.72.239.149:80 RTSP/1.0\r\nCSeq: 1\r\n\r\n";
	string describe = "DESCRIBE rtsp://184.72.239.149:80 RTSP/1.0\r\nCSeq: 1\r\n\r\n";
	string setup = "SETUP rtsp://184.72.239.149:80 RTSP/1.0\r\nCSeq: 1\r\nTransport: RTP/AVP;unicast;client_port=8000-8001\r\n\r\n";
	string play = "PLAY rtsp://184.72.239.149:80 RTSP/1.0\r\nCSeq: 2\r\nSession: 978485885\r\n\r\n";
	string pause = "PAUSE rtsp://184.72.239.149:80 RTSP/1.0\r\nCSeq: 3\r\nSession: 978485885\r\n\r\n";
	string teardown = "TEARDOWN rtsp://184.72.239.149:80 RTSP/1.0\r\nCSeq: 4\r\nSession: 978485885\r\n\r\n";

	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "184.72.239.149", &serv_addr.sin_addr)<=0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	const char *stringToSend = play.c_str();

	send(sock, stringToSend, strlen(stringToSend), 0);
	printf("Message sent\n");
	valread = read( sock, buffer, 1024);
	printf("%s\n", buffer );
	return 0;
	return 0;
}