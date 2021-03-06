//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//

#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(30);

    ipcams = ofx::Video::IpVideoGrabberSettings::fromFile("streams.json");

    ofLogNotice("ofApp::setup()") << "Loaded " << ipcams.size() << " locations.";

    reloadCameras();
    
    gui.setup("GUI", "settings/settings.xml");
    gui.add(recording.set("Recording", false));
    gui.add(live.set("LIVE", true));
    gui.add(blend.set("Blend", 0.0, 0.0, 1.0));
    gui.add(yOffset.set("Right Y", 0, -1.0, 1.0));
    gui.add(frameHeight.set("Frame Height", 0, 0, 1080));
    gui.add(time.set("Time", 0, 0, 300));

    
    buffer.allocate(1920*2, 1080);
    
    blendShader.load("shaders/blend");
    
    ofxNestedFileLoader loader;
    vector<string> videoPaths = loader.load("videos");
    for(int i = 0; i < videoPaths.size(); i++) {
        ofVideoPlayer video;
        video.load(videoPaths[i]);
        videos.push_back(video);
    }
    
//    gui.add(vidIndex1.set("Video Index 1", 0, 0, videoPaths.size()));
//    gui.add(vidIndex2.set("Video Index 2", 0, 0, videoPaths.size()));
    
    img1.load("Good1.png");
    img2.load("Good2.png");
    
    img1.resize(1920, 1080);
    img2.resize(1920, 1080);
}


ofx::Video::IpVideoGrabberSettings& ofApp::getNextCamera()
{
    nextCamera = (nextCamera + 1) % ipcams.size();
    return ipcams[nextCamera];
}


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

void ofApp::videoResized(const void* sender, ofResizeEventArgs& arg)
{
    // Find the camera that sent the resize event changed.
    for (std::size_t i = 0; i < numCameras; ++i)
    {
        if (sender == grabbers[i].get())
        {
            std::stringstream ss;
            ss << "videoResized: ";
            ss << "Camera connected to: " << grabbers[i]->getURI() + " ";
            ss << "New DIM = " << arg.width << "/" << arg.height;
            ofLogVerbose("ofApp") << ss.str();
        }
    }
}

void ofApp::update()
{
    for (auto& grabber: grabbers)
    {
        grabber->update();
    }
    
    if(recording) {
        time = int(ofGetElapsedTimef()) - timeRecStarted;
        if(time > time.getMax()) {
            toggleRecording();
        }
    }
}

void ofApp::draw()
{
    ofBackground(0,0,0);

    ofSetHexColor(0xffffff);
    
    std::size_t row = 0;
    std::size_t col = 0;
    
    float x = 0;
    float y = 0;
    
    float w = ofGetWidth() / numCameraColumns;
    float h = ofGetHeight() / numCameraRows;
    
    buffer.begin();
    blendShader.begin();
    if(live) {
        blendShader.setUniformTexture("stream1", img1, 0);
        blendShader.setUniformTexture("stream2", img2, 1);
    }
    blendShader.setUniform1f("yOffset", yOffset);
    blendShader.setUniform2f("resolution", 1920, 1080);
    blendShader.setUniform1f("blend", blend);
    ofDrawRectangle(0, 0, 1920*2, 1080);
    blendShader.end();
    buffer.end();
//    grabbers[0]->draw(0, 0, 1920, 1080);
    
    buffer.draw(0, 0, 1920, 1080/2);
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(5);
    ofNoFill();
    int width = 1920/2 + int(ofMap(blend, 0.0, 1.0, 1920/2, 0.0));
    int height = width * 54 / 384.5;
    ofDrawRectangle(0, frameHeight, width, height);
    ofPopStyle();
    
    gui.draw();
    
    ofDrawBitmapStringHighlight("Total Resolution: " + ofToString(3840 + int(ofMap(blend, 0.0, 1.0, 3840, 0.0))), 0, ofGetHeight() - 20);
}

void ofApp::toggleRecording() {
    if(recording) {
        ofLaunchBrowser("http://" + camIP1 + "/axis-cgi/virtualinput/deactivate.cgi?schemaversion=1&port=1");
        ofLaunchBrowser("http://" + camIP2 + "/axis-cgi/virtualinput/deactivate.cgi?schemaversion=1&port=1");
        recording = false;
        time = 0;
        timeRecStarted = 0;
    }
    else {
        ofLaunchBrowser("http://" + camIP1 + "/axis-cgi/virtualinput/activate.cgi?schemaversion=1&port=1");
        ofLaunchBrowser("http://" + camIP2 + "/axis-cgi/virtualinput/activate.cgi?schemaversion=1&port=1");
        recording = true;
        time = 0;
        timeRecStarted = int(ofGetElapsedTimef());
    }
}

void ofApp::keyPressed(int key)
{
    if (key == ' ')
    {
        reloadCameras();
    }
    if(key == 'r') {
        toggleRecording();
    }
    if(key == 'c') {
        int width = 1920/2 + int(ofMap(blend, 0.0, 1.0, 1920/2, 0.0));
        int height = width * 54 / 384.5;
        screen.grabScreen(0, frameHeight, width, height);
        screen.save("output" + ofGetTimestampString() + ".png");
    }
}
