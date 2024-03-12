#pragma once

#include "ofMain.h"
#include "ofxTensorFlow2.h"
#include "ofxMidi.h"

#define USE_LIVE_VID

class ImageToImageModel : public ofxTF2::ThreadedModel {

	public:

		// override the runModel function of ThreadedModel
		// this way the thread will take this augmented function
		// otherwise it would call runModel with no way of pre-/postprocessing
		cppflow::tensor runModel(const cppflow::tensor & input) const override {
			// cast data type and expand to batch size of 1
			auto inputCast = cppflow::cast(input, TF_UINT8, TF_FLOAT);
			inputCast = cppflow::expand_dims(inputCast, 0);
			// call to super
			auto output = Model::runModel(inputCast);
			// postprocess: last layer = (tf.nn.tanh(x) * 150 + 255. / 2)
			return ofxTF2::mapTensorValues(output, -22.5f, 277.5f, 0.0f, 255.0f);
		}
};

class ofApp : public ofBaseApp, public ofxMidiListener{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// Midi
		void newMidiMessage(ofxMidiMessage &message);
		ofxMidiIn midiIn;
		std::vector<ofxMidiMessage> midiMessages; 
		std::size_t maxMessages = 10; 
		
		//Tensorflow style transfer
		std::vector<std::string> modelPaths; // paths to available models
		std::size_t modelIndex = 0; // current model path index
		std::string modelName = ""; // current model path name
		bool newInput = false;      // is there new input to process?
		bool autoLoad = true;       // load models automatically?
		float loadTimestamp = 0;    // last model load time stamp
		float loadTimeSeconds = 10; // how long to wait before loading models
		
		ImageToImageModel model;
		cppflow::tensor input;
		cppflow::tensor output;
		int nnWidth = 640;
		int nnHeight = 480;


		ofVideoGrabber vidIn;
		int camWidth = 640;
		int camHeight = 480;
		ofImage imgOut;
};
