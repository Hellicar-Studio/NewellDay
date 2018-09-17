#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    player1.load("Videos/20181508bkln1.mp4");
    player2.load("Videos/20181508bkln2.mp4");

    string settingsPath1 = "Settings/settingsCam1.xml";
    string settingsPath2 = "Settings/settingsCam2.xml";

    scale.resize(2);
    l1.resize(2);
    l2.resize(2);
    rot.resize(2);
    amount.resize(2);
    center.resize(2);
    offset.resize(2);
    
    cam1Gui.setup("P1", settingsPath1);
    cam1Gui.add(scale[0].set("Scale", 1.0, 0.0, 1.0));
    cam1Gui.add(l1[0].set("L1", 0.0, 0.0, player1.getWidth()));
    cam1Gui.add(l2[0].set("L2", 0.0, 0.0, player1.getWidth()));
    cam1Gui.add(rot[0].set("Rot", 0.0, 0.0, 360));
    cam1Gui.add(amount[0].set("Amount", 1.0, 0.0, 1.0));
    cam1Gui.add(center[0].set("Center", ofVec2f(player1.getWidth()/2, player1.getHeight()/2), ofVec2f(0, 0), ofVec2f(player1.getWidth(), player1.getHeight())));
    cam1Gui.add(offset[0].set("Offset", ofVec2f(0.0), ofVec2f(0.0), ofVec2f(player1.getWidth(), player1.getHeight())));
    cam1Gui.loadFromFile(settingsPath1);

    cam2Gui.setup("P2", settingsPath2);
    cam2Gui.add(scale[1].set("Scale", 1.0, 0.0, 1.0));
    cam2Gui.add(l1[1].set("L1", 0.0, 0.0, player2.getWidth()));
    cam2Gui.add(l2[1].set("L2", 0.0, 0.0, player2.getWidth()));
    cam2Gui.add(rot[1].set("Rot", 0.0, 0.0, 360));
    cam2Gui.add(amount[1].set("Amount", 1.0, 0.0, 1.0));
    cam2Gui.add(center[1].set("Center", ofVec2f(player2.getWidth()/2, player2.getHeight()/2), ofVec2f(0, 0), ofVec2f(player2.getWidth(), player1.getHeight())));
    cam2Gui.add(offset[1].set("Offset", ofVec2f(0.0), ofVec2f(0.0), ofVec2f(player2.getWidth(), player2.getHeight())));

    cam2Gui.loadFromFile(settingsPath2);
    
    cam2Gui.setPosition(cam1Gui.getPosition().x + cam1Gui.getWidth() + 10, cam1Gui.getPosition().y);
    
    currentPlayer = &player1;
    
    shader.load("Shaders/fisheye");
    
    player1.play();
    player2.play();
    
    buffers[0].allocate(player1.getWidth(), player1.getHeight());
    buffers[1].allocate(player2.getWidth(), player2.getHeight());

}

//--------------------------------------------------------------
void ofApp::update(){
    player1.update();
    player2.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    drawToBuffer(&player1, &buffers[0], &shader, 0);
    drawToBuffer(&player2, &buffers[1], &shader, 1);
    
    drawBuffer(&buffers[0], 0);
    drawBuffer(&buffers[1], 1);

    cam1Gui.draw();
    cam2Gui.draw();
}

void ofApp::drawToBuffer(ofVideoPlayer* player, ofFbo* buffer, ofShader* shader, int uniformsIndex) {
    buffer->begin();
    shader->begin();
    shader->setUniformTexture("inputTexture", player->getTexture(), 0);
    shader->setUniform2f("resolution", player->getWidth(), player->getHeight());
    shader->setUniform1f("amount", amount[uniformsIndex]);
    shader->setUniform2f("center", center[uniformsIndex].get().x, center[uniformsIndex].get().y);
    ofSetColor(255);
    ofDrawRectangle(0, 0, player->getWidth(), player->getHeight());
    shader->end();
    buffer->end();
}

void ofApp::drawBuffer(ofFbo* buffer, int index) {
    ofPushMatrix();
    ofScale(scale[index], scale[index]);
    ofTranslate(offset[index].get().x, offset[index].get().y);
    ofPushMatrix();
    ofTranslate(buffer->getWidth()/2, buffer->getHeight()/2);
    ofRotate(rot[index], 0, 0, 1);
    ofTranslate(-buffer->getWidth()/2, -buffer->getHeight()/2);
    ofSetColor(255);
    buffer->draw(0, 0, buffer->getWidth(), buffer->getHeight());
    ofPopMatrix();
    ofSetColor(255, 255, 0);
    ofDrawLine(l1[index], 0, l1[index], currentPlayer->getHeight());
    ofDrawLine(l2[index], 0, l2[index], currentPlayer->getHeight());
    ofSetColor(0, 255, 0);
    ofDrawLine(0, buffer->getHeight()/2, buffer->getWidth(), buffer->getHeight()/2);
    ofSetColor(255, 0, 0);
    ofSetLineWidth(3);
    ofDrawLine(center[index].get().x, center[index].get().y + 10, center[index].get().x, center[index].get().y - 10);
    ofDrawLine(center[index].get().x + 10, center[index].get().y, center[index].get().x - 10, center[index].get().y);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '1') {
        currentPlayer = &player1;
    }
    if(key == '2') {
        currentPlayer = &player2;
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
//    if(button == OF_MOUSE_BUTTON_1) {
//        x = ofMap(x, 0, offset[0].get().x + player1.getWidth()*scale[0], 0, player1.getWidth());
//        y = ofMap(y, 0, offset[0].get().y + player1.getHeight()*scale[0], 0, player1.getHeight());
//
//        center[0] = ofVec2f(x, y);
//    }
//    if(button == OF_MOUSE_BUTTON_2) {
//        x = ofMap(x, 0, offset[1].get().x + player2.getWidth() * scale[1], 0, player2.getWidth());
//        y = ofMap(y, 0, offset[1].get().y + player2.getHeight() * scale[1], 0, player2.getHeight());
//
//        center[0] = ofVec2f(x, y);
//    }
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
