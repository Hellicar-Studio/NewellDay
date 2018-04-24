#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class MonitorFourK {
public:
	MonitorFourK() {};
	void setup(int num) {
		gui.setup(Monitor)
	}
private:
	ofxPanel gui;
	ofParameter<float> x, y, width, height;
};
