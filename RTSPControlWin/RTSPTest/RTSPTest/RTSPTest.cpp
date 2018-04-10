// RTSPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "RTSPController.cpp"

using namespace std;


int main()
{
	RTSPController controller;

	controller.initializeWinSock();

	controller.createSocket();

	controller.setServerIPAndPort("192.168.100.24", 554, 3000);

	controller.connectToRemoteServer();

    return 0;
}

