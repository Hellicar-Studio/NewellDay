#include "stdafx.h"
#include "ConnectionController.cpp"
#include <fstream>


struct StreamProfile {
public:
	string sessionID;
	string pictureAndSequenceParameters;
	int client_port;
};

class RTSPControllerListener {
public:
	virtual ~RTSPControllerListener() {};
	virtual void onRTSPControllerIsSetup(StreamProfile streamProfile) {};
};

class RTSPController : public ConnectionController {
public:
	StreamProfile streamProfile;
	int numMessages;
	RTSPControllerListener* listener = nullptr;

	RTSPController() : ConnectionController() {
		numMessages = 0;
	}

	int addListener(RTSPControllerListener* lis) {
		if (nullptr == lis) {
			cout << "Given listener is a nullptr.";
			return -1;
		}
		listener = lis;
		return 0;
	}

	void setIPAndPort(string _IPAddress, int _port, int _clientPort) {
		ConnectionController::setIPAndPort(_IPAddress, _port);
		streamProfile.client_port = _clientPort;
	}

	string sendMessage(string message) {
		string buff = ConnectionController::sendMessage(message);
		numMessages++;
		return buff;
	}

	string sendOptions() {
		ostringstream oss;
		if (streamProfile.sessionID == "") {
			oss << "OPTIONS rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\n\r\n";
		}
		else {
			oss << "OPTIONS rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\nSession: "<< streamProfile.sessionID <<"\r\n\r\n";
		}
		string message = oss.str();
		cout << "\nSending Options Request to IPAddress: " << IPAddress << "\n";
		return sendMessage(message);
	}

	string sendDescribe() {
		ostringstream oss;
		oss << "DESCRIBE rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nUser-Agent: Sunrise Master\r\nAccept: application/sdp\r\n\r\n";
		string message = oss.str();
		cout << "\nSending Describe Request to IPAddress: " << IPAddress;
		return sendMessage(message);
	}

	string sendSetup() {
		ostringstream oss;
		oss << "SETUP rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nTransport: RTP/AVP;unicast;client_port=" << streamProfile.client_port << "-" << streamProfile.client_port + 1 << "\r\n\r\n";
		string message = oss.str();
		cout << "\nSending Setup Request to IPAddress: " << IPAddress;
		return sendMessage(message);
	}

	string setupSession() {
		string resp = sendSetup();
		streamProfile.sessionID = getParamFromResponseBuffer(resp, "Session: ", ";");
		streamProfile.pictureAndSequenceParameters = getParamFromResponseBuffer(resp, "sprop-parameter-sets=", "\r\n");
		listener->onRTSPControllerIsSetup(streamProfile);
		return resp;
	}

	void writeStreamParametersToFile(string file_name) {
		ofstream file;
		file.open(file_name);
		if (file.is_open()) {
			file << streamProfile.pictureAndSequenceParameters;
			file.close();
		}
		else {
			cout << "File " << file_name << " could not be openned";
			return;
		}
	}

	string sendPlay() {
		ostringstream oss;
		oss << "PLAY rtsp://" << IPAddress << ":" << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << streamProfile.sessionID << "\r\nRange: npt=0.000-\r\n\r\n";
		string message = oss.str();
		cout << "\nSending Play Request to IPAddress: " << IPAddress;
		return sendMessage(message);
	}

	string sendPause() {
		ostringstream oss;
		oss << "PAUSE rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << streamProfile.sessionID << "\r\n\r\n";
		string message = oss.str();
		cout << "\nSending Pause Request to IPAddress: " << IPAddress;
		return sendMessage(message);
	}

	string sendTeardown() {
		ostringstream oss;
		oss << "TEARDOWN rtsp://" << IPAddress << ":" << port << "/axis-media/media.amp?videocodec=h264&streamprofile=UHDRes RTSP/1.0\r\nCSeq: " << numMessages << "\r\nSession: " << streamProfile.sessionID << "\r\n\r\n";
		string message = oss.str();
		cout << "\nSending Teardown Request to IPAddress: " << IPAddress;
		string buff = sendMessage(message);
		numMessages = 0;
		return buff;
	}

	string getParamFromResponseBuffer(string response, string StartIdentifier, string EndIdentifier) {
		string resp = response;

		size_t start = resp.find(StartIdentifier);

		string paramZone = resp.substr(start + StartIdentifier.length());

		size_t end = paramZone.find(EndIdentifier);
		string param = paramZone.substr(0, end);

		if (param == "") {
			cout << "\nIdentifier string: '" << StartIdentifier.c_str() << "' not found. Param not returned.\n";
			return "";
		}

		return param;
	}
};
