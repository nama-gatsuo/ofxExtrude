#pragma once

#include "ofMain.h"
#include "ofxExtrude.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

private:	
	ofEasyCam cam;
	ofVboMesh meshStar;
	ofVboMesh meshTypo;
	
	ofxPanel panel;
	ofParameter<glm::vec3> starPos;
	ofParameter<glm::vec3> starDepth;
	ofParameter<int> starNum;
	ofParameter<float> starR1;
	ofParameter<float> starR2;

	ofParameter<glm::vec3> typoPos;
	ofParameter<std::string> typoContent;
	ofParameter<glm::vec2> typoP1;
	ofParameter<glm::vec2> typoP2;
	ofParameter<glm::vec2> typoN1;
	ofParameter<glm::vec2> typoN2;
	
	std::array<ofEventListener, 9> listeners;
	ofTrueTypeFont verdana;

	ofPath createStar(float r1, float r2, int num);
	ofMesh createStarExt(float r1, float r2, int num, const glm::vec3& depth);
	ofMesh createTypo(const std::string& str, const glm::vec2& p1, const glm::vec2& n1, const glm::vec2& p2, const glm::vec2& n2);

};
