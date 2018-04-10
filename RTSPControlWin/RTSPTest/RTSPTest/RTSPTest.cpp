// RTSPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "RTSPController.cpp"

using namespace std;

int main()
{
	RTSPController controller24, controller34;

	controller24.initializeWinSock();

	controller24.setIPAndPort("192.168.100.24", 554, 100);

	controller34.initializeWinSock();

	controller34.setIPAndPort("192.168.100.34", 554, 200);

	controller24.connectToServer();

	controller24.sendOptions();

	controller24.sendDescribe();

	controller24.setupSession();

	controller24.sendPlay();

	controller24.sendPause();

	controller24.sendTeardown();

	controller34.connectToServer();

	controller34.sendOptions();

	controller34.sendDescribe();

	controller34.setupSession();

	controller34.sendPlay();

	controller34.sendPause();

	controller34.sendTeardown();

    return 0;
}

