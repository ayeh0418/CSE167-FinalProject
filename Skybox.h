#ifndef _SKYBOX_H_
#define _SKYBOX_H_

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

class Skybox
{
private:
	std::vector<unsigned int> indices;
	GLuint vao, vbos[2];
	GLuint skyboxShader;
	unsigned int cubemapTexture;

public:
	Skybox(float size, std::vector<std::string> skyboxVec);
	~Skybox();

	void draw();
	void update();
	unsigned int loadCubemap(std::vector<std::string> faces);
	unsigned int getTexture() { return cubemapTexture; }
};

#endif