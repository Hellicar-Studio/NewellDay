#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	denoise.load("shaders/denoise");
	img.load("images/brooklyn1.jpg");
	buffer.allocate(img.getWidth(), img.getHeight());
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	buffer.begin();
	ofSetColor(255, 0, 255);
	denoise.begin();
	denoise.setUniformTexture("inputTexture", img, 0);
	denoise.setUniform2f("resolution", img.getWidth(), img.getHeight());
	ofDrawRectangle(0, 0, img.getWidth(), img.getHeight());
	denoise.end();
	buffer.end();

	ofSetColor(255);
	img.draw(0, 0);
	buffer.draw(img.getWidth(), 0);

	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), ofVec2f(20, 20));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
