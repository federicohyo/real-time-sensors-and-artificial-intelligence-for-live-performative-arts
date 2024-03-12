#pragma once

#include "ofMain.h"
#include "attractors.h"


class ofApp : public ofBaseApp {
private:
    glm::mat3x2 TRANSFORM = { {6, 0}, {0, 0}, {0, 6} };
    glm::vec2 OFFSET = { ofGetWidth() * 0.5, ofGetHeight() * 0.3 };
    
    vector<glm::vec2> points;
    LorenzAttractor attractor {10.0, 28.0, 8.0 / 3.0};
    
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
  

    
    // Textures and framebuffer
    //ofFbo fbo;
    //ofTexture* tex;
    //ofTexture* getTextureRef();
    // easycam
    ofEasyCam myCam;
    //ofMesh mymesh;
 
};
