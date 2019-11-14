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

	for (int i = 0; i <= 150; i++) {
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

	// c1 continuity lines
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * controlPts.size(), controlPts.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

	sphere1 = new Geometry("sphere.obj", 0);
	sphere2 = new Geometry("sphere.obj", 0);
	sphere3 = new Geometry("sphere.obj", 0);
	sphere4 = new Geometry("sphere.obj", 0);
}

BezierCurve::~BezierCurve() {
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo2);
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vao2);
}

glm::vec3 BezierCurve::getPoint(float t) {
	return a * pow(t, 3) + b * pow(t, 2) + c * t + d;
}

void BezierCurve::draw(GLuint shader, glm::mat4 model) {
	// glUseProgram(trackShader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 0, 0)));
	// Bind to the VAO.
	glBindVertexArray(vao);
	glLineWidth(2);
	glDrawArrays(GL_LINE_STRIP, 0, 151);
	// glDrawElements(GL_LINE_STRIP, pts.size(), GL_UNSIGNED_INT, 0);

	// Unbind from the VAO.
	glBindVertexArray(0);

	// draw c1 continuity lines
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 1, 0)));
	glBindVertexArray(vao2);
	glLineWidth(1.5);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);


	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.06f));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1, 0, 0)));
	sphere1->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[0]) * scale);
	sphere4->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[3]) * scale);
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0, 1, 0)));
	sphere2->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[1]) * scale);
	sphere3->draw(shader, glm::translate(glm::mat4(1.0f), controlPts[2]) * scale);
	
}