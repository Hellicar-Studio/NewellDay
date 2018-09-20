#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	img.load("images/landscape.jpg");
	img.resize(15360, 2160);

	viewSpace.allocate(15360, 2160);
	wallSpace.allocate(12960, 1920);
	monitorSpace.allocate(15360, 2160);

	cams.resize(4);
	cams[0].setup(152.88, -180, ofColor(255, 0, 0), &viewSpace, 0);
	cams[1].setup(3557, -240, ofColor(0, 255, 0), &viewSpace, 1);
	cams[2].setup(8090.16, -10.51, ofColor(0, 0, 255), &viewSpace, 2);
	cams[3].setup(11300, -30, ofColor(255, 255, 0), &viewSpace, 3);
    cams[0].getVideoPlayer()->load("videos/20181508bkln0.mp4");
    cams[1].getVideoPlayer()->load("videos/20181508bkln1.mp4");
    cams[2].getVideoPlayer()->load("videos/20181508bkln2.mp4");
    cams[3].getVideoPlayer()->load("videos/20181508bkln3.mp4");
    
    for(int i = 0; i < cams.size(); i++) {
        cams[i].getShader()->load("shaders/dewarp");
    }

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
    
//    for(int i = 0; i < videoPlayers.size(); i++) {
//        videoPlayers[i].load("videos/example.mp4");
//    }
    
//    ipcams = ofx::Video::IpVideoGrabberSettings::fromFile("streamsPublic.json");
    
//    reloadCameras();
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < cams.size(); i++) {
        cams[i].update();
    }
//    for (auto player: videoPlayers)
//    {
//        player.update();
//    }
}

//--------------------------------------------------------------
//ofx::Video::IpVideoGrabberSettings& ofApp::getNextCamera()
//{
//    nextCamera = (nextCamera + 1) % ipcams.size();
//    return ipcams[nextCamera];
//}


//--------------------------------------------------------------
//void ofApp::reloadCameras()
//{
//    // Clear the grabbers.
//    grabbers.clear();
//
//    // Initialize new grabbers.
//    for (std::size_t i = 0; i < numCameras; ++i)
//    {
//        auto c = std::make_shared<ofx::Video::IPVideoGrabber>();
//        auto& settings = getNextCamera();
//        c->setUsername(settings.getUsername());
//        c->setPassword(settings.getPassword());
//        c->setURI(settings.getURL());
//        c->connect();
//
//        grabbers.push_back(c);;
//    }
//}

//--------------------------------------------------------------
void ofApp::draw(){
	viewSpace.begin();
	ofBackground(127);
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
//        fourKMonitors[i].drawGui();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'p') {
        for(int i = 0; i < cams.size(); i++) {
            cams[i].toggleVideoPause();
        }
    }
    if(key == 's') {
        ofImage img;
        img.allocate(wallSpace.getWidth(), wallSpace.getHeight(), OF_IMAGE_COLOR);
        wallSpace.readToPixels(img);
        img.save("Output" + ofToString(ofGetTimestampString()) + ".png");
    }
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
