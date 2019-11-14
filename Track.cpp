#include "Track.h"

Track::Track() {
	glm::vec3 temp = glm::vec3(0, 0, 0);
	BezierCurve* bCurve1 = new BezierCurve(glm::vec3(-7, 0, 0), glm::vec3(-6, 0, -1), glm::vec3(-5, 1, -2), glm::vec3(-4, 1, -3));
	temp = bCurve1->getNewPt();
	BezierCurve* bCurve2 = new BezierCurve(glm::vec3(-4, 1, -3), temp, glm::vec3(-1, 2, -5), glm::vec3(0, 2, -6));
	temp = bCurve2->getNewPt();
	BezierCurve* bCurve3 = new BezierCurve(glm::vec3(0, 2, -6), temp, glm::vec3(2, 1, -4), glm::vec3(3, 1, -3));
	temp = bCurve3->getNewPt();
	BezierCurve* bCurve4 = new BezierCurve(glm::vec3(3, 1, -3), temp, glm::vec3(5, 0, -1), glm::vec3(6, 0, 0));
	temp = bCurve4->getNewPt();
	BezierCurve* bCurve5 = new BezierCurve(glm::vec3(6, 0, 0), temp, glm::vec3(4, -1, 2), glm::vec3(3, -1, 3));
	temp = bCurve5->getNewPt();
	BezierCurve* bCurve6 = new BezierCurve(glm::vec3(3, -1, 3), temp, glm::vec3(1, -2, 5), glm::vec3(0, -2, 6));
	temp = bCurve6->getNewPt();
	BezierCurve* bCurve7 = new BezierCurve(glm::vec3(0, -2, 6), temp, glm::vec3(-2, -1, 4), glm::vec3(-3, -1, 3));
	temp = bCurve7->getNewPt();
	BezierCurve* bCurve8 = new BezierCurve(glm::vec3(-3, -1, 3), temp, glm::vec3(-5, 0, 1), glm::vec3(-7, 0, 0));
	temp = bCurve8->getNewPt();
	bCurve1 = new BezierCurve(glm::vec3(-7, 0, 0), temp, glm::vec3(-5, 1, -2), glm::vec3(-4, 1, -3));

	curves.push_back(bCurve1);
	curves.push_back(bCurve2);
	curves.push_back(bCurve3);
	curves.push_back(bCurve4);
	curves.push_back(bCurve5);
	curves.push_back(bCurve6);
	curves.push_back(bCurve7);
	curves.push_back(bCurve8);
}

Track::~Track() {
	for (BezierCurve* bc : curves) {
		delete bc;
	}
}

void Track::draw(GLuint shader, glm::mat4 model) {
	for (BezierCurve* bc : curves) {
		bc->draw(shader, glm::mat4(1.0f));
	}
}