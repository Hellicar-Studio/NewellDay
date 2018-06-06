#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxNestedFileLoader.h"
#include "ViewRegion.h"

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
    
        void onImageChanged(int& e);
    
        ofImage fullImage;
    
        vector<string> imagePaths;
    
        ofFbo fullImageBuffer;
    
        vector<ViewRegion> viewRegions;
    
        ofxPanel gui;
        ofParameter<int> imageIndex;
        ofParameter<int> viewRegionIndex;
        ofParameter<int> yOffset;
};
