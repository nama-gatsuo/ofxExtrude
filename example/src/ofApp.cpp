#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	verdana.loadFont("verdana.ttf", 64, true, true, true);
	// prepare callback functions
	auto callbackCreateStarf = [&](float&) {
		meshStar = createStarExt(starR1.get(), starR2.get(), starNum.get(), starDepth.get());
	};
	auto callbackCreateStari = [&](int&) {
		meshStar = this->createStarExt(starR1.get(), starR2.get(), starNum.get(), starDepth.get());
	};
	auto callbackCreateStarf3 = [&](glm::vec3&) {
		meshStar = createStarExt(starR1.get(), starR2.get(), starNum.get(), starDepth.get());
	};
	auto callbackCreateTypof2 = [&](glm::vec2&){
		meshTypo = createTypo(typoContent.get(), typoP1.get(), typoN1.get(), typoP2.get(), typoN2.get());
	};
	auto callbackCreateTypofStr = [&](std::string&) {
		meshTypo = createTypo(typoContent.get(), typoP1.get(), typoN1.get(), typoP2.get(), typoN2.get());
	};

	// setups parameters
	ofParameterGroup starGroup("star");
	starGroup.add(starPos.set("star_pos", glm::vec3(100, 0, 0), glm::vec3(-500.), glm::vec3(500.)));
	starGroup.add(starNum.set("star_num", 5, 3, 10));
	starGroup.add(starR1.set("star_rad_outer", 50., 10., 100.));
	starGroup.add(starR2.set("star_rad_inner", 30., 10., 100.));
	starGroup.add(starDepth.set("star_depth", glm::vec3(0, 0, 100), glm::vec3(-300), glm::vec3(300)));
	
	ofParameterGroup typoGroup("typo");
	starGroup.add(typoPos.set("typo_pos", glm::vec3(-100, 0, 0), glm::vec3(-500.), glm::vec3(500.)));
	typoGroup.add(typoContent.set("typo_content", "a"));
	typoGroup.add(typoP1.set("typo_p1", glm::vec2(0), glm::vec2(0), glm::vec2(500)));
	typoGroup.add(typoN1.set("typo_n1", glm::vec2(400, 0), glm::vec2(0), glm::vec2(500)));
	typoGroup.add(typoP2.set("typo_p2", glm::vec2(500), glm::vec2(0), glm::vec2(500)));
	typoGroup.add(typoN2.set("typo_n2", glm::vec2(100, 500), glm::vec2(0), glm::vec2(500)));

	// set callbacks to parameters
	listeners[0] = starNum.newListener(callbackCreateStari);
	listeners[1] = starR1.newListener(callbackCreateStarf);
	listeners[2] = starR2.newListener(callbackCreateStarf);
	listeners[3] = starDepth.newListener(callbackCreateStarf3);

	listeners[4] = typoContent.newListener(callbackCreateTypofStr);
	listeners[5] = typoP1.newListener(callbackCreateTypof2);
	listeners[6] = typoN1.newListener(callbackCreateTypof2);
	listeners[7] = typoP2.newListener(callbackCreateTypof2);
	listeners[8] = typoN2.newListener(callbackCreateTypof2);

	panel.setup();
	panel.add(starGroup);
	panel.add(typoGroup);

	float f(0.);
	callbackCreateStarf(f);
	std::string str("a");
	callbackCreateTypofStr(str);

	ofBackground(64);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofEnableDepthTest();
	
	cam.begin();

	ofPushMatrix();
	ofTranslate(starPos.get());
	ofSetColor(128);
	meshStar.draw(OF_MESH_FILL);
	ofSetColor(255);
	meshStar.draw(OF_MESH_WIREFRAME);
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(typoPos.get());
	ofSetColor(128);
	meshTypo.draw(OF_MESH_FILL);
	ofSetColor(255);
	meshTypo.draw(OF_MESH_WIREFRAME);
	ofPopMatrix();

	cam.end();

	ofDisableDepthTest();
	panel.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

ofPath ofApp::createStar(float r1, float r2, int num) {
	ofPath path;
	float t = 0.;
	for (int i = 0; i < num; i++) {
		t = TWO_PI / float(num) * i;
		glm::vec2 p1(r1 * cos(t), r1 * sin(t));
		t = TWO_PI / float(num) * (i + 0.5);
		glm::vec2 p2(r2 * cos(t), r2 * sin(t));
		if (i == 0) {
			path.moveTo(p1);
			path.lineTo(p2);
		}
		path.lineTo(p1);
		path.lineTo(p2);
	}
	path.close();
	return path;
}

ofMesh ofApp::createStarExt(float r1, float r2, int num, const glm::vec3& depth) {
	ofPath star = createStar(r1, r2, num);
	return ofxExtrude::getByStraight(star, depth);
}

ofMesh ofApp::createTypo(const std::string& str, const glm::vec2& p1, const glm::vec2& n1, const glm::vec2& p2, const glm::vec2& n2) {
	
	auto path = verdana.getCharacterAsPoints(str.data()[0], false, false);
	auto tess = verdana.getCharacterAsPoints(str.data()[0], true, true).getTessellation();

	ofPath curve;
	curve.setCurveResolution(10);
	curve.moveTo(p1);
	curve.bezierTo(n1, n2, p2);
	auto curvePoints = curve.getOutline()[0];
	for (auto&& p : curvePoints) {
		p = glm::vec3(0, -p.y, p.x);
	}

	return ofxExtrude::getByCurve(tess, path, curvePoints);
}
