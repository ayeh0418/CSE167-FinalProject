#ifndef _BEZIER_CURVE_H_
#define _BEZIER_CURVE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "shader.h"
#include "Geometry.h"

class BezierCurve
{
private:
	glm::vec3 a, b, c, d;
	std::vector<glm::vec3> pts;
	std::vector<glm::vec3> controlPts;
	GLuint vao, vao2, vbo, vbo2;
	Geometry* sphere1;
	Geometry* sphere2;
	Geometry* sphere3;
	Geometry* sphere4;
public:
	BezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	~BezierCurve();
	glm::vec3 getPoint(float t);
	void draw(GLuint shader, glm::mat4 model);
	glm::vec3 getNewPt() { return 2.0f * controlPts[3] - controlPts[2]; }
	// void set2ndPt(glm::vec3 secPt) { controlPts[1] = secPt; }
};

#endif