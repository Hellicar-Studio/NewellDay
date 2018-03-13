//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//

#include "ofApp.h"


int main()
{
	ofSetupOpenGL(1920, 1080, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
