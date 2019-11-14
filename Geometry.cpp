#include "Geometry.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Window.h"

Geometry::Geometry(std::string filename, unsigned int texture) {
	file = filename;
	init(filename);
	skyboxTexture = texture;
}

Geometry::~Geometry() {
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo2);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Geometry::draw(GLuint programShader, glm::mat4 C) {

	model = C;

	glUniformMatrix4fv(glGetUniformLocation(programShader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// Bind to the VAO.
	glBindVertexArray(vao);
	/*
	if (file.compare("sphere.obj") == 0) {
		if (show) {
			glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
		}
	}*/
	// else {
	//	if (showRobot) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);  	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//	}
	// }
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Geometry::update(glm::mat4 C) {}

void Geometry::init(std::string filename) {
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::ivec3> faces;
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> normalIndices;
	std::ifstream objFile(filename); // The obj file we are reading.

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				points.push_back(point);
			}

			if (label == "vn")
			{
				// Read the later three float numbers and use them as the 
				// coordinates
				glm::vec3 vn;
				ss >> vn.x >> vn.y >> vn.z;

				// Process the point
				vertexNormals.push_back(vn);
			}

			if (label == "f")
			{
				// Read the three numbers and use them as the 
				// vertices.
				std::string index1, index2, index3;
				unsigned int v1, v2, v3, n1, n2, n3;
				glm::ivec3 face;

				ss >> index1 >> index2 >> index3;

				v1 = std::stof(index1.substr(0, index1.find("/")));
				v1--;
				// face.x = v1;
				vertexIndices.push_back(v1);
				index1 = index1.substr(index1.find("/") + 1);
				v2 = std::stof(index2.substr(0, index2.find("/")));
				v2--;
				// face.y = v2;
				vertexIndices.push_back(v2);
				index2 = index2.substr(index2.find("/") + 1);
				v3 = std::stof(index3.substr(0, index3.find("/")));
				v3--;
				// face.z = v3;
				vertexIndices.push_back(v3);
				index3 = index3.substr(index3.find("/") + 1);
				// Process the point. For example, you can save it to a.
				// faces.push_back(face);

				n1 = std::stof(index1.substr(index1.find("/") + 1));
				n1--;
				normalIndices.push_back(n1);
				n2 = std::stof(index2.substr(index2.find("/") + 1));
				n2--;
				normalIndices.push_back(n2);
				n3 = std::stof(index3.substr(index3.find("/") + 1));
				n3--;
				normalIndices.push_back(n3);
			}
		}

		// Assume that you parse indices of v//vn into different std::vector (vertex_indices_, normal_indices_)
		// input_vertices and input_normals are raw input data from *.obj files
		// vertices_ and normals_ are aligned data
		for (unsigned i = 0; i < vertexIndices.size(); i++) {
			vertices.push_back(points[vertexIndices[i]]);
			normals.push_back(vertexNormals[normalIndices[i]]);
			indices.push_back(i);
		}
	}
	else
	{
		std::cerr << "Can't open the file " << filename << std::endl;
	}

	objFile.close();

	// initialization
	GLfloat maxX = 0;
	GLfloat maxY = 0;
	GLfloat maxZ = 0;
	GLfloat minX = 0;
	GLfloat minY = 0;
	GLfloat minZ = 0;
	GLfloat midX = 0;
	GLfloat midY = 0;
	GLfloat midZ = 0;
	GLfloat maxD = 0;

	// find the max and mix coordinates along each axis
	for (int i = 0; i < points.size(); i++) {
		if (i == 0) {
			minX = points[i].x;
			minY = points[i].y;
			minZ = points[i].z;
		}
		else {
			if (points[i].x > maxX) {
				maxX = points[i].x;
			}

			if (points[i].y > maxY) {
				maxY = points[i].y;
			}

			if (points[i].z > maxZ) {
				maxZ = points[i].z;
			}

			if (points[i].x < minX) {
				minX = points[i].x;
			}

			if (points[i].y < minY) {
				minY = points[i].y;
			}

			if (points[i].z < minZ) {
				minZ = points[i].z;
			}
		}
	}

	// find the midpoints
	midX = (maxX + minX) / 2;
	midY = (maxY + minY) / 2;
	midZ = (maxZ + minZ) / 2;

	// cetner the model
	for (int j = 0; j < points.size(); j++) {
		points[j].x -= midX;
		points[j].y -= midY;
		points[j].z -= midZ;
	}


	// find the maximum distance from any point to the center
	for (int k = 0; k < points.size(); k++) {

		GLfloat distance = sqrt(pow(points[k].x, 2) + pow(points[k].y, 2) +
			pow(points[k].z, 2));
		if (distance > maxD) {
			maxD = distance;
		}
	}

	GLfloat scale = 11 / maxD; // the scalar

	// scale the model to fit the window
	for (int a = 0; a < points.size(); a++) {
		points[a].x = scale * (GLfloat)points[a].x;
		points[a].y = scale * (GLfloat)points[a].y;
		points[a].z = scale * (GLfloat)points[a].z;
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 1, 1);
	
	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
 
	// Generate a second VBO
	glGenBuffers(1, &vbo2);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the second VBO. We will use it to store the normals.
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);

	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
		normals.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	
	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0],
		GL_STATIC_DRAW);


	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}