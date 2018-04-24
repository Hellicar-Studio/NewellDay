#pragma once

#include "ofMain.h"
#include "ViewRegion.h"

class OutputMonitor {
public:
	OutputMonitor() {
		width = 1080;
		height = 1920;
	}
	void setup(int _x, int _y, ViewRegion* _viewRegion, ofFbo* _monitorBuffer) {
		x = _x;
		y = _y;
		viewRegion = _viewRegion;
		monitorBuffer = _monitorBuffer;
	}
	void draw() {
		ofPushStyle();
		ofNoFill();
		ofSetColor(0);
		ofSetLineWidth(5);
		ofDrawRectangle(x, y, width, height);
		ofSetColor(255);
		monitorBuffer->getTexture().drawSubsection(x, y, viewRegion->width, viewRegion->height, viewRegion->x, viewRegion->y);
		ofPopStyle();
	}
	float x, y, width, height;
private:
	ofFbo* monitorBuffer;
	ViewRegion* viewRegion;
};