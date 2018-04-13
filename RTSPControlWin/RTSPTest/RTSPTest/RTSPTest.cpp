// RTSPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "RTSPController.cpp"

using namespace std;

int main()
{
	// Test
	RTSPController controller[4];

	controller[0].setIPAndPort("192.168.0.41", 554, 42);
	controller[1].setIPAndPort("192.168.0.51", 554, 52);
	controller[2].setIPAndPort("192.168.0.61", 554, 62);
	controller[3].setIPAndPort("192.168.0.71", 554, 72);

	for (int i = 0; i < 4; i++) {
		controller[i].initializeWinSock();
		controller[i].connectToServer();
	}

	for (int i = 0; i < 4; i++) {
		controller[i].sendOptions();
		controller[i].sendDescribe();
		controller[i].setupSession();
		controller[i].sendPlay();
	}

	printf("\nStreams Started, press any key to stop them.\n");
	system("pause");

	for (int i = 0; i < 4; i++) {
		controller[i].sendPause();
		controller[i].sendTeardown();
	}

	//for (int i = 0; i < 4; i++) {
	//	controller[i].sendActivateVirtualPort(1);
	//}

	//for (int i = 0; i < 4; i++) {
	//	controller[i].sendDeactivateVirtualPort(1);
	//}

    return 0;
}

