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

class BezierCurve
{
private:
	glm::vec3 a, b, c, d;
	std::vector<glm::vec3> pts;
	std::vector<glm::vec3> controlPts;
	GLuint vao, vbo;
public:
	BezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	~BezierCurve();
	glm::vec3 getPoint(float t);
	void draw(GLuint shader, glm::mat4 model);
};

#endif