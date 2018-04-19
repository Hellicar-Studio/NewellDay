// RTSPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>

#include "RTSPController.cpp"
#include "SunriseFinder.cpp"

#define START 3
#define STOP 4

using namespace std;

int main(int argc, char* argv[])
{
	// Test 123
	SunriseFinder sunriseFinder;
	sunriseFinder.setTime();
	sunriseFinder.initializeWinSock();
	sunriseFinder.setIPAndPort("https://api.sunrise-sunset.org/api", 80);
	cout << sunriseFinder.IPAddress << endl;
	sunriseFinder.connectToServer();
	sunriseFinder.sendGetSunrise();
	RTSPController rtspController[4]; // These cameras listen for RTSP by default on port 554 and for HTTP by default on port 80
	RTSPControllerListener rtspControllerListener[4];
	int duration = 60;

	for (int i = 0; i < argc; i++) {
		string dur = argv[i];
		stringstream convert(dur);
		if (!(convert >> duration)) {
			duration = 60;
			cout << "\nSetting default recording duration of 60 seconds";
		}
		else {
			cout << "\nRecording Duration is: " << duration;
		}
	}


	rtspController[0].setIPAndPort("192.168.0.41", 554, 1042);
	rtspController[1].setIPAndPort("192.168.0.51", 554, 1052);
	rtspController[2].setIPAndPort("192.168.0.61", 554, 1062);
	rtspController[3].setIPAndPort("192.168.0.71", 554, 1072);

	for (int i = START; i < STOP; i++) {
		rtspController[i].addListener(&rtspControllerListener[i]);
	}

	for (int i = START; i < STOP; i++) {
		rtspController[i].initializeWinSock();
		rtspController[i].connectToServer();
	}

	for (int i = START; i < STOP; i++) {
		rtspController[i].sendOptions();
		rtspController[i].sendDescribe();
		string s = rtspController[i].setupSession();
		cout << s << endl;
		ostringstream oss;
		oss << "Stream" << i << "Parameters.txt";
		rtspController[i].writeStreamParametersToFile(oss.str());
		rtspController[i].sendPlay();
		_sleep(2000);
	}

	int count = 0;

	while (count < duration) {
		for (int i = START; i < STOP; i++) {
			rtspController[i].sendOptions();
		}
		cout << "\nSeconds: "<< count <<"\n";
		count += 30;
		_sleep(30000);
	}

	for (int i = START; i < STOP; i++) {
		rtspController[i].sendPause();
		rtspController[i].sendTeardown();
	}

    return 0;
}

