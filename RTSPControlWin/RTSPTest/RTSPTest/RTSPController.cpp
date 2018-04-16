#include "stdafx.h"
#include "ConnectionController.cpp"

struct StreamProfile {
public:
	string sessionID;
	string pictureAndSequenceParameters;
	int client_port;
};

class RTSPControllerListener {
public:
	virtual ~RTSPControllerListener() {};
	virtual void onRTSPControllerIsSetup(StreamProfile streamProfile) = 0;
};

class RTSPController : public ConnectionController {
public:
	StreamProfile streamProfile;
	int numMessages;
	RTSPControllerListener* listener = nullptr;

	RTSPController() : ConnectionController() {
		numMessages = 0;
	}

	int init(RTSPControllerListener* lis) {
		listener = lis;
	}

	void setIPAndPort(string _IPAddress, int _port, int _clientPort) {
		ConnectionController::setIPAndPort(_IPAddress, _port);
		streamProfile.client_port = _clientPort;
	}

	char* sendMessage(string message) {
		char *buff = ConnectionController::sendMessage(message);
		numMessages++;
		return buff;
	}

	char* sendOptions() {
		ostringstream oss;
		if (streamProfile.sessionID == "") {
			oss << "OPTIONS rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\n\r\n";
		}
		else {
			oss << "OPTIONS rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\nSession: "<< streamProfile.sessionID <<"\r\n\r\n";
		}
		string message = oss.str();
		printf("\n%s\n", message.c_str());
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
		oss << "SETUP rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nTransport: RTP/AVP;unicast;client_port=" << streamProfile.client_port << "-" << streamProfile.client_port + 1 << "\r\n\r\n";
		string message = oss.str();
		printf("\nSending Setup Request\n");
		return sendMessage(message);
	}

	char* setupSession() {
		char* resp = sendSetup();
		streamProfile.sessionID = getParamFromResponseBuffer(resp, "Session: ", ";");
		streamProfile.pictureAndSequenceParameters = getParamFromResponseBuffer(resp, "sprop-parameter-sets=", "\r\n");
		listener->onRTSPControllerIsSetup(streamProfile);
		return resp;
	}

	char* sendPlay() {
		ostringstream oss;
		oss << "PLAY rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << streamProfile.sessionID << "\r\nRange: npt=0.000-\r\n\r\n";
		string message = oss.str();
		printf("\nSending Play Request\n");
		return sendMessage(message);
	}

	char* sendPause() {
		ostringstream oss;
		oss << "PAUSE rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << streamProfile.sessionID << "\r\n\r\n";
		string message = oss.str();
		printf("\nSending Pause Request\n");
		return sendMessage(message);
	}

	char* sendTeardown() {
		ostringstream oss;
		oss << "TEARDOWN rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << streamProfile.sessionID << "\r\n\r\n";
		string message = oss.str();
		printf("\nSending Teardown Request\n");
		char *buff = sendMessage(message);
		numMessages = 0;
		return buff;
	}

	string getParamFromResponseBuffer(char* response, string StartIdentifier, string EndIdentifier) {
		string resp = response;

		size_t start = resp.find(StartIdentifier);

		string paramZone = resp.substr(start + StartIdentifier.length());

		size_t end = paramZone.find(EndIdentifier);
		string param = paramZone.substr(0, end);

		if (param == "") {
			printf("\nIdentifier string: '%s' not found. Param not returned.\n", StartIdentifier.c_str());
			return "";
		}

		return param;
	}
};
