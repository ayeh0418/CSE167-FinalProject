#ifndef _TRACK_H_
#define _TRACK_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "shader.h"
#include "BezierCurve.h"
#include "Geometry.h"

class Track
{
private:
	
	// std::vector<glm::vec3> controlPts;
public:
	std::vector<BezierCurve*> curves;

	Track();
	~Track();
	void draw(GLuint shader, glm::mat4 model);
};

#endif

