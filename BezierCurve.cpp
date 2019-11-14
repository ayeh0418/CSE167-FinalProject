#include "BezierCurve.h"
#include <fstream>
#include <sstream>
#include <iostream>

// constructor
BezierCurve::BezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	active = 3; // no point selected by default

	// store control points in a vector
	controlPts.push_back(p0);
	controlPts.push_back(p1);
	controlPts.push_back(p2);
	controlPts.push_back(p3);

	// compute weights
	a = -controlPts[0] + 3.0f * controlPts[1] - 3.0f * controlPts[2] + controlPts[3];
	b = 3.0f * controlPts[0] - 6.0f * controlPts[1] + 3.0f * controlPts[2];
	c = -3.0f * controlPts[0] + 3.0f * controlPts[1];
	d = controlPts[0];

	// get points on the curve
	for (int i = 0; i <= 150; i++) {
		pts.push_back(getPoint(i / 150.0f));
	}

	// curve
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pts.size(), pts.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// c1 continuity lines
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * controlPts.size(), controlPts.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind from the VBO.
	glBindVertexArray(0);			  // Unbind from the VAO.

	// spheres representing control points
	sphere1 = new Geometry("sphere.obj", 0);
	sphere2 = new Geometry("sphere.obj", 0);
	sphere3 = new Geometry("sphere.obj", 0);
	sphere4 = new Geometry("sphere.obj", 0);
}

// destructor
BezierCurve::~BezierCurve() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo2);
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vao2);
}

// compute a point on the curve
glm::vec3 BezierCurve::getPoint(float t) {
	return a * pow(t, 3) + b * pow(t, 2) + c * t + d;
}

// render the curve, points, and continuity lines
void BezierCurve::draw(GLuint shader, glm::mat4 model) {
	// pass in parameters to track_shader
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 0, 0)));

	// draw the curve
	glBindVertexArray(vao);
	glLineWidth(2);
	glDrawArrays(GL_LINE_STRIP, 0, 151);
	glBindVertexArray(0);

	// draw c1 continuity lines
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 1, 0)));
	glBindVertexArray(vao2);
	glLineWidth(1.5);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);

	// draw spheres at control points
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.06f));
	/*
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 0, 0)));
	sphere1->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[0]) * scale);
	// sphere4->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[3]) * scale);
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 1, 0)));
	sphere2->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[1]) * scale);
	sphere3->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[2]) * scale);
	*/

	// change color of the selective control point
	switch (active) {
	case 0:
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
		sphere1->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[0]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 0, 0)));
		// sphere4->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[3]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 1, 0)));
		sphere2->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[1]) * scale);
		sphere3->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[2]) * scale);
		break;

	case 1:
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 0, 0)));
		sphere1->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[0]) * scale);
		// sphere4->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[3]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
		sphere2->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[1]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 1, 0)));
		sphere3->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[2]) * scale);
		break;

	case 2:
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 0, 0)));
		sphere1->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[0]) * scale);
		// sphere4->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[3]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 1, 0)));
		sphere2->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[1]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
		sphere3->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[2]) * scale);
		break;
		
	case 3:
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 0, 0)));
		sphere1->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[0]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
		// sphere4->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[3]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 1, 0)));
		sphere2->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[1]) * scale);
		sphere3->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[2]) * scale);
		break;
		
	default:
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 0, 0)));
		sphere1->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[0]) * scale);
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 1, 0)));
		sphere2->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[1]) * scale);
		sphere3->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[2]) * scale);
		break;
	}
}

// update points
void BezierCurve::update(){
	// compute weights
	a = -controlPts[0] + 3.0f * controlPts[1] - 3.0f * controlPts[2] + controlPts[3];
	b = 3.0f * controlPts[0] - 6.0f * controlPts[1] + 3.0f * controlPts[2];
	c = -3.0f * controlPts[0] + 3.0f * controlPts[1];
	d = controlPts[0];

	// reset points on the curve
	pts.clear();
	for (int i = 0; i <= 150; i++) {
		pts.push_back(getPoint(i / 150.0f));
	}

	// curve
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pts.size(), pts.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// c1 continuity lines
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * controlPts.size(), controlPts.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind from the VBO.
	glBindVertexArray(0);			  // Unbind from the VAO.
}