//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//

#pragma once

#include "ofMain.h"
#include "IPVideoGrabber.h"
#include "ofxGui.h"
#include "ofxNestedFileLoader.h"

class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
    void toggleRecording();

    std::vector<std::shared_ptr<ofx::Video::IPVideoGrabber>> grabbers;

    void reloadCameras();
    ofx::Video::IpVideoGrabberSettings& getNextCamera();
    std::vector<ofx::Video::IpVideoGrabberSettings> ipcams; // a list of IPCameras
    std::size_t nextCamera = 0;

    // This message occurs when the incoming video stream image size changes. 
    // This can happen if the IPCamera has a single broadcast state (some cheaper IPCams do this)
    // and that broadcast size is changed by another user. 
    void videoResized(const void* sender, ofResizeEventArgs& arg);

    std::size_t numCameras = 2;
    std::size_t numCameraRows = 1;
    std::size_t numCameraColumns = 2;
    
    ofxPanel gui;
    ofParameter<bool> recording;
    ofParameter<bool> live;
    ofParameter<float> blend;
    ofParameter<float> yOffset;
    ofParameter<int> time;
    ofParameter<int> frameHeight;
    
    
    ofImage img1, img2;
    
    ofImage screen;
    
    int timeRecStarted;
    
    ofFbo buffer;
    
    ofShader blendShader;
    
    vector<ofVideoPlayer> videos;
    
    string camIP1 = "192.168.100.24";
    string camIP2 = "192.168.100.26";

    
};
