#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include "Skybox.h"

Skybox::Skybox(float size, std::vector<std::string> skyboxVec)
{
	skyboxShader = LoadShaders("shaders/skybox_shader.vert", "shaders/skybox_shader.frag");
	std::vector<std::string> skybox_faces = skyboxVec;

	cubemapTexture = loadCubemap(skybox_faces);
	// The 8 vertices of a cube.
	std::vector<glm::vec3> vertices
	{
		glm::vec3(-500, 500, 500),
		glm::vec3(-500, -500, 500),
		glm::vec3(500, -500, 500),
		glm::vec3(500, 500, 500),
		glm::vec3(-500, 500, -500),
		glm::vec3(-500, -500, -500),
		glm::vec3(500, -500, -500),
		glm::vec3(500, 500, -500)
	};

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.
	std::vector<glm::ivec3> indices
	{
		// Front face.
		glm::ivec3(2, 1, 0),
		glm::ivec3(0, 3, 2),

		// Back face.
		glm::ivec3(5, 6, 7),
		glm::ivec3(7, 4, 5),

		// Right face.
		glm::ivec3(6, 2, 3),
		glm::ivec3(3, 7, 6),

		// Left face.
		glm::ivec3(1, 5, 4),
		glm::ivec3(4, 0, 1),

		// Top face.
		glm::ivec3(3, 0, 4),
		glm::ivec3(4, 7, 3),

		// Bottom face.
		glm::ivec3(6, 5, 1),
		glm::ivec3(1, 2, 6)
	};

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

	M = glm::mat4(1.0f);
}

Skybox::~Skybox()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Skybox::draw()
{
	// glDepthMask(GL_FALSE);
	glUseProgram(skyboxShader);
	glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);

	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "model"), 1, GL_FALSE, glm::value_ptr(M));
	// ... set view and projection matrix
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);

	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Skybox::update(glm::mat4 C)
{
	M = C * M;
}


unsigned int Skybox::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	return textureID;
}

