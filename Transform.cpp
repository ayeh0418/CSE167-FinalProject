#include "Transform.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Window.h"

Transform::Transform(glm::mat4 transformM) {
	M = transformM;
	showRobot = true;
}

Transform::~Transform() {
	for (Node* child : children) {
		delete child;
	}
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C) { 
	if (showRobot) {
		glm::mat4 newM = C * M;
		for (const auto& child : children) {
			child->draw(shaderProgram, newM);
		}
	}
}

void Transform::update(glm::mat4 C) {
	M = C * M;
}

void Transform::addChild(Node* child) {
	children.push_back(child);
}

glm::mat4 Transform::getModel() {
	return M;
}
/*
void Transform::setShowRobot(bool showR) {

	showRobot = showR;
	
	for (Node* child : children) {
		child->setShowRobot(showR);
	}
	
}
*/