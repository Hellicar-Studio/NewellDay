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
        gui.setup(name, "settings/" + name + ".xml");
        gui.add(x.set("X", _x, 0, 15360 - 3840));
        gui.add(y.set("Y", _y, -240, 240));
        gui.add(cx.set("Dewarp Center X", 3840/2, 0, 3840));
        gui.add(cy.set("Dewarp Center Y", 2160/2, 0, 2160));
        gui.add(dwAmount.set("Dewarp Amount", 1.0, 0.0, 1.0));
        gui.add(screenPositionX.set("Screen Pos X",0, 0, 15360 - 3840));
//        gui.loadFromFile("settings/" + name + ".xml");
        col = _col;
        viewBuffer = _viewBuffer;
        gui.setPosition(20 + (gui.getWidth() + 10)*_index, 600);
	}
    
    void update() {
        player.update();
        screenPositionX = x;
        screenPositionY = y;
    }

	void draw() {
		ofPushStyle();
		ofNoFill();
		ofSetColor(col);
		ofSetLineWidth(10);
		ofDrawRectangle(x, y, width, height);
        ofFill();
        ofSetColor(255);
        shader.begin();
        shader.setUniform2f("center", ofVec2f(cx, cy));
        shader.setUniform1f("amount", dwAmount);
        shader.setUniform2f("resolution", player.getWidth(), player.getHeight());
        shader.setUniform2f("offset", ofVec2f(x, y));
        shader.setUniformTexture("inputTexture", player.getTexture(), 0);
        ofDrawRectangle(x, y, player.getWidth(), player.getHeight());
        shader.end();
		ofPopStyle();
	}

	void drawOnMonitor() {
		ofPushStyle();
		ofSetColor(255);
        shader.begin();
        shader.setUniform2f("center", ofVec2f(cx, cy));
        shader.setUniform1f("amount", dwAmount);
        shader.setUniform2f("resolution", player.getWidth(), player.getHeight());
        shader.setUniform2f("offset", ofVec2f(x, y));
        shader.setUniformTexture("inputTexture", player.getTexture(), 0);
        ofDrawRectangle(x, y, player.getWidth(), player.getHeight());
        shader.end();
        ofPopStyle();
	}

	void drawGui() {
		gui.draw();
	}
    
    ofVideoPlayer* getVideoPlayer() { return &player; };
    ofShader* getShader() { return &shader; };
    
    bool toggleVideoPause() {
        player.setPaused(!player.isPaused());
    }
    
private:
	ofxPanel gui;
	ofFbo* viewBuffer;
	ofParameter<float> x, y, width, height, cx, cy, dwAmount;
	ofParameter<float> screenPositionX, screenPositionY;
	ofColor col;
    ofVideoPlayer player;
    ofShader shader;
};
