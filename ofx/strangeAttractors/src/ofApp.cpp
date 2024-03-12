#include "ofApp.h"
#include "attractors.h"


void ofApp::setup(){
    ofBackground(12,125,125);
    myCam.setAutoDistance(false);
    //myCam.setControlArea(ofRectangle(200, 100, 300,400));
    //ofSetColor(0,0,0,100);

    // allocate fbo , mesh and imagePol
    //mymesh.setMode(OF_PRIMITIVE_POINTS);
    //glEnable(GL_POINT_SMOOTH); // use circular points instead of square points

}

void ofApp::update(){
    /*attractor.step();
    points.push_back(TRANSFORM * attractor.position + OFFSET);
    
    for (int i=0; i < static_cast<int>(points.size()); i++) {
        float* data = glm::value_ptr(points[i]);
        //ofDrawCircle(data[0], data[1], data[2]);
        mymesh.addVertex(ofVec3f(ofMap(data[0],0,640,0,ofGetWidth()), ofMap(data[1],0,640,0,ofGetHeight()), 0));
                                //ofMap(data[2],0,640,0,ofGetWidth()))); // make a new vertex
        mymesh.addTexCoord(ofVec2f(data[0],data[1]));
        mymesh.addColor(ofFloatColor(255,255,255));  // add a color at that vertex
    }*/

}

void ofApp::draw(){
//
   // ofSetColor(0,0,0,100);
    myCam.begin();
    ofNoFill();
    ofSetColor(255);
    ofDrawSphere(64);
    ofDrawCircle(0,0,72);

    //ofDrawGrid();
    //ofTranslate(ofPoint(-ofGetWidth()/2,-ofGetHeight()/2));
    //mesh.setMode(OF_PRIMITIVE_POINTS);
    //ofPushMatrix();
    //mymesh.draw();

    myCam.end();   


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