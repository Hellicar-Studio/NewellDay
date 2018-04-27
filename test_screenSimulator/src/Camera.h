#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class Camera {
public:
	Camera() {
		x = 0;
		y = 0;
		width = 3840;
		height = 2160;
		screenPositionY = 0;
	}
	void setup(int _x, int _y, ofColor _col, ofFbo* _viewBuffer, int _index) {
        string name = "CamControl" + ofToString(_index);
		gui.setup(name, name + ".xml");
		gui.add(x.set("X", _x, 0, 15360 - 3840));
		gui.add(y.set("Y", _y, 0, 2160));
		gui.add(screenPositionX.set("Screen Pos X",0, 0, 15360 - 3840));
        gui.loadFromFile("settings/" + name + ".xml");
		col = _col;
		viewBuffer = _viewBuffer; 
		gui.setPosition(20 + (gui.getWidth() + 10)*_index, 600);
	}

	void draw() {
		ofPushStyle();
		ofNoFill();
		ofSetColor(col);
		ofSetLineWidth(10);
		ofDrawRectangle(x, y, width, height);
		ofPopStyle();
	}

	void drawOnMonitor() {
		ofPushStyle();
		ofSetColor(255);
		viewBuffer->getTexture().drawSubsection(screenPositionX, screenPositionY, width, height, x, y);
		ofPopStyle();
	}

	void drawGui() {
		gui.draw();
	}
private:
	ofxPanel gui;
	ofFbo* viewBuffer;
	ofParameter<float> x, y, width, height;
	ofParameter<float> screenPositionX, screenPositionY;
	ofColor col;
};
