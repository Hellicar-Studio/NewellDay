#include "RTSPController.cpp"

int main(int argc, char const  *argv[]) {

	RTSPController rtspConnection;

	rtspConnection.setIPAndPort("184.72.239.149", 80, 90);

	rtspConnection.connectToServer();

	rtspConnection.sendOptions();

	rtspConnection.sendDescribe();

	rtspConnection.setupSession();

	rtspConnection.sendPlay();

	rtspConnection.sendPause();

	rtspConnection.sendTeardown();

	printf("\n");

	return 0;
}