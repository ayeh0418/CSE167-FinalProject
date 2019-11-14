#include "BezierCurve.h"
#include <fstream>
#include <sstream>
#include <iostream>

BezierCurve::BezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	// trackShader = LoadShaders("shaders/track_shader.vert", "shaders/track_shader.frag");
	a = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
	b = 3.0f * p0 - 6.0f * p1 + 3.0f * p2;
	c = -3.0f * p0 + 3.0f * p1;
	d = p0;
	controlPts.push_back(p0);
	controlPts.push_back(p1);
	controlPts.push_back(p2);
	controlPts.push_back(p3);

	for (int i = 0; i < 150; i++) {
		// std::cout << getPoint(i / 150.0f).x << " " << getPoint(i / 150.0f).y << " " << getPoint(i / 150.0f).z << std::endl;
		pts.push_back(getPoint(i / 150.0f));
	}

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pts.size(),
		pts.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

BezierCurve::~BezierCurve() {
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

glm::vec3 BezierCurve::getPoint(float t) {
	return a * pow(t, 3) + b * pow(t, 2) + c * t + d;
}

void BezierCurve::draw(GLuint shader, glm::mat4 model) {
	// glUseProgram(trackShader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// Bind to the VAO.
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, pts.size());
	// glDrawElements(GL_LINE_STRIP, pts.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}