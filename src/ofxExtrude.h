#pragma once
#include "ofVboMesh.h"
#include "ofPath.h"

class ofxExtrude {
public:

	static ofVboMesh getByCurve(const ofPath& path, const ofPolyline& curve, bool cap = true) {
		getByCurve(path.getTessellation(), path, curve, cap);
	}

	static ofVboMesh getByCurve(const ofMesh& tess, const ofPath& path, const ofPolyline& curve, bool cap = true) {

		ofVboMesh front(tess), back(tess);

		glm::mat4 currentMat = glm::translate(curve[0]);;

		if (cap) {
			for (auto&& v : front.getVertices()) {
				v = currentMat * glm::vec4(v, 1.);
			}
		}

		ofVboMesh side;

		int size = path.getOutline().size();

		for (auto& outline : path.getOutline()) {

			currentMat = glm::translate(curve[0]);

			for (int i = 1; i < curve.size(); i++) {
				auto d = curve[i] - curve[i - 1];
				glm::quat q = glm::rotation(glm::vec3(0, 0, 1.), glm::normalize(d)); // compute the rotation between two vectors
				glm::mat4 rot = glm::toMat4(q);
				glm::mat4 trans = glm::translate(curve[i]);
				glm::mat4 mat = trans * rot;

				side.append(getSide(outline, currentMat, mat));

				currentMat = mat;
			}
		}

		// cap back side
		if (cap) {
			for (auto&& v : back.getVertices()) {
				v = currentMat * glm::vec4(v, 1.);
			}
		}

		ofVboMesh result;
		if (cap) result.append(front);
		result.append(side);
		if (cap) result.append(back);

		return result;
	}

	static ofVboMesh getByStraight(const ofPath& path, const glm::vec3& direction, bool cap = true) {
		return getByStraight(path.getTessellation(), path, direction, cap);
	}

	static ofVboMesh getByStraight(const ofMesh& tess, const ofPath& path, const glm::vec3& direction, bool cap = true) {

		ofVboMesh front(tess), back(tess); // traingles

		// create side
		ofVboMesh side;
		for (auto& outline : path.getOutline()) {
			side.append(getSide(outline, glm::mat4(), glm::translate(direction)));			
		}

		// cap back side
		for (auto&& v : back.getVertices()) {
			v = v + direction;
		}

		ofVboMesh result;
		result.append(front);
		result.append(side);
		result.append(back);

		return result;
	
	}

private:
	static ofMesh getSide(const ofPolyline& outline, const glm::mat4& m1, const glm::mat4& m2) {
		ofMesh mesh;
		for (auto& v : outline.getVertices()) {
			mesh.addVertex(m1 * glm::vec4(v, 1.));
			mesh.addVertex(m2 * glm::vec4(v, 1.));
		}

		for (int i = 0; i < outline.size(); i++) {
			if (i != outline.size() - 1) {
				mesh.addIndex(i * 2);
				mesh.addIndex(i * 2 + 1);
				mesh.addIndex(i * 2 + 3);
				mesh.addIndex(i * 2);
				mesh.addIndex(i * 2 + 3);
				mesh.addIndex(i * 2 + 2);
			} else {
				mesh.addIndex(i * 2);
				mesh.addIndex(i * 2 + 1);
				mesh.addIndex(1);
				mesh.addIndex(i * 2);
				mesh.addIndex(1);
				mesh.addIndex(0);
			}
		}

		return mesh;
	}

	

};