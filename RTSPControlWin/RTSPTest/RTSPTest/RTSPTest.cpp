// RTSPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "RTSPController.cpp"

using namespace std;

int main()
{
	// Test 123
	RTSPController rtspController[4]; // These cameras listen for RTSP by default on port 554 and for HTTP by default on port 80
	RTSPControllerListener rtspControllerListener[4];

	rtspController[0].setIPAndPort("192.168.0.41", 554, 1042);
	rtspController[0].addListener(&rtspControllerListener[0]);
	rtspController[1].setIPAndPort("192.168.0.51", 554, 52);
	rtspController[2].setIPAndPort("192.168.0.61", 554, 62);
	rtspController[3].setIPAndPort("192.168.0.71", 554, 72);

	for (int i = 0; i < 1; i++) {
		rtspController[i].initializeWinSock();
		rtspController[i].connectToServer();
	}

	for (int i = 0; i < 1; i++) {
		rtspController[i].sendOptions();
		rtspController[i].sendDescribe();
		rtspController[i].setupSession();
		rtspController[i].sendPlay();
	}


	printf("\nStreams Started, press any key to ask for options again.\n");
	system("pause");

	for (int i = 0; i < 1; i++) {
		rtspController[i].sendOptions();
	}

	printf("\nStreams Alive, press any key to stop them.\n");
	system("pause");

	for (int i = 0; i < 1; i++) {
		rtspController[i].sendPause();
		rtspController[i].sendTeardown();
	}


	//for (int i = 0; i < 4; i++) {
	//	controller[i].sendActivateVirtualPort(1);
	//}

	//for (int i = 0; i < 4; i++) {
	//	controller[i].sendDeactivateVirtualPort(1);
	//}

    return 0;
}

