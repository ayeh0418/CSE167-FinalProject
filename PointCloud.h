#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include "Object.h"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::ivec3> faces;
	GLuint vao, vbo, ebo, vbo2;
	GLfloat pointSize;
public:
	PointCloud(std::string objFilename, float width, float height, 
		GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);
	void scale(glm::mat4 scaler);
	void rotate(glm::mat4 rotateM);
};

#endif

