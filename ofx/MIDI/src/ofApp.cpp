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

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(255, 255, 255);
	ofSetLogLevel(OF_LOG_VERBOSE);

	// print input ports to console
	midiIn.listInPorts();

	// open port by number (you may need to change this)
	midiIn.openPort(1);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);

	// add ofApp as a listener and enable direct message handling
	// comment this to use queued message handling
	midiIn.addListener(this);

	// print received messages to the console
	midiIn.setVerbose(true);
}

//--------------------------------------------------------------
void ofApp::update() {

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
}

//--------------------------------------------------------------
void ofApp::draw() {

	for(unsigned int i = 0; i < midiMessages.size(); ++i) {

		ofxMidiMessage &message = midiMessages[i];
		int x = 10;
		int y = i*40 + 40;

		// draw the last received message contents to the screen,
		// this doesn't print all the data from every status type
		// but you should get the general idea
		stringstream text;
		text << ofxMidiMessage::getStatusString(message.status);
		while(text.str().length() < 16) { // pad status width
			text << " ";
		}

		ofSetColor(127);
		if(message.status < MIDI_SYSEX) {
			text << "chan: " << message.channel;
            if(message.status == MIDI_NOTE_ON || message.status == MIDI_NOTE_OFF) {
                // Print each byte in hex format
                text << " bytes: ";
                for(size_t j = 0; j < message.bytes.size(); ++j) {
                    text << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(message.bytes[j]) << " ";
                }
                
                // Visual representation for pitch and velocity
                text << "\tpitch: " << message.pitch;
                ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
                                ofMap(message.pitch, 0, 127, 0, ofGetWidth()*0.2), 10);
                text << "\tvel: " << message.velocity;
                ofDrawRectangle(x + (ofGetWidth()*0.2 * 2), y + 12,
                                ofMap(message.velocity, 0, 127, 0, ofGetWidth()*0.2), 10);
            }
			else if(message.status == MIDI_CONTROL_CHANGE) {
                
                text << " bytes: ";
                for(size_t j = 0; j < message.bytes.size(); ++j) {
                    text << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(message.bytes[j]) << " ";
                }
                
				text << "\tctl: " << message.control;
				ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
					ofMap(message.control, 0, 127, 0, ofGetWidth()*0.2), 10);
				text << "\tval: " << message.value;
				ofDrawRectangle(x + ofGetWidth()*0.2 * 2, y + 12,
					ofMap(message.value, 0, 127, 0, ofGetWidth()*0.2), 10);
			}
			else if(message.status == MIDI_PROGRAM_CHANGE) {
				text << "\tpgm: " << message.value;
				ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
					ofMap(message.value, 0, 127, 0, ofGetWidth()*0.2), 10);
			}
			else if(message.status == MIDI_PITCH_BEND) {
				text << "\tval: " << message.value;
				ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
					ofMap(message.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()*0.2), 10);
			}
			else if(message.status == MIDI_AFTERTOUCH) {
				text << "\tval: " << message.value;
				ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
					ofMap(message.value, 0, 127, 0, ofGetWidth()*0.2), 10);
			}
			else if(message.status == MIDI_POLY_AFTERTOUCH) {
				text << "\tpitch: " << message.pitch;
				ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
					ofMap(message.pitch, 0, 127, 0, ofGetWidth()*0.2), 10);
				text << "\tval: " << message.value;
				ofDrawRectangle(x + ofGetWidth()*0.2 * 2, y + 12,
					ofMap(message.value, 0, 127, 0, ofGetWidth()*0.2), 10);
			}
			text << " "; // pad for delta print
		}
		else {
			text << message.bytes.size() << " bytes ";
		}

		text << "delta: " << message.deltatime;
		ofSetColor(0);
		ofDrawBitmapString(text.str(), x, y);
		text.str(""); // clear
	}
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
    
    if(message.channel == 10) {
          return; // Ignore this message and return early
      }
    
    // This function is used to disable the printing of time
    if(message.status == MIDI_TIME_CLOCK) {
        return; // Ignore this message and return early
    }
	// add the latest message to the message queue
	midiMessages.push_back(message);

	// remove any old messages if we have too many
	while(midiMessages.size() > maxMessages) {
		midiMessages.erase(midiMessages.begin());
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

//--------------------------------------------------------------
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
void ofApp::mouseReleased() {
}
