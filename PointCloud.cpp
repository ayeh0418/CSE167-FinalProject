#include "PointCloud.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Window.h"

PointCloud::PointCloud(std::string objFilename, float width, float height, 
	GLfloat pointSize): pointSize(pointSize)
{
	std::ifstream objFile(objFilename); // The obj file we are reading.

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
				glm::ivec3 face;

				ss >> index1 >> index2 >> index3;
				
				face.x = std::stof(index1.substr(0, index1.find("/")));
				face.x--;
				face.y = std::stof(index2.substr(0, index2.find("/")));
				face.y--;
				face.z = std::stof(index3.substr(0, index3.find("/")));
				face.z--;
				// Process the point. For example, you can save it to a.
				faces.push_back(face);
			}
			
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	glGenBuffers(1, &vbo2);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);

	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNormals.size(),
		vertexNormals.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(),
		GL_STATIC_DRAW);


	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	//glPointSize(pointSize);
	// Draw points 
	//glDrawArrays(GL_POINTS, 0, points.size());
	glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	//spin(0.1f);
}

void PointCloud::updatePointSize(GLfloat size) 
{
	// update the point size
	pointSize += size;
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PointCloud::scale(glm::mat4 scaler)
{
	model = model * scaler;
}

void PointCloud::rotate(glm::mat4 rotateM)
{
	model = model * rotateM;
}