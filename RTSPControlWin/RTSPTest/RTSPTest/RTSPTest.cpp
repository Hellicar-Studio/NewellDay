// RTSPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "RTSPController.cpp"

using namespace std;

int main()
{
	RTSPController controller[4];

	controller[0].setIPAndPort("192.168.0.41", 80, 42);
	controller[1].setIPAndPort("192.168.0.51", 80, 52);
	controller[2].setIPAndPort("192.168.0.61", 80, 62);
	controller[3].setIPAndPort("192.168.0.71", 80, 72);

	for (int i = 0; i < 4; i++) {
		controller[i].initializeWinSock();
		controller[i].connectToServer();
	}

	//for (int i = 0; i < 4; i++) {
	//	controller[i].sendActivateVirtualPort(1);
	//}

	//Sleep(30000);

	for (int i = 0; i < 4; i++) {
		controller[i].sendDeactivateVirtualPort(1);
	}

    return 0;
}

