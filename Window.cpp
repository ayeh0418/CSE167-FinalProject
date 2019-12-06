#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "Project 3";

/*
Geometry * Window::eyeball;

*/
Geometry* Window::sphere;
Geometry* Window::head;
Geometry* Window::antenna;
Geometry* Window::wing;
Geometry* Window::body;
Transform* Window::spaceship;
Transform* Window::ship2world;
Transform* Window::body2ship;
Transform* Window::head2ship;
Transform* Window::head22ship;
Transform* Window::wind12ship;
Transform* Window::wind22ship;
Transform* Window::wind32ship;
Transform* Window::wind42ship;
Transform* Window::antenna12ship;
Transform* Window::antenna22ship;
Transform* Window::antenna32ship;
Transform* Window::antenna42ship;
Transform* Window::ball2ship;
bool Window::turnL = false;
bool Window::turnR = false;
bool Window::goForward = false;
bool Window::goBackward = false;
float Window::angle = 0;
/*
Track * Window::track;
Transform * Window::robot;
Transform * Window::squad;
Transform* Window::head2Robot;
Transform* Window::antenna12Robot;
Transform* Window::antenna22Robot;
Transform* Window::eyeballL2Robot;
Transform* Window::eyeballR2Robot;
Transform* Window::arm12Robot;
Transform* Window::arm22Robot;
Transform* Window::arm32Robot;
Transform* Window::arm42Robot;
Transform* Window::arm52Robot;
Transform* Window::arm62Robot;
Transform* Window::sphere2Robot;
Transform* Window::sphere2World;
*/
glm::mat4 Window::projection; // Projection matrix.
glm::vec3 Window::eyeVec = glm::vec3(0, 3, 10);
glm::vec3 Window::eye = eyeVec;  // Camera position. (0, 2, 10)
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.
glm::vec3 Window::lastPos(0, 0, 0);

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.
GLuint Window::skyboxProgram; // The shader program id.
GLuint Window::trackProgram; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::viewPosLoc; // Location of viewPos in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.

bool Window::pressed = false;
int Window::mode = 0;
int Window::timer = 0;
float Window::rotSpeed = -0.015f;
float Window::floatSpeed = -0.006f;
double Window::fov = 60;
double Window::maxFov = 60;
double Window::near = 1;
double Window::far = 1000;
double Window::nearW;
double Window::farW;
double Window::nearH;
double Window::farH;
glm::vec3 Window::right = glm::vec3(1.0f, 0, 0);
glm::vec3 Window::nc;
glm::vec3 Window::fc;
glm::vec3 Window::normalR;
glm::vec3 Window::normalL;
glm::vec3 Window::normalU;
glm::vec3 Window::normalD;
glm::vec3 Window::normalN;
glm::vec3 Window::normalF;
glm::vec3 Window::rightP;
glm::vec3 Window::leftP;
glm::vec3 Window::upP;
glm::vec3 Window::downP;
glm::vec3 Window::nearP;
glm::vec3 Window::farP;
bool Window::showRobot = true;
int Window::cullNum = 100;
bool Window::culling = false;

Skybox* Window::env;
Skybox* Window::env1;
Skybox* Window::env2;
Skybox* Window::env3;

int Window::controlPtCount = 0;
int Window::curveCount = 0;
int Window::prevCount = 7;
int Window::nextCount = 1;
int Window::curveCountUpdate = 0;
glm::vec3 Window::lastPt;
bool Window::pause = true;
double Window::oldTime = 0.0;
double Window::newTime = 0.0;
double Window::leftover = 0.0;
bool Window::camView = false;

int Window::planetNumber;

std::vector<std::vector<std::string>> Window::skyboxVec;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	skyboxProgram = LoadShaders("shaders/skybox_shader.vert", "shaders/skybox_shader.frag");
	trackProgram = LoadShaders("shaders/track_shader.vert", "shaders/track_shader.frag");

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	viewPosLoc = glGetUniformLocation(program, "viewPos");
	modelLoc = glGetUniformLocation(program, "model");
	// colorLoc = glGetUniformLocation(program, "color");

	
	return true;
}

bool Window::initializeObjects()
{
	// On initialize, we begin in galaxy.
	planetNumber = 0;

	std::vector<std::string> galaxy_skybox_faces
	{
		"Left_Tex.jpg",
		"Right_Tex.jpg",
		"Up_Tex.jpg",
		"Down_Tex.jpg",
		"Front_Tex.jpg",
		"Back_Tex.jpg"
	};

	std::vector<std::string> red_skybox_faces
	{
		"redSkyLeft.png",
		"redSkyRight.png",
		"redSkyUp.png",
		"redSkyDown.png",
		"redSkyFront.png",
		"redSkyBack.png"
	};

	std::vector<std::string> dark_skybox_faces
	{
		"darkSkyLeft.png",
		"darkSkyRight.png",
		"darkSkyUp.png",
		"darkSkyDown.png",
		"darkSkyFront.png",
		"darkSkyBack.png"
	};

	std::vector<std::string> lake_skybox_faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	skyboxVec.push_back(galaxy_skybox_faces);
	skyboxVec.push_back(red_skybox_faces);
	skyboxVec.push_back(dark_skybox_faces);
	skyboxVec.push_back(lake_skybox_faces);

	env = new Skybox(1.0f, skyboxVec[0]);
	env1 = new Skybox(1.0f, skyboxVec[1]);
	env2 = new Skybox(1.0f, skyboxVec[2]);
	env3 = new Skybox(1.0f, skyboxVec[3]);

	sphere = new Geometry("sphere.obj", env->getTexture());
	head = new Geometry("head_s.obj", env->getTexture());
	antenna = new Geometry("antenna_s.obj", env->getTexture());
	body = new Geometry("body_s.obj", env->getTexture());
	wing = new Geometry("limb_s.obj", env->getTexture());
	
	glm::mat4 identity = glm::mat4(1.0f);
	glm::mat4 scaler = glm::mat4(1.0f);
	glm::mat4 wind1Scaler = glm::scale(glm::vec3(35, 2, 0.5));
	glm::mat4 wind2Scaler = glm::scale(glm::vec3(0.5, 2, 35));
	glm::mat4 antennaScaler = glm::scale(glm::vec3(1, 1.5, 1));
	glm::mat4 shipScaler = glm::scale(glm::vec3(0.3, 0.3, 0.3));
	// glm::mat4 ballScaler = glm::scale(glm::vec3(0.1, 0.1, 0.1));
	
	glm::mat4 head2Rot = glm::rotate(identity, glm::radians(180.0f), glm::vec3(0, 0, 1));
	glm::mat4 shipRot2 = glm::rotate(identity, glm::radians(90.0f), glm::vec3(-1, 0, 0));

	ship2world = new Transform(identity);
	spaceship = new Transform(identity * shipRot2 * shipScaler);
	body2ship = new Transform(identity);
	head2ship = new Transform(glm::translate(identity, glm::vec3(0, 1, 0)));
	head22ship = new Transform(glm::translate(identity, glm::vec3(0, -1, 0)) * head2Rot);
	wind12ship = new Transform(glm::translate(identity, glm::vec3(0, 0, 0)) * wind1Scaler);
	wind22ship = new Transform(glm::translate(identity, glm::vec3(0, 0, 0)) * wind2Scaler);
	antenna12ship = new Transform(glm::translate(identity, glm::vec3(-4.9, 1, 0)) * antennaScaler);
	antenna22ship = new Transform(glm::translate(identity, glm::vec3(4.9, 1, 0)) * antennaScaler);
	antenna32ship = new Transform(glm::translate(identity, glm::vec3(0, 1, -4.9)) * antennaScaler);
	antenna42ship = new Transform(glm::translate(identity, glm::vec3(0, 1, 4.9)) * antennaScaler);
	// ball2ship = new Transform(glm::translate(identity, eye));
	
	head2ship->addChild(head);
	head22ship->addChild(head);	
	body2ship->addChild(body);
	wind12ship->addChild(wing);
	wind22ship->addChild(wing);
	antenna12ship->addChild(antenna);
	antenna22ship->addChild(antenna);
	antenna32ship->addChild(antenna);
	antenna42ship->addChild(antenna);
	// ball2ship->addChild(sphere);

	body2ship->addChild(wind12ship);
	body2ship->addChild(wind22ship);
	body2ship->addChild(antenna12ship);
	body2ship->addChild(antenna22ship);
	body2ship->addChild(antenna32ship);
	body2ship->addChild(antenna42ship);

	spaceship->addChild(body2ship);
	spaceship->addChild(head2ship);
	spaceship->addChild(head22ship);
	// spaceship->addChild(ball2ship);

	ship2world->addChild(spaceship);
	return true;
}

void Window::cleanUp()
{	
	// Deallcoate the objects.
	// delete squad;
	delete env;
	// delete sphere;
	// delete sphere2World;
	// delete track;
	// Delete the shader program.
	glDeleteProgram(program);
	glDeleteProgram(skyboxProgram);
	glDeleteProgram(trackProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(fov), 
		double(width) / (double)height, near, far);
}

void Window::idleCallback()
{
	glm::vec3 position = glm::column(ship2world->getModel(), 3);
	// rotate tail
	body2ship->update(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0, -1, 0)));

	if (goForward == true)
	{
		// ship2world->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, following.z - position.z)));
		// eye.z = eye.z + (following.z - position.z);
		// center = center + (following - position);

		ship2world->update(glm::translate(glm::mat4(1.0f), 0.05f * glm::normalize(glm::vec3(position.x - eye.x, 0, position.z - eye.z))));
		center += 0.05f * glm::normalize(glm::vec3(position.x - eye.x, 0, position.z - eye.z));
		eye += 0.05f * glm::normalize(glm::vec3(position.x - eye.x, 0, position.z - eye.z));
	}

	if (goBackward == true)
	{
		// ship2world->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, position.z - following.z)));
		// eye.z = eye.z + (position.z - following.z);
		// center.z = center.z + (position.z - following.z);

		ship2world->update(glm::translate(glm::mat4(1.0f), 0.05f * glm::normalize(glm::vec3(eye.x - position.x, 0, eye.z - position.z))));
		center += 0.05f * glm::normalize(glm::vec3(eye.x - position.x, 0, eye.z - position.z));
		eye += 0.05f * glm::normalize(glm::vec3(eye.x - position.x, 0, eye.z - position.z));

	}


	if (turnL == true) {
		// body2ship->update(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0, -1, 0)));
		// body2ship->update(glm::rotate(glm::mat4(1.0f), glm::radians(0.1f), glm::vec3(0, 0, 1)));
		// spaceship->update(glm::translate(glm::mat4(1.0f), glm::vec3(-0.03, 0, 0)));

		spaceship->update(glm::rotate(glm::mat4(1.0f), glm::radians(0.3f), glm::vec3(0, 1, 0)));
		glm::vec3 camDir = eye - center;
		glm::vec4 camM = glm::vec4(camDir, 0.0f);
		float angle = glm::radians(0.3f);
		glm::vec3 axis = glm::vec3(0, 1, 0);
		glm::mat4 rotateM = glm::rotate(angle, axis);
		camM = rotateM * camM;
		camDir = glm::vec3(camM);
		eye = position + camDir;
		view = glm::lookAt(eye, position, up);
	}

	if (turnR == true) {
		// body2ship->update(glm::rotate(glm::mat4(1.0f), glm::radians(-3.0f), glm::vec3(0, -1, 0)));
		// body2ship->update(glm::rotate(glm::mat4(1.0f), glm::radians(-0.1f), glm::vec3(0, 0, 1)));
		// spaceship->update(glm::translate(glm::mat4(1.0f), glm::vec3(0.03, 0, 0)));

		spaceship->update(glm::rotate(glm::mat4(1.0f), glm::radians(-0.3f), glm::vec3(0, 1, 0)));
		glm::vec3 camDir = eye - center;
		glm::vec4 camM = glm::vec4(camDir, 0.0f);
		float angle = glm::radians(-0.3f);
		glm::vec3 axis = glm::vec3(0, 1, 0);
		glm::mat4 rotateM = glm::rotate(angle, axis);
		camM = rotateM * camM;
		camDir = glm::vec3(camM);
		eye = position + camDir;
		view = glm::lookAt(eye, position, up);
	}

	glm::mat4 identity = glm::mat4(1.0f);

	if (camView) {
		eye.y = position.y;
		eye.z = position.z + 0.5f;
		glm::vec3 c = center;
		c.z -= 10.0f;
		view = glm::lookAt(eye, c, up);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
	}
	else {
		view = glm::lookAt(eye, center, up);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	}
	
	/*
	timer++;
	if (timer % 60 == 0) {
		rotSpeed = rotSpeed * -1;
		floatSpeed = floatSpeed * -1;
	}

	glm::mat4 antennaRot = glm::rotate(glm::mat4(1.0f), 0.005f, glm::vec3(0, 1, 0));
	glm::mat4 armRotL = glm::rotate(glm::mat4(1.0f), rotSpeed, glm::vec3(0, 0, 1));
	glm::mat4 armRotR = glm::rotate(glm::mat4(1.0f), -rotSpeed, glm::vec3(0, 0, 1));
	glm::mat4 armsRot = glm::rotate(glm::mat4(1.0f), 0.005f, glm::vec3(0, 0, 1));

	glm::mat4 T1 = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0.2, 1));
	glm::mat4 inverseT1 = glm::translate(glm::mat4(1.0f), glm::vec3(1, -0.2, -1));

	glm::mat4 T2 = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0.2, 1));
	glm::mat4 inverseT2 = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -0.2, -1));

	glm::mat4 T3 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.3, 0.2, 0));
	glm::mat4 inverseT3 = glm::translate(glm::mat4(1.0f), glm::vec3(1.3, -0.2, 0));

	glm::mat4 T4 = glm::translate(glm::mat4(1.0f), glm::vec3(1.3, 0.2, 0));
	glm::mat4 inverseT4 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.3, -0.2, 0));

	glm::mat4 T5 = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0.2, -1));
	glm::mat4 inverseT5 = glm::translate(glm::mat4(1.0f), glm::vec3(1, -0.2, 1));

	glm::mat4 T6 = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0.2, -1));
	glm::mat4 inverseT6 = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -0.2, 1));
	
	
	glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1 / sqrt(2), 0, 1 / sqrt(2)));
	glm::mat4 inverseR1 = glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1 / sqrt(2), 0, 1 / sqrt(2)));

	glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(-1 / sqrt(2), 0, 1 / sqrt(2)));
	glm::mat4 inverseR2 = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(-1 / sqrt(2), 0, 1 / sqrt(2)));
	
	glm::mat4 R5 = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(-1 / sqrt(2), 0, 1 / sqrt(2)));
	glm::mat4 inverseR5 = glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(-1 / sqrt(2), 0, 1 / sqrt(2)));

	glm::mat4 R6 = glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1 / sqrt(2), 0, 1 / sqrt(2)));
	glm::mat4 inverseR6 = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1 / sqrt(2), 0, 1 / sqrt(2)));

	antenna22Robot->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)) * antennaRot);
	
	arm12Robot->update(T1 * R1 * armRotL * inverseR1 * inverseT1);
	arm22Robot->update(T2 * R2 * armRotR * inverseR2 * inverseT2);
	arm32Robot->update(T3 * armRotL * inverseT3);
	arm42Robot->update(T4 * armRotR * inverseT4);
	arm52Robot->update(T5 * R5 * armRotL * inverseR5 * inverseT5);
	arm62Robot->update(T6 * R6 * armRotR * inverseR6 * inverseT6);
	// arms2Robot->update(armsRot);

	head2Robot->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)));
	antenna12Robot->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)));
	eyeballL2Robot->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)));
	eyeballR2Robot->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)));
	*/
}

void Window::displayCallback(GLFWwindow* window)
{	
	/*
	if (culling) {
		int cullNumber = 0;

		farH = 2 * far * tan(glm::radians(fov) / 2);
		farW = width / height * farH;
		nearH = 2 * near * tan(glm::radians(fov) / 2);
		nearW = width / height * nearH;
		nearP = eye + glm::normalize(center - eye) * (float)near;
		farP = eye + glm::normalize(center - eye) * (float)far;

		glm::vec3 tempR = (nearP + right * (float)nearW / 2.0f) - eye;
		tempR = glm::normalize(tempR);

		glm::vec3 tempL = (nearP + (-right) * (float)nearW / 2.0f) - eye;
		tempL = glm::normalize(tempL);

		glm::vec3 localU = glm::cross(-right, eye - center);

		glm::vec3 tempU = (nearP + localU * (float)nearW / 2.0f) - eye;
		tempU = glm::normalize(tempU);

		glm::vec3 tempD = (nearP + (-localU) * (float)nearW / 2.0f) - eye;
		tempD = glm::normalize(tempD);

		// normals of the planes of the frustum
		normalR = glm::cross(up, tempR);
		normalL = glm::cross(tempL, up);
		normalU = glm::cross(tempU, right);
		normalD = glm::cross(right, tempD);
		normalN = glm::normalize(center - eye);
		normalF = glm::normalize(eye - center);

		// points on the planes of the frustum
		rightP = eye + 200.0f * tempR;
		leftP = eye + 200.0f * tempL;
		upP = eye + 200.0f * tempU;
		downP = eye + 200.0f * tempD;

		for (Node* child : squad->getChildren()) {
			glm::mat4 model = ((Transform*)child)->getModel();
			glm::vec3 x = glm::column(model, 3);
			float distanceR = glm::dot(x, normalR) - glm::dot(rightP, normalR);
			float distanceL = glm::dot(x, normalL) - glm::dot(leftP, normalL);
			float distanceU = glm::dot(x, normalU) - glm::dot(upP, normalU);
			float distanceD = glm::dot(x, normalD) - glm::dot(downP, normalD);
			float distanceN = glm::dot(x, normalN) - glm::dot(nearP, normalN);
			float distanceF = glm::dot(x, normalF) - glm::dot(farP, normalF);

			// near plane
			if (distanceN > 0.5) {
				showRobot = false;
			}
			else if (distanceN < -0.5) {
				showRobot = true;
			}
			else if (-0.5 < distanceN && distanceN < 0.5) {
				showRobot = true;
			}

			// far plane
			if (!showRobot) {
				if (distanceF > 0.5) {
					showRobot = false;
				}
				else if (distanceF < -0.5) {
					showRobot = true;
				}
				else if (-0.5 < distanceF && distanceF < 0.5) {
					showRobot = true;
				}
			}

			// right plane
			if (!showRobot) {
				if (distanceR > 0.5) {
					showRobot = false;
				}
				else if (distanceR < -0.5) {
					showRobot = true;
				}
				else if (-0.5 < distanceR && distanceR < 0.5) {
					showRobot = true;
				}
			}

			// left plane
			if (!showRobot) {
				if (distanceL > 0.5) {
					showRobot = false;
				}
				else if (distanceL < -0.5) {
					showRobot = true;
				}
				else if (-0.5 < distanceL && distanceL < 0.5) {
					showRobot = true;
				}
			}

			// up plane
			if (!showRobot) {
				if (distanceU > 0.5) {
					showRobot = false;
				}
				else if (distanceU < -0.5) {
					showRobot = true;
				}
				else if (-0.5 < distanceU && distanceU < 0.5) {
					showRobot = true;
				}
			}

			// down plane
			if (!showRobot) {
				if (distanceD > 0.5) {
					showRobot = false;
				}
				else if (distanceD < -0.5) {
					showRobot = true;
				}
				else if (-0.5 < distanceD && distanceD < 0.5) {
					showRobot = true;
				}
			}

			child->setShowRobot(!showRobot);

			if (!showRobot) {
				cullNumber++;
			}
			cullNum = cullNumber;
		}
	}*/

	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(eye));
	
	glm::mat4 identity = glm::mat4(1.0f);

	glUseProgram(program);
	ship2world->draw(program, identity);

	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));


	switch (planetNumber)
	{
	case 0:
		// draw the galaxy skybox
		head->setSkyboxTexture(env->getTexture());
		antenna->setSkyboxTexture(env->getTexture());
		wing->setSkyboxTexture(env->getTexture());
		body->setSkyboxTexture(env->getTexture());

		glUseProgram(program);
		ship2world->draw(program, identity);

		env->draw();
		break;

	case 1:
		head->setSkyboxTexture(env1->getTexture());
		antenna->setSkyboxTexture(env1->getTexture());
		wing->setSkyboxTexture(env1->getTexture());
		body->setSkyboxTexture(env1->getTexture());

		glUseProgram(program);
		ship2world->draw(program, identity);

		env1->draw();

		break;

	case 2:
		head->setSkyboxTexture(env2->getTexture());
		antenna->setSkyboxTexture(env2->getTexture());
		wing->setSkyboxTexture(env2->getTexture());
		body->setSkyboxTexture(env2->getTexture());

		glUseProgram(program);
		ship2world->draw(program, identity);

		env2->draw();

		break;

	case 3:
		head->setSkyboxTexture(env3->getTexture());
		antenna->setSkyboxTexture(env3->getTexture());
		wing->setSkyboxTexture(env3->getTexture());
		body->setSkyboxTexture(env3->getTexture());

		glUseProgram(program);
		ship2world->draw(program, identity);

		env3->draw();

		break;

	default:
		break;
	}


	// Render the objects.
	// squad->draw(program, identity);
	
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::vec3 position;
	int stateL = glfwGetKey(window, GLFW_KEY_LEFT);
	int stateR = glfwGetKey(window, GLFW_KEY_RIGHT);
	glm::mat4 shipScaler = glm::scale(glm::vec3(0.3, 0.3, 0.3));
	glm::mat4 shipRot2 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(-1, 0, 0));
	// Check for a key press.
	if (action == GLFW_PRESS)
	{	
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_C:
			camView = !camView;
			break;
		
		case GLFW_KEY_0:
			if (planetNumber != 0) {
				planetNumber = 0;

				// reset ship2world translation
				position = glm::column(ship2world->getModel(), 3);
				ship2world->update(glm::translate(glm::mat4(1.0f), -1.0f * position));

				// reset spaceship rotation
				glm::mat4 invM = glm::inverse(spaceship->getModel());
				spaceship->update(shipRot2 * shipScaler * invM);

				// reset camera
				eye = eyeVec;
				center = glm::vec3(0, 0, 0);
				view = glm::lookAt(eye, center, up);
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			}
			break;

		case GLFW_KEY_1:
			if (planetNumber != 1) {
				planetNumber = 1;

				// reset ship2world translation
				position = glm::column(ship2world->getModel(), 3);
				ship2world->update(glm::translate(glm::mat4(1.0f), -1.0f * position));

				// reset spaceship rotation
				glm::mat4 invM = glm::inverse(spaceship->getModel());
				spaceship->update(shipRot2 * shipScaler * invM);

				// reset camera
				eye = eyeVec;
				center = glm::vec3(0, 0, 0);
				view = glm::lookAt(eye, center, up);
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			}
			break;

		case GLFW_KEY_2:
			if (planetNumber != 2) {
				planetNumber = 2;

				// reset ship2world translation
				position = glm::column(ship2world->getModel(), 3);
				ship2world->update(glm::translate(glm::mat4(1.0f), -1.0f * position));

				// reset spaceship rotation
				glm::mat4 invM = glm::inverse(spaceship->getModel());
				spaceship->update(shipRot2 * shipScaler * invM);

				// reset camera
				eye = eyeVec;
				center = glm::vec3(0, 0, 0);
				view = glm::lookAt(eye, center, up);
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			}
			break;

		case GLFW_KEY_3:
			if (planetNumber != 3) {
				planetNumber = 3;

				// reset ship2world translation
				position = glm::column(ship2world->getModel(), 3);
				ship2world->update(glm::translate(glm::mat4(1.0f), -1.0f * position));

				// reset spaceship rotation
				glm::mat4 invM = glm::inverse(spaceship->getModel());
				spaceship->update(shipRot2 * shipScaler * invM);

				// reset camera
				eye = eyeVec;
				center = glm::vec3(0, 0, 0);
				view = glm::lookAt(eye, center, up);
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			}
			break;

		default:
			break;
		}
	}

	switch (key) 
	{
	case GLFW_KEY_LEFT:
		if (action == GLFW_REPEAT) {
			turnL = true;
		}
		else {
			turnL = false;
		}
		break;

	case GLFW_KEY_RIGHT:
		if (action == GLFW_REPEAT) {
			turnR = true;
		}
		else {
			turnR = false;
		}
		break;

	default:
		break;
	}

	switch (key)
	{
	case GLFW_KEY_UP:
		if (action == GLFW_REPEAT) {
			goForward = true;
		}
		else {
			goForward = false;
		}
		break;

	case GLFW_KEY_DOWN:
		if (action == GLFW_REPEAT) {
			goBackward = true;
		}
		else {
			goBackward = false;
		}

	default: 
		break;
	}
	
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (pressed) {
		glm::vec3 currentPos = trackBallMapping(glm::vec2(xpos, ypos));
		glm::vec3 camDir = eye - center;
		glm::vec4 camM = glm::vec4(camDir, 0.0f);
		glm::vec4 rightM = glm::vec4(camDir, 0.0f);
		float angle = acos(glm::dot(lastPos, currentPos) / glm::length(lastPos) * glm::length(currentPos));
		glm::vec3 axis = glm::cross(lastPos, currentPos);
		glm::mat4 rotateM = glm::rotate(angle, axis);
		camM = rotateM * camM;
		camDir = glm::vec3(camM);
		center = eye - camDir;
		view = glm::lookAt(eye, center, up);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		lastPos = currentPos;
		right = glm::vec3(rotateM * glm::vec4(right, 1.0f));
		// update window title
		std::string s = std::to_string(cullNum);
		char const* pchar = s.c_str();
		glfwSetWindowTitle(window, pchar);
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		pressed = true;
		glfwGetCursorPos(window, &xpos, &ypos);
		lastPos = trackBallMapping(glm::vec2(xpos, ypos));

		controlPtCount--;
		if (controlPtCount < 0) {
			controlPtCount = 2;
			curveCount--;
		}

		if (curveCount < 0) {
			curveCount = 7;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		controlPtCount++;
		if (controlPtCount > 2) {
			controlPtCount = 0;
			curveCount++;
		}

		if (curveCount > 7) {
			curveCount = 0;
		}
	}
	else if (action == GLFW_RELEASE) {
		pressed = false;
	}

	prevCount = curveCount - 1;
	nextCount = curveCount + 1;

	if (prevCount < 0) {
		prevCount = 7;
	}

	if (nextCount > 7) {
		nextCount = 0;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	if (fov >= 1.0f && fov <= maxFov)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= maxFov)
		fov = maxFov;

	projection = glm::perspective(glm::radians(fov), (double) width / (double) height, near, far);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	// update window title
	std::string s = std::to_string(cullNum);
	char const* pchar = s.c_str();
	glfwSetWindowTitle(window, pchar);
}

glm::vec3 Window::trackBallMapping(glm::vec2 point) {
	glm::vec3 v;
	float d;

	v.x = (2.0f * point.x - width) / width;
	v.y = (height - 2.0f * point.y) / width;
	v.z = 0.0f;

	d = glm::length(v);

	d = (d < 1.0f) ? d : 1.0f;
	v.z = sqrt(1.001f - d * d);

	v = glm::normalize(v);
	return v;
}
