#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <list>

#include "Node.h"

class Transform : public Node
{
private:
	glm::mat4 M;
	std::list<Node*> children = {};
	bool showRobot;
public:
	Transform(glm::mat4 transformM);
	~Transform();
	
	std::list<Node*> getChildren() { return children; }
	void setShowRobot(bool showR) { showRobot = showR; }
	bool getShowRobot() { return showRobot; }
	void draw(GLuint shaderprogram, glm::mat4 C) override;
	void update(glm::mat4 C) override;
	void addChild(Node* child);
	glm::mat4 getModel();
};

#endif