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
    gui.add(blend.set("Blend", 0.0, 0.0, 1.0));
    gui.add(time.set("Time", 0, 0, 300));
    
    buffer.allocate(1920*2, 1080);
    
    blendShader.load("shaders/blend");
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
    blendShader.setUniformTexture("stream1", grabbers[0]->getTexture(), 0);
    blendShader.setUniformTexture("stream2", grabbers[1]->getTexture(), 1);
    blendShader.setUniform2f("resolution", 1920, 1080);
    blendShader.setUniform1f("blend", blend);
    ofDrawRectangle(0, 0, 1920*2, 1080);
    blendShader.end();
    buffer.end();
    
    buffer.draw(0, 0, 1920, 1080/2);
    
    gui.draw();
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
}
