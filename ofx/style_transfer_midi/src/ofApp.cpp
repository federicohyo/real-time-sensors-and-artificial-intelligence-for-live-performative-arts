/*
 * Copyright (c) 2013-2023 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#include "ofApp.h"
#include <signal.h>


//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	//ofBackground(255, 255, 255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetWindowTitle("midi_style_transfer");

	// Midi setup 
	midiIn.listInPorts();
	midiIn.openPort(1);
	// don't ignore sysex, timing, & active sense messages,
	midiIn.ignoreTypes(false, true, false);
	// add ofApp as a listener and enable direct message handling
	midiIn.addListener(this);
	// print received messages to the console
	midiIn.setVerbose(true);
	ofLog(OF_LOG_NOTICE, "##################################################"); 

	
	// Tensorflow and camera setup
	
	// use only a portion of the GPU memory & grow as needed
	if(!ofxTF2::setGPUMaxMemory(ofxTF2::GPU_PERCENT_70, true)) {
		ofLogError() << "failed to set GPU Memory options!";
	}

	// go through the models directory and print out all the paths
	ofDirectory modelsDir(ofToDataPath("models"));
	modelsDir.listDir();
	for(int i = 0; i < modelsDir.size(); i++) {
		ofDirectory sub(modelsDir.getPath(i));
		if(sub.isDirectory()) {
			auto absSubPath = sub.getAbsolutePath();
   			ofLogNotice() << "Found model: " << absSubPath;
			modelPaths.push_back(absSubPath);
		}
	}

	// load first model, bail out on error
	if(!model.load(modelPaths[modelIndex])) {
		std::exit(EXIT_FAILURE);
	}
	modelName = ofFilePath::getBaseName(modelPaths[modelIndex]);
	
	
	// setup video grabber
	vidIn.setDesiredFrameRate(30);
	vidIn.setup(camWidth, camHeight);

	// allocate output image
	imgOut.allocate(nnWidth, nnHeight, OF_IMAGE_COLOR);

	// start the model!
	model.startThread();

	
}

//--------------------------------------------------------------
void ofApp::update() {
	
	// Midi 
	/// queued message handling
	/// queued message handling
	if(midiIn.hasWaitingMessages()) {
		ofxMidiMessage message;

		// add the latest message to the message queue
		while(midiIn.getNextMessage(message)) {
			midiMessages.push_back(message);
		}

		// remove any old messages if we have too many
		while(midiMessages.size() > maxMessages) {
			midiMessages.erase(midiMessages.begin());
		}
	}
	
	vidIn.update();
	
	if(vidIn.isFrameNew()) {
	
		ofPixels &pixels = vidIn.getPixels();
		ofPixels resizedPixels(pixels);
		resizedPixels.resize(nnWidth, nnHeight);
		input = ofxTF2::pixelsToTensor(resizedPixels);
		newInput = true;
	}
	
	// thread-safe conditional input update
	if(newInput && model.readyForInput()) {
		model.update(input);
		newInput = false;
	}

	// thread-safe conditional output update
	if(model.isOutputNew()) {
		auto output = model.getOutput();
		ofxTF2::tensorToImage(output, imgOut);
		imgOut.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255); // Set color to white to draw the image in its original colors

    // Draw the processed image
    if(imgOut.isAllocated()) { // Check if imgOut has been allocated before attempting to draw
        imgOut.draw(0, 0, ofGetWidth(), ofGetHeight()); // Adjust as necessary to maintain aspect ratio
    }

    // Draw model information and controls
    float diff = (ofGetElapsedTimef() - loadTimestamp);
    std::string text = "Model: " + modelName; // Display the current model name
    // Optionally, display a countdown or indication for automatic model loading
    if(autoLoad) {
        text += "\nLoading new model in ";
        text += std::to_string((int)(loadTimeSeconds - diff) + 1) + " seconds";
    }
    // Display FPS and controls info
    text += "\n" + ofToString((int)ofGetFrameRate()) + " fps";
    text += "\nPress 'a' to toggle auto load (if implemented)";
    ofDrawBitmapStringHighlight(text, 4, 12); // Modify position as needed

    // Optionally, draw MIDI message information or other interactive elements
    // This could include current MIDI messages, status, or instructions for users
}


//--------------------------------------------------------------
void ofApp::exit() {

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
/// direct message handling
/// note: this is called on the MIDI thread, so copy message contents
void ofApp::newMidiMessage(ofxMidiMessage &message) {
    // Log every new MIDI message for debugging
    ofLogNotice() << "Received MIDI message: " << message.toString();
    
    // add the latest message to the message queue
    midiMessages.push_back(message);

    // remove any old messages if we have too many
    while(midiMessages.size() > maxMessages) {
        midiMessages.erase(midiMessages.begin());
    }

    // Check for MIDI_NOTE_ON message and change model
    if(message.status == MIDI_NOTE_ON) {
    
    	// Create a mapping between pitch and model
		if (message.pitch == 108){modelIndex=0;}
		else if (message.pitch == 110){modelIndex=1;}
		else if (message.pitch == 111){modelIndex=2;}
		else if (message.pitch == 113){modelIndex=3;}
		else if (message.pitch == 115){modelIndex=4;}
		else if (message.pitch == 116){modelIndex=5;}
		else if (message.pitch == 118){modelIndex=6;}
		else if (message.pitch == 120){modelIndex=7;}
		else if (message.pitch == 96){modelIndex=8;}
		else if (message.pitch == 98){modelIndex=9;}
		else if (message.pitch == 99){modelIndex=0;}
        
		//modelIndex = (modelIndex + 1) % modelPaths.size(); // Cycle through models

        ofLogNotice() << "Switching to model: " << modelPaths[modelIndex];
        bool loadSuccess = model.load(modelPaths[modelIndex]); // Synchronously load for simplicity
        if(loadSuccess) {
            ofLogNotice() << "Model loaded successfully: " << modelPaths[modelIndex];
            modelName = ofFilePath::getBaseName(modelPaths[modelIndex]);
            loadTimestamp = ofGetElapsedTimef(); // Reset timestamp for logging
            newInput = true; // Flag to process new input with the model
        } else {
            ofLogError() << "Failed to load model: " << modelPaths[modelIndex];
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch(key) {
		case '?':
			midiIn.listInPorts();
			break;
	}
}

void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

