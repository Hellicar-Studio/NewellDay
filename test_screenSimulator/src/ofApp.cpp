#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	img.load("images/landscape.jpg");
	img.resize(15360, 2160);

	viewSpace.allocate(15360, 2160);
	wallSpace.allocate(12960, 1920);
	monitorSpace.allocate(15360, 2160);

	cams.resize(4);
	cams[0].setup(0, 0, ofColor(255, 0, 0), &viewSpace, 0);
	cams[1].setup(0, 0, ofColor(0, 255, 0), &viewSpace, 1);
	cams[2].setup(0, 0, ofColor(0, 0, 255), &viewSpace, 2);
	cams[3].setup(0, 0, ofColor(255, 255, 0), &viewSpace, 3);

	fourKMonitors.resize(4);
	int x = 0;
	int y = 0;
	for (int i = 0; i < fourKMonitors.size(); i++) {
		fourKMonitors[i].setup(x, y, i);
		x += 3840;
	}
    
	outputMonitors.resize(12);
	x = 0;
	y = 0;
	for (int i = 0; i < outputMonitors.size(); i++) {
		outputMonitors[i].setup(x, y, fourKMonitors[i/3].getViewRegion(i%3), &monitorSpace);
		x += 1080;
	}
    
    ipcams = ofx::Video::IpVideoGrabberSettings::fromFile("streamsPublic.json");
    
    reloadCameras();
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto& grabber: grabbers)
    {
        grabber->update();
    }
}

//--------------------------------------------------------------
ofx::Video::IpVideoGrabberSettings& ofApp::getNextCamera()
{
    nextCamera = (nextCamera + 1) % ipcams.size();
    return ipcams[nextCamera];
}


//--------------------------------------------------------------
void ofApp::reloadCameras()
{
    // Clear the grabbers.
    grabbers.clear();
    
    // Initialize new grabbers.
    for (std::size_t i = 0; i < numCameras; ++i)
    {
        auto c = std::make_shared<ofx::Video::IPVideoGrabber>();
        auto& settings = getNextCamera();
        c->setUsername(settings.getUsername());
        c->setPassword(settings.getPassword());
        c->setURI(settings.getURL());
        c->connect();
        
        grabbers.push_back(c);;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	viewSpace.begin();
	ofBackground(127);
    grabbers[0]->draw(0, 0, 3840, 2160);
    grabbers[1]->draw(3840, 0, 3840, 2160);
    grabbers[0]->draw(3840*2, 0, 3840, 2160);
    grabbers[1]->draw(3840*3, 0, 3840, 2160);
	for (int i = 0; i < 4; i++) {
		cams[i].draw();
	}
	viewSpace.end();

	monitorSpace.begin();
	ofBackground(127);
	for (int i = 0; i < cams.size(); i++) {
		cams[i].drawOnMonitor();
	}
	for (int i = 0; i < fourKMonitors.size(); i++) {
		fourKMonitors[i].draw();
	}
	monitorSpace.end();

	wallSpace.begin();
	ofBackground(127);
	for (int i = 0; i < outputMonitors.size(); i++) {
		outputMonitors[i].draw();
	}
	wallSpace.end();
    
    float scale = ofGetWidth() / viewSpace.getWidth();

	viewSpace.draw(0, ofGetHeight() - viewSpace.getHeight() * scale, viewSpace.getWidth() *scale, viewSpace.getHeight() *scale);
	monitorSpace.draw(0, ofGetHeight() - viewSpace.getHeight() * scale - monitorSpace.getHeight() * scale, monitorSpace.getWidth() *scale , monitorSpace.getHeight() *scale);
	wallSpace.draw(0, 0, wallSpace.getWidth() * scale, wallSpace.getHeight() * scale );

	for (int i = 0; i < 4; i++) {
		cams[i].drawGui();
		fourKMonitors[i].drawGui();
	}

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
