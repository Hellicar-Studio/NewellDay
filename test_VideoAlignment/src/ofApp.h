#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void drawToBuffer(ofVideoPlayer* player, ofFbo* buffer, ofShader* shader, int uniformsIndex);
        void drawBuffer(ofFbo* buffer, int index);

        ofVideoPlayer player1;
        ofVideoPlayer player2;
    
        ofVideoPlayer* currentPlayer;
    
        ofShader shader;
    
        ofFbo buffers[2];

        ofxPanel cam1Gui;
        ofxPanel cam2Gui;
        vector< ofParameter<float> > scale;
        vector< ofParameter<float> > l1;
        vector< ofParameter<float> > l2;
        vector< ofParameter<float> > rot;
        vector< ofParameter<float> > amount;
        vector< ofParameter<ofVec2f> > center;
        vector< ofParameter<ofVec2f> > offset;
};
