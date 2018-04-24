#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ViewRegion {
public:
	ViewRegion() {
	}

	void setup(int _x, int _y, int _width, int _height, int _xMin, int _index) {
		gui.setName("View Region: " + ofToString(_index));
		gui.add(x.set("X", _x + _xMin, _xMin, _xMin + 3840 - _width));
		gui.add(y.set("Y", _y, 0, 2160 - _height));
		width = _width;
		height = _height;
	}
	void draw() {
		ofPushStyle();
		ofSetColor(255, 255, 0);
		ofNoFill();
		ofSetLineWidth(5);
		ofDrawRectangle(x, y, width, height);
		ofPopStyle();
	}
	ofParameterGroup gui;
	ofParameter<int> x, y, width, height;
private:
};