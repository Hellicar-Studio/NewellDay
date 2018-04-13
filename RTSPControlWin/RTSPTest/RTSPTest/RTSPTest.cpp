// RTSPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "RTSPController.cpp"

using namespace std;

int main()
{
	// Test
	RTSPController rtspController[4];


	rtspController[0].setIPAndPort("192.168.0.41", 554, 42);
	rtspController[1].setIPAndPort("192.168.0.51", 554, 52);
	rtspController[2].setIPAndPort("192.168.0.61", 554, 62);
	rtspController[3].setIPAndPort("192.168.0.71", 554, 72);

	for (int i = 0; i < 4; i++) {
		rtspController[i].initializeWinSock();
		rtspController[i].connectToServer();
	}

	for (int i = 0; i < 4; i++) {
		rtspController[i].sendOptions();
		rtspController[i].sendDescribe();
		rtspController[i].setupSession();
		rtspController[i].sendPlay();
	}

	printf("\nStreams Started, press any key to stop them.\n");
	system("pause");

	for (int i = 0; i < 4; i++) {
		rtspController[i].sendPause();
		rtspController[i].sendTeardown();
	}

	printf("\nStreams Destroyed, press any key to ask for options again.\n");
	system("pause");

	for (int i = 0; i < 4; i++) {
		rtspController[i].sendOptions();
	}

	//for (int i = 0; i < 4; i++) {
	//	controller[i].sendActivateVirtualPort(1);
	//}

	//for (int i = 0; i < 4; i++) {
	//	controller[i].sendDeactivateVirtualPort(1);
	//}

    return 0;
}

