#include "stdafx.h"
#include "ConnectionController.cpp"

class RTSPController : public ConnectionController {
public:
	int client_port;
	int numMessages;

	RTSPController() : ConnectionController() {
		numMessages = 0;
	}

	void setIPAndPort(string _IPAddress, int _port, int _clientPort) {
		ConnectionController::setIPAndPort(_IPAddress, _port);
		client_port = _clientPort;
	}

	char* sendMessage(string message) {
		char *buff = ConnectionController::sendMessage(message);
		numMessages++;
		return buff;
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
		char *buff = sendMessage(message);
		numMessages = 0;
		return buff;
	}

	void setSessionIDFromResponseBuffer(char* response) {
		string identifier = "Session: ";

		char *s = strstr(response, "Session: ");

		if (s == NULL) {
			printf("\nIdentifier string: '%s' not found. Session not set.\n", identifier.c_str());
			return;
		}

		string sessionID = "";

		for (int i = s + identifier.length() - response; i < 1024; i++) {
			if (response[i] == ';')
				break;
			sessionID += response[i];
		}

		session = sessionID;
	}
};
