//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Testing redirects and https.
    //std::string url = "https://httpbin.org/get";
    std::string url = "http://192.168.100.24/vapix/services";

    // Create a client.
    ofxHTTP::Client client;

    // Create a request.
    ofxHTTP::GetRequest request(url);
    
    request.setContentType("application/soap+xml; action=http://www.axis.com/vapix/ws/event1/GetEventInstances; Charset=UTF-8");
    
    ofxHTTP::PostRequest postrequest(url);
    
    postrequest.setContentType("application/soap+xml; action=http://www.axis.com/vapix/ws/event1/GetEventInstances; Charset=UTF-8");
    
    postrequest.add
    
    // Create a context.
    ofxHTTP::Context context;

    try
    {
        // Execute the request within the given context.
        auto response = client.execute(context, request);

        // Check the response.
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            // A successful response.
            ofLogNotice("ofApp::setup") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";

            // Buffer the response, or otherwise consume the stream.
            ofBuffer buffer(response->stream());

            ofLogNotice("ofApp::setup") << "Content Begin";

            std::cout << buffer << std::endl;

            ofLogNotice("ofApp::setup") << "Content End";
        }
        else
        {
            ofLogError("ofApp::setup") << response->getStatus() << " " << response->getReason();
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << exc.displayText();
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::setup") << exc.what();
    }
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
}
