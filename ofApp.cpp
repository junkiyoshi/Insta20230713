#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofSetLineWidth(3);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
	for (int k = 0; k < 1; k++) {

		ofMesh mesh;
		vector<glm::vec3> right, left, frame;

		glm::vec3 last_location;
		float last_theta;

		for (int i = 0; i < 24; i++) {

			auto len = ofMap(ofNoise((ofGetFrameNum() + i + k * 10) * 0.04), 0, 1, 200, 500);
			auto next_len = ofMap(ofNoise((ofGetFrameNum() + i + 1 + k * 10) * 0.04), 0, 1, 200, 500);

			auto param = ofGetFrameNum() + i;
			auto next_param = ofGetFrameNum() + i + 1;

			auto location = glm::vec3(this->make_point(len, param), 0);
			auto next = glm::vec3(this->make_point(len, next_param), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, 24, 0, 4) * cos(theta + PI * 0.5), ofMap(i, 0, 24, 0, 4) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, 24, 0, 4) * cos(theta - PI * 0.5), ofMap(i, 0, 24, 0, 4) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);
		}


		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		mesh.addVertex(last_location);
		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(4 * cos(theta), 4 * sin(theta), 0));
			frame.push_back(last_location + glm::vec3(4 * cos(theta), 4 * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		reverse(right.begin(), right.end());
		ofColor color;
		for (int i = 0; i < 10; i++) {

			ofRotate(36);

			color.setHsb(ofMap(i, 0, 10, 0, 255), 150, 255);
			ofSetColor(color);

			ofNoFill();
			ofBeginShape();
			ofVertices(frame);
			
			ofVertices(right);
			ofVertices(left);
			ofEndShape(false);
		}

		for (int i = 0; i < 10; i++) {

			ofRotate(36);

			ofSetColor(39);
			mesh.draw();
		}
	}
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}