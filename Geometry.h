#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

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

class Geometry : public Node
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> faces;
	std::vector<unsigned int> indices;
	GLuint vao, vbo, ebo, vbo2;
	std::string file;
	bool show;
	bool showRobot;
	unsigned int skyboxTexture;

public:
	Geometry(std::string filename, unsigned int texture);
	~Geometry();

	bool getShow() { return show; }
	void setShow(bool showSphere) { show = showSphere; }
	void setShowRobot(bool showR) {}
	bool getShowRobot() { return showRobot; }
	void draw(GLuint programShader, glm::mat4 C);
	void update(glm::mat4 C);
	void init(std::string filename);

	void setSkyboxTexture(unsigned int newTexture);
};

#endif