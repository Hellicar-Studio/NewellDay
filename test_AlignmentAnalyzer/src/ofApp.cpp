#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofxNestedFileLoader loader;
    imagePaths = loader.load("images");
    if(imagePaths.size() > 0) {
        fullImage.load(imagePaths[0]);
    }
    
    string settingsPath = "settings/settings.xml";
    gui.setup("Controls", settingsPath);
    gui.add(imageIndex.set("Image Index", 0, 0, imagePaths.size()-1));
    gui.add(viewRegionIndex.set("viewRegionIndex", 0, 0, 10));
    gui.add(yOffset.set("y Offset", 0, -500, 500));
    gui.loadFromFile(settingsPath);
    
    imageIndex.addListener(this, &ofApp::onImageChanged);
    
    fullImageBuffer.allocate(fullImage.getWidth(), fullImage.getHeight());
    
    viewRegions.resize(12);
    
    //viewRegions[0].setup(x, y, width, height, xMin, index);
    
    viewRegions[0].setup(0+600, 120, 1080, 1920, 0, 0);
    viewRegions[1].setup(1080+600, 120, 1080, 1920, 0, 1);
    viewRegions[2].setup(2160+600, 120, 1080, 1920, 0, 2);
    
    viewRegions[3].setup(0, 120, 1080, 1920, 3840, 3);
    viewRegions[4].setup(1080, 120, 1080, 1920, 3840, 4);
    viewRegions[5].setup(2160, 120, 1080, 1920, 3840, 5);
    
    viewRegions[6].setup(0+600, 120, 1080, 1920, 7680, 6);
    viewRegions[7].setup(1080+600, 120, 1080, 1920, 7680, 7);
    viewRegions[8].setup(2160+600, 120, 1080, 1920, 7680, 8);
    
    viewRegions[9].setup(0, 120, 1080, 1920, 11520, 9);
    viewRegions[10].setup(1080, 120, 1080, 1920, 11520, 10);
    viewRegions[11].setup(2160, 120, 1080, 1920, 11520, 11);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    fullImageBuffer.begin();
    fullImage.draw(0, 0);
    for(int i = 0; i < viewRegions.size(); i++) {
        viewRegions[i].draw();
    }
    fullImageBuffer.end();
    
    float scale = ofGetWidth() / fullImage.getWidth();
    
    fullImage.drawSubsection(0, yOffset, 1080, 1920, viewRegions[viewRegionIndex].x, viewRegions[viewRegionIndex].y);
    fullImage.drawSubsection(1080, yOffset, 1080, 1920, viewRegions[viewRegionIndex+1].x, viewRegions[viewRegionIndex+1].y);

//    viewRegions[viewRegionIndex]
    
    ofPushMatrix();
    ofScale(scale, scale);
    fullImageBuffer.draw(0, 0);
    ofPopMatrix();
    
    gui.draw();
    ofDrawBitmapStringHighlight(imagePaths[imageIndex], gui.getPosition().x, gui.getPosition().y+gui.getHeight() + 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == OF_KEY_RIGHT) {
        imageIndex++;
        if(imageIndex > imagePaths.size()-1)
            imageIndex = imagePaths.size()-1;
    }
    if(key == OF_KEY_LEFT) {
        imageIndex--;
        if(imageIndex < 0)
            imageIndex = 0;
    }
}

//--------------------------------------------------------------
void ofApp::onImageChanged(int& e) {
    fullImage.load(imagePaths[e]);
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
