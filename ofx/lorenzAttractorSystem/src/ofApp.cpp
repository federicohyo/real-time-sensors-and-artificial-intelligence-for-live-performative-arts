#include "ofApp.h"

/*
A direct port of http://nathanselikoff.com/training/tutorial-strange-attractors-in-c-and-opengl
Nathan Selikoff's tutorial Strange Attractors in C++ and OpenGL
*/

//--------------------------------------------------------------
void ofApp::setup(){
	strangeMesh.setMode(OF_PRIMITIVE_POINTS);
	strangeMesh.enableColors();
	
	float	x = 0.1, y = 0.1,		// starting point
	a = -0.966918,					// coefficients for "The King's Dream"
	b = 2.879879,
	c = 0.765145,
	d = 0.744728;
	
	int	initialIterations = 100,	// initial number of iterations
									// to allow the attractor to settle
	iterations = 1000000;			// number of times to iterate through
									// the functions and draw a point
	
	initialZoomFactor = 100.0;		// blows the points out to a usable scale
	
	// compute some initial iterations to settle into the orbit of the attractor
	for (int i = 0; i < initialIterations; i++) {
 
		// compute a new point using the strange attractor equations
		float xnew = sin(y*b) + c*sin(x*b);
		float ynew = sin(x*a) + d*sin(y*a);
 
		// save the new point
		x = xnew;
		y = ynew;
	}

	
	// go through the equations many times, drawing a point for each iteration
	for (int i = 0; i < iterations; i++) {
 
		// compute a new point using the strange attractor equations
		float xnew = sin(y*b) + c*sin(x*b);
		float ynew = sin(x*a) + d*sin(y*a);
 
		// save the new point
		x = xnew;
		y = ynew;
 
		// draw the new point
		// glVertex2f(x, y);
		
		ofVec3f tempVec3f(xnew*initialZoomFactor, ynew*initialZoomFactor, 0.0);
		
		strangeMesh.addVertex(tempVec3f);
		strangeMesh.addColor(ofFloatColor(1.0, 1.0, 1.0, 0.025));
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255,155,155);
	cam.begin();
	strangeMesh.draw();
	cam.end();
	

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}