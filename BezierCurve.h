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
	glm::vec3 a, b, c, d;		 // weights
	std::vector<glm::vec3> pts;  // points on the curve
	GLuint vao, vao2, vbo, vbo2; // buffers
	Geometry* sphere1;			 // representing control point 1
	Geometry* sphere2;			 // representing control point 2
	Geometry* sphere3;			 // representing control point 3
	Geometry* sphere4;			 // representing control point 4
	int active;					 // keep track of the selected control point
public:
	std::vector<glm::vec3> controlPts;

	BezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	~BezierCurve();
	glm::vec3 getPoint(float t);
	void draw(GLuint shader, glm::mat4 model);
	glm::vec3 getNewPt() { return 2.0f * controlPts[3] - controlPts[2]; }
	glm::vec3 getNewPtPrev() { return 2.0f * controlPts[0] - controlPts[1]; }
	void update();
	void setActive(int a) { active = a; }
};

#endif