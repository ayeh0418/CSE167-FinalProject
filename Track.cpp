#include "Track.h"

Track::Track() {
	currCurve = 0;

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

void Track::update() {
	switch (currCurve) {
	case 0:
		if (currPt != 1) {
			curves[0]->controlPts[1] = curves[7]->getNewPt();
			curves[1]->controlPts[1] = curves[0]->getNewPt();
		}
		else {
			curves[7]->controlPts[2] = curves[0]->getNewPtPrev();
		}
		break;

	case 1:
		if (currPt != 1) {
			curves[1]->controlPts[1] = curves[0]->getNewPt();
			curves[2]->controlPts[1] = curves[1]->getNewPt();
		}
		else {
			curves[0]->controlPts[2] = curves[1]->getNewPtPrev();
		}
		break;

	case 2:
		if (currPt != 1) {
			curves[2]->controlPts[1] = curves[1]->getNewPt();
			curves[3]->controlPts[1] = curves[2]->getNewPt();
		}
		else {
			curves[1]->controlPts[2] = curves[2]->getNewPtPrev();
		}
		break;

	case 3:
		if (currPt != 1) {
			curves[3]->controlPts[1] = curves[2]->getNewPt();
			curves[4]->controlPts[1] = curves[3]->getNewPt();
		}
		else {
			curves[2]->controlPts[2] = curves[3]->getNewPtPrev();
		}
		break;

	case 4:
		if (currPt != 1) {
			curves[4]->controlPts[1] = curves[3]->getNewPt();
			curves[5]->controlPts[1] = curves[4]->getNewPt();
		}
		else {
			curves[3]->controlPts[2] = curves[4]->getNewPtPrev();
		}
		break;

	case 5:
		if (currPt != 1) {
			curves[5]->controlPts[1] = curves[4]->getNewPt();
			curves[6]->controlPts[1] = curves[5]->getNewPt();
		}
		else {
			curves[4]->controlPts[2] = curves[5]->getNewPtPrev();
		}
		break;

	case 6:
		if (currPt != 1) {
			curves[6]->controlPts[1] = curves[5]->getNewPt();
			curves[7]->controlPts[1] = curves[6]->getNewPt();
		}
		else {
			curves[5]->controlPts[2] = curves[6]->getNewPtPrev();
		}
		break;

	case 7:
		if (currPt != 1) {
			curves[7]->controlPts[1] = curves[6]->getNewPt();
			curves[0]->controlPts[1] = curves[7]->getNewPt();
		}
		else {
			curves[6]->controlPts[2] = curves[7]->getNewPtPrev();
		}
		break;

	default:
		break;
	}
	curves[currCurve]->update();
	curves[(currCurve + 1) % 8]->update();
	curves[(currCurve + 7) % 8]->update();
}