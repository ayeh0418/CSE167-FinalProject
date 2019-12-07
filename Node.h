#ifndef _NODE_H_
#define _NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class Node
{
protected:
	glm::mat4 model;
	glm::vec3 color;
public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
	virtual void setShowRobot(bool showR) = 0;
	virtual bool getShowRobot() = 0;
	virtual void draw(GLuint shaderProgram, glm::mat4 C) = 0;
	virtual void update(glm::mat4 C) = 0;
};

#endif