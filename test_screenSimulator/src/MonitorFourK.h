#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ViewRegion.h"

class MonitorFourK {
public:
	MonitorFourK() {
		width = 3840;
		height = 2160;
		viewRegions.resize(3);
	};
	void setup(int _x, int _y, int _index) {
		x = _x;
		y = _y;
        cout<<y<<endl;
        if(_index%2) {
            viewRegions[0].setup(0, y, 1080, 1920, _x, 0);
            viewRegions[1].setup(1080, y, 1080, 1920, _x, 1);
            viewRegions[2].setup(2160, y, 1080, 1920, _x, 2);
        } else {
            viewRegions[0].setup(600, y, 1080, 1920, _x, 0);
            viewRegions[1].setup(1680, y, 1080, 1920, _x, 1);
            viewRegions[2].setup(2760, y, 1080, 1920, _x, 2);
        }

        string name = "Monitor" + ofToString(_index);
		gui.setup(name, "settings/" + name + ".xml");
		gui.add(viewRegions[0].gui);
		gui.add(viewRegions[1].gui);
		gui.add(viewRegions[2].gui);
//        gui.loadFromFile("settings/" + name + ".xml");

		gui.setPosition(20 + (gui.getWidth() + 10) * _index, 680);
	}
	void draw() {
		ofPushStyle();
		ofNoFill();
		ofSetColor(0);
		ofSetLineWidth(10);
		ofDrawRectangle(x, y, width, height);
		for (int i = 0; i < viewRegions.size(); i++) {
			viewRegions[i].draw();
		}
		ofPopStyle();

	}
	void drawGui() {
		gui.draw();
	}

	ViewRegion* getViewRegion(int i) {
		return &(viewRegions[i]);
	}
private:
	ofxPanel gui;
	ofParameter<float> x, y, width, height;
	vector<ViewRegion> viewRegions;
};
