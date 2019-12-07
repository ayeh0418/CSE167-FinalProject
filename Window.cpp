#include "Window.h"
#include "irrKlang-1.6.0/include/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice();
ISound* background;
ISound* pilot;
ISound* collect;

int Window::width;
int Window::height;

const char* Window::windowTitle = "Project 3";
Geometry * Window::eyeball;
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

// Andrew's alien
// Track * Window::track;
Transform * Window::robotA;
Transform * Window::squadA;
Transform* Window::head2RobotA;
Transform* Window::antenna12RobotA;
Transform* Window::antenna22RobotA;
Transform* Window::eyeballL2RobotA;
Transform* Window::eyeballR2RobotA;
Transform* Window::arm12RobotA;
Transform* Window::arm22RobotA;
Transform* Window::arm32RobotA;
Transform* Window::arm42RobotA;
Transform* Window::arm52RobotA;
Transform* Window::arm62RobotA;
Transform* Window::sphere2RobotA;
Transform* Window::sphere2WorldA;

// Jonathan's alien
Transform* Window::squadJ;
Transform* Window::transformLeftArm;
Transform* Window::transformRightArm;
Transform* Window::transformRightLeg;
Transform* Window::transformLeftLeg;
Transform* Window::transformHead;
Transform* Window::transformLeftEye;
Transform* Window::transformRightEye;
Transform* Window::transformLeftAntenna;
Transform* Window::transformRightAntenna;
Transform* Window::transformGroupArmy;
Transform* Window::transformBody;
glm::mat4 Window::tLArmMatrix;
glm::mat4 Window::tRArmMatrix;
glm::mat4 Window::tLLegMatrix;
glm::mat4 Window::tRLegMatrix;
glm::mat4 Window::tHeadMatrix;
glm::mat4 Window::tLAntenna;
glm::mat4 Window::tRAntenna;
glm::mat4 Window::tLEye;
glm::mat4 Window::tREye;
bool Window::rotateForward = true;
float Window::rotAngle = 0.25f;

// Daniel's alien
Transform* Window::squadD;
Transform* Window::robotD;
Transform* Window::body2Bot;
Transform* Window::head2Bot;
Transform* Window::leftEye;
Transform* Window::rightEye;
Transform* Window::leftArm;
Transform* Window::rightArm;
Transform* Window::leftLeg;
Transform* Window::rightLeg;
Transform* Window::sprout;
Transform* Window::leftLeaf;
Transform* Window::rightLeaf;
bool Window::forwardMotion = true;

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

int Window::cols;
int Window::rows;
int Window::scale;
int Window::terrainHeight;
int Window::terrainWidth;
int Window::terrainYValue;
int Window::YTerrainMagnitude;
std::vector<std::vector<float>> Window::terrainYVec;
PerlinNoise Window::pn;

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

	if (!trackProgram)
	{
		std::cerr << "Failed to initialize track shader program" << std::endl;
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

	background = SoundEngine->play3D("breakout.mp3", vec3df(0, 0, 0), true, false, true);
	pilot = SoundEngine->play3D("pilot.mp3", vec3df(0, 0, 0), true, false, true);
	pilot->setIsPaused(true); // pause the sound

	return true;
}

bool Window::initializeObjects()
{
	// On initialize, we begin in galaxy.
	planetNumber = 0;
	srand(time(NULL)); // random seed
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
	eyeball = new Geometry("eyeball_s.obj", env->getTexture());
	
	glm::mat4 identity = glm::mat4(1.0f);
	glm::mat4 scaler = glm::mat4(1.0f);
	glm::mat4 wind1Scaler = glm::scale(glm::vec3(35, 2, 0.5));
	glm::mat4 wind2Scaler = glm::scale(glm::vec3(0.5, 2, 35));
	glm::mat4 antennaScaler = glm::scale(glm::vec3(1, 1.5, 1));
	glm::mat4 shipScaler = glm::scale(glm::vec3(0.3, 0.3, 0.3));
	glm::mat4 ballScaler = glm::scale(glm::vec3(7, 7, 7));
	
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
	ball2ship = new Transform(ballScaler);
	
	head2ship->addChild(head);
	head22ship->addChild(head);	
	body2ship->addChild(body);
	wind12ship->addChild(wing);
	wind22ship->addChild(wing);
	antenna12ship->addChild(antenna);
	antenna22ship->addChild(antenna);
	antenna32ship->addChild(antenna);
	antenna42ship->addChild(antenna);
	ball2ship->addChild(sphere);

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

	// Andrew's aliens
	glm::mat4 antennaScalerA = glm::scale(glm::vec3(0.35, 0.3, 0.35));
	glm::mat4 antennaScaler2 = glm::scale(glm::vec3(0.35, 0.15, 0.35));
	glm::mat4 eyeScaler = glm::scale(glm::vec3(2.0, 2.0, 2.0));
	glm::mat4 sphereScaler = glm::scale(glm::vec3(2.5, 2.5, 2.5));
	glm::mat4 antennaRot = glm::rotate(identity, -70.0f, glm::vec3(0, 0, 1));
	glm::mat4 arm1Rot = glm::rotate(identity, 90.0f, glm::vec3(1 / sqrt(2), 0, 1 / sqrt(2)));
	glm::mat4 arm2Rot = glm::rotate(identity, -90.0f, glm::vec3(-1 / sqrt(2), 0, 1 / sqrt(2)));
	glm::mat4 arm3Rot = glm::rotate(identity, 90.0f, glm::vec3(0, 0, 1));
	glm::mat4 arm4Rot = glm::rotate(identity, -90.0f, glm::vec3(0, 0, 1));
	glm::mat4 arm5Rot = glm::rotate(identity, 90.0f, glm::vec3(-1 / sqrt(2), 0, 1 / sqrt(2)));
	glm::mat4 arm6Rot = glm::rotate(identity, -90.0f, glm::vec3(1 / sqrt(2), 0, 1 / sqrt(2)));

	robotA = new Transform(identity);
	squadA = new Transform(identity);
	// arms2Robot = new Transform(identity);
	head2RobotA = new Transform(glm::translate(identity, glm::vec3(0, 0.5, 0)));
	antenna12RobotA = new Transform(glm::translate(identity, glm::vec3(0, 1.35, 0)) * antennaScalerA);
	antenna22RobotA = new Transform(glm::translate(identity, glm::vec3(0.25, 2.33, 0)) * antennaRot * antennaScaler2);
	eyeballL2RobotA = new Transform(glm::translate(identity, glm::vec3(-0.25, 0.9, 0.85)));
	eyeballR2RobotA = new Transform(glm::translate(identity, glm::vec3(0.25, 0.9, 0.85)));
	arm12RobotA = new Transform(glm::translate(identity, glm::vec3(-1, 0.2, 1)) * arm1Rot);
	arm22RobotA = new Transform(glm::translate(identity, glm::vec3(1, 0.2, 1)) * arm2Rot);
	arm32RobotA = new Transform(glm::translate(identity, glm::vec3(-1.3, 0.2, 0)) * arm3Rot);
	arm42RobotA = new Transform(glm::translate(identity, glm::vec3(1.3, 0.2, 0)) * arm4Rot);
	arm52RobotA = new Transform(glm::translate(identity, glm::vec3(-1, 0.2, -1)) * arm5Rot);
	arm62RobotA = new Transform(glm::translate(identity, glm::vec3(1, 0.2, -1)) * arm6Rot);
	sphere2RobotA = new Transform(glm::translate(identity, glm::vec3(0, 0.2, 0)) * sphereScaler);

	robotA->addChild(head2RobotA);
	robotA->addChild(antenna12RobotA);
	robotA->addChild(antenna22RobotA);
	robotA->addChild(eyeballL2RobotA);
	robotA->addChild(eyeballR2RobotA);
	robotA->addChild(arm12RobotA);
	robotA->addChild(arm22RobotA);
	robotA->addChild(arm32RobotA);
	robotA->addChild(arm42RobotA);
	robotA->addChild(arm52RobotA);
	robotA->addChild(arm62RobotA);
	// robotA->addChild(sphere2RobotA);

	head2RobotA->addChild(head);
	antenna12RobotA->addChild(antenna);
	antenna22RobotA->addChild(antenna);
	eyeballL2RobotA->addChild(eyeball);
	eyeballR2RobotA->addChild(eyeball);
	arm12RobotA->addChild(wing);
	arm22RobotA->addChild(wing);
	arm32RobotA->addChild(wing);
	arm42RobotA->addChild(wing);
	arm52RobotA->addChild(wing);
	arm62RobotA->addChild(wing);
	sphere2RobotA->addChild(sphere);

	for (int i = 0; i < 10; i++) {
		float randX = rand() % 20 -10;
		float randY = rand() % 4 -2;
		float randZ = rand() % 30 - 30;
		Transform* newRobot = new Transform(glm::translate(identity, glm::vec3(randX, randY, randZ)));
		squadA->addChild(newRobot);
		newRobot->addChild(robotA);
	}

	// Jonathan's alien
	squadJ = new Transform(identity);
	transformGroupArmy = new Transform(identity);
	transformBody = new Transform(identity);

	tLArmMatrix = glm::mat4(1.0f);
	tLArmMatrix = glm::translate(tLArmMatrix, glm::vec3(-1.25f, 0.0f, 0.0f));
	tLArmMatrix = glm::scale(tLArmMatrix, glm::vec3(1.25f, 1.25f, 1.25f));
	transformLeftArm = new Transform(tLArmMatrix);

	tRArmMatrix = glm::mat4(1.0f);
	tRArmMatrix = glm::translate(tRArmMatrix, glm::vec3(1.25f, 0.0f, 0.0f));
	tRArmMatrix = glm::scale(tRArmMatrix, glm::vec3(1.25f, 1.25f, 1.25f));
	transformRightArm = new Transform(tRArmMatrix);

	tLLegMatrix = glm::mat4(1.0f);
	tLLegMatrix = glm::translate(tLLegMatrix, glm::vec3(-0.6f, -1.5f, 0.0f));
	tLLegMatrix = glm::scale(tLLegMatrix, glm::vec3(1.25f, 1.0f, 1.25f));
	transformLeftLeg = new Transform(tLLegMatrix);

	tRLegMatrix = glm::mat4(1.0f);
	tRLegMatrix = glm::translate(tRLegMatrix, glm::vec3(0.6f, -1.5f, 0.0f));
	tRLegMatrix = glm::scale(tRLegMatrix, glm::vec3(1.25f, 1.0f, 1.25f));
	transformRightLeg = new Transform(tRLegMatrix);

	tHeadMatrix = glm::mat4(1.0f);
	tHeadMatrix = glm::translate(tHeadMatrix, glm::vec3(0.0f, 1.1f, 0.0f));
	transformHead = new Transform(tHeadMatrix);

	tLEye = glm::mat4(1.0f);
	tLEye = glm::translate(tLEye, glm::vec3(-0.4f, 0.4f, 0.8f));
	tLEye = glm::scale(tLEye, glm::vec3(1.5f, 1.5f, 1.5f));
	transformLeftEye = new Transform(tLEye);

	tREye = glm::mat4(1.0f);
	tREye = glm::translate(tREye, glm::vec3(0.4f, 0.4f, 0.8f));
	tREye = glm::scale(tREye, glm::vec3(1.5f, 1.5f, 1.5f));
	transformRightEye = new Transform(tREye);

	tLAntenna = glm::mat4(1.0f);
	tLAntenna = glm::translate(tLAntenna, glm::vec3(-0.4f, 1.0f, 0.0f));
	tLAntenna = glm::scale(tLAntenna, glm::vec3(0.75f, 0.1f, 0.2f));
	tLAntenna = glm::rotate(tLAntenna, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transformLeftAntenna = new Transform(tLAntenna);

	tRAntenna = glm::mat4(1.0f);
	tRAntenna = glm::translate(tRAntenna, glm::vec3(0.4f, 1.0f, 0.0f));
	tRAntenna = glm::scale(tRAntenna, glm::vec3(0.75f, 0.1f, 0.2f));
	tRAntenna = glm::rotate(tRAntenna, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	transformRightAntenna = new Transform(tRAntenna);

	transformBody->addChild(body);

	transformBody->addChild(transformLeftArm);
	transformLeftArm->addChild(wing);

	transformBody->addChild(transformRightArm);
	transformRightArm->addChild(wing);

	transformBody->addChild(transformLeftLeg);
	transformLeftLeg->addChild(wing);

	transformBody->addChild(transformRightLeg);
	transformRightLeg->addChild(wing);

	transformBody->addChild(transformHead);
	transformHead->addChild(head);

	transformHead->addChild(transformLeftEye);
	transformLeftEye->addChild(eyeball);

	transformHead->addChild(transformRightEye);
	transformRightEye->addChild(eyeball);

	transformHead->addChild(transformLeftAntenna);
	transformLeftAntenna->addChild(antenna);

	transformHead->addChild(transformRightAntenna);
	transformRightAntenna->addChild(antenna);

	for (int j = 0; j < 10; j++) {
		float randX = rand() % 30 - 15;
		float randZ = rand() % 30 - 30;
		Transform* newRobot = new Transform(glm::translate(identity, glm::vec3(randX, 0, randZ)));
		squadJ->addChild(newRobot);
		newRobot->addChild(transformBody);
	}


	// Daniel's alien
	squadD = new Transform(identity);
	glm::mat4 robotScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.9, 0.9, 0.9));
	glm::mat4 eyeScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.45, 0.45, 0.45));
	glm::mat4 headBodyMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8, 0.8, 0.8));
	glm::mat4 antennaScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2));
	glm::mat4 sproutScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2));
	robotD = new Transform(identity);
	body2Bot = new Transform(identity * headBodyMatrix);
	head2Bot = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.9, 0)) * headBodyMatrix);
	leftEye = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.2, 1.3, 0.65)) * eyeScaleMatrix);
	rightEye = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.2, 1.3, 0.65)) * eyeScaleMatrix);
	leftArm = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0)) * headBodyMatrix);
	rightArm = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(1, 0, 0)) * headBodyMatrix);
	leftLeg = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.7, -1.3, 0)) * headBodyMatrix);
	rightLeg = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.7, -1.3, 0)) * headBodyMatrix);
	sprout = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.7, 0)) * antennaScaleMatrix);
	leftLeaf = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.1, 2.1, 0)) );
	rightLeaf = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.1, 2.1, 0)) );
	robotD->addChild(body2Bot);
	robotD->addChild(head2Bot);
	robotD->addChild(leftEye);
	robotD->addChild(rightEye);
	robotD->addChild(leftArm);
	robotD->addChild(rightArm);
	robotD->addChild(leftLeg);
	robotD->addChild(rightLeg);
	robotD->addChild(sprout);
	robotD->addChild(leftLeaf);
	robotD->addChild(rightLeaf);

	body2Bot->addChild(body);
	// body2Bot->addChild(sphere);
	head2Bot->addChild(head);
	leftEye->addChild(eyeball);
	rightEye->addChild(eyeball);
	leftArm->addChild(wing);
	rightArm->addChild(wing);
	leftLeg->addChild(wing);
	rightLeg->addChild(wing);
	sprout->addChild(antenna);
	leftLeaf->addChild(eyeball);
	rightLeaf->addChild(eyeball);

	for (int k = 0; k < 10; k++) {
		float randX = rand() % 30 - 15;
		float randZ = rand() % 30 - 30;
		Transform* newRobot = new Transform(glm::translate(identity, glm::vec3(randX, 0, randZ)));
		squadD->addChild(newRobot);
		newRobot->addChild(robotD);
	}

	//TODO: May need to update this
	scale = 2;
	terrainWidth = 300;
	terrainHeight = 300;
	cols = terrainWidth / scale;
	rows = terrainHeight / scale;
	terrainYValue = -20;
	YTerrainMagnitude = 10;
	unsigned int seed = 227;
	pn = PerlinNoise(seed);

	//initialize terrainYValue
	for (int z = 0; z < rows; z++)
	{
		std::vector<float> v1;
		for (int x = 0; x < cols; x++)
		{
			float noise = pn.noise(x, 0.2, z);
			//float randomYNum = rand() % YTerrainMagnitude + (terrainYValue - YTerrainMagnitude / 2);

			float min1 = 0;
			float max1 = 1;
			float min2 = terrainYValue - (YTerrainMagnitude / 2);
			float max2 = terrainYValue + (YTerrainMagnitude / 2);

			float randomYNum = (noise - min1) / (max1 - min1) * (max2 - min2) + min2;

			v1.push_back(randomYNum);
		}
		terrainYVec.push_back(v1);
	}
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
	glm::mat4 identity = glm::mat4(1.0f);
	glm::vec3 position = glm::column(ship2world->getModel(), 3);
	body2ship->update(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0, -1, 0))); // rotate wings

	if (goForward == true)
	{
		center = position;
		ship2world->update(glm::translate(glm::mat4(1.0f), 0.07f * glm::normalize(glm::vec3(position.x - eye.x, 0, position.z - eye.z))));
		center += 0.07f * glm::normalize(glm::vec3(position.x - eye.x, 0, position.z - eye.z));
		eye += 0.07f * glm::normalize(glm::vec3(position.x - eye.x, 0, position.z - eye.z));
	}

	if (goBackward == true)
	{
		center = position;
		ship2world->update(glm::translate(glm::mat4(1.0f), 0.07f * glm::normalize(glm::vec3(eye.x - position.x, 0, eye.z - position.z))));
		center += 0.07f * glm::normalize(glm::vec3(eye.x - position.x, 0, eye.z - position.z));
		eye += 0.07f * glm::normalize(glm::vec3(eye.x - position.x, 0, eye.z - position.z));
	}

	if (turnL == true) {
		center = position;
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
		center = position;
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

	// switch between cam views
	if (camView) {
		eye = position + 0.5f * glm::normalize(eye - position);
		eye.y = position.y;
		view = glm::lookAt(eye, position, up);

		// center = position + 1.0f * glm::normalize(position - eye);
		// std::cout << "c: " << center.z << " p: " << position.z << " e: " << eye.z << std::endl;
		// eye = position;
		//  view = glm::lookAt(eye, center, up);
	}
	else {
		eye = position + 10.0f * glm::normalize(eye - position);
		eye.y = eyeVec.y;
		view = glm::lookAt(eye, center, up);
	}
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


	// Andrew's alien
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

	antenna22RobotA->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)) * antennaRot);
	
	arm12RobotA->update(T1 * R1 * armRotL * inverseR1 * inverseT1);
	arm22RobotA->update(T2 * R2 * armRotR * inverseR2 * inverseT2);
	arm32RobotA->update(T3 * armRotL * inverseT3);
	arm42RobotA->update(T4 * armRotR * inverseT4);
	arm52RobotA->update(T5 * R5 * armRotL * inverseR5 * inverseT5);
	arm62RobotA->update(T6 * R6 * armRotR * inverseR6 * inverseT6);

	head2RobotA->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)));
	antenna12RobotA->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)));
	eyeballL2RobotA->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)));
	eyeballR2RobotA->update(glm::translate(glm::mat4(1.0f), glm::vec3(0, floatSpeed, 0)));


	// Jonathan's alien
	if (timer == 200)
	{
		if (rotateForward == true)
		{
			//start from -1000 because you need to go back all the way pass "middle"
			timer = -200;
			rotateForward = false;
		}
		else
		{
			timer = -200;
			rotateForward = true;
		}
		//rotAngle = -rotAngle;
	}

	if (rotateForward == true)
	{
		transformLeftArm->update(glm::rotate(glm::radians(rotAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
		transformRightArm->update(glm::rotate(glm::radians(-rotAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
		transformRightLeg->update(glm::rotate(glm::radians(rotAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
		transformLeftLeg->update(glm::rotate(glm::radians(-rotAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else
	{
		transformLeftArm->update(glm::rotate(glm::radians(-rotAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
		transformRightArm->update(glm::rotate(glm::radians(rotAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
		transformRightLeg->update(glm::rotate(glm::radians(-rotAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
		transformLeftLeg->update(glm::rotate(glm::radians(rotAngle), glm::vec3(1.0f, 0.0f, 0.0f)));
	}

	
	// Daniel's alien
	if (timer % 450 == 0) {
		if (forwardMotion) {
			forwardMotion = false;
		}
		else {
			forwardMotion = true;
		}
	}
	if (forwardMotion) {
		rightArm->update(glm::rotate(glm::radians(0.1f), glm::vec3(1, 0, 0)));
		leftArm->update(glm::rotate(glm::radians(-0.1f), glm::vec3(1, 0, 0)));
		rightLeg->update(glm::rotate(glm::radians(-0.1f), glm::vec3(1, 0, 0)));
		leftLeg->update(glm::rotate(glm::radians(0.1f), glm::vec3(1, 0, 0)));
	}
	else {
		rightArm->update(glm::rotate(glm::radians(-0.1f), glm::vec3(1, 0, 0)));
		leftArm->update(glm::rotate(glm::radians(0.1f), glm::vec3(1, 0, 0)));
		rightLeg->update(glm::rotate(glm::radians(0.1f), glm::vec3(1, 0, 0)));
		leftLeg->update(glm::rotate(glm::radians(-0.1f), glm::vec3(1, 0, 0)));
	}
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
	for (Node* child : squadA->getChildren()) {
		glm::mat4 model = ((Transform*)child)->getModel();
		glm::vec3 x = glm::column(model, 3);
		glm::vec3 ship = glm::column(ship2world->getModel(), 3);

		if (glm::distance(x, ship) <= 3) {
			if (child->getShowRobot()) {
				collect = SoundEngine->play3D("collect.mp3", vec3df(0, 0, 0), false, false, true);
				child->setShowRobot(false);
			}
		}
	}
	
	for (Node* child : squadJ->getChildren()) {
		glm::mat4 model = ((Transform*)child)->getModel();
		glm::vec3 x = glm::column(model, 3);
		glm::vec3 ship = glm::column(ship2world->getModel(), 3);

		if (glm::distance(x, ship) <= 3) {
			if (child->getShowRobot()) {
				collect = SoundEngine->play3D("collect.mp3", vec3df(0, 0, 0), false, false, true);
				child->setShowRobot(false);
			}
		}
	}
	
	for (Node* child : squadD->getChildren()) {
		glm::mat4 model = ((Transform*)child)->getModel();
		glm::vec3 x = glm::column(model, 3);
		glm::vec3 ship = glm::column(ship2world->getModel(), 3);

		if (glm::distance(x, ship) <= 3) {
			if (child->getShowRobot()) {
				collect = SoundEngine->play3D("collect.mp3", vec3df(0, 0, 0), false, false, true);
				child->setShowRobot(false);
			}
		}
	}

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

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 topLeft = glm::vec3(-(cols / 2) * scale, terrainYValue, -(rows / 2) * scale);


	switch (planetNumber)
	{
	case 0:
		// draw the galaxy skybox
		head->setSkyboxTexture(env->getTexture());
		antenna->setSkyboxTexture(env->getTexture());
		wing->setSkyboxTexture(env->getTexture());
		body->setSkyboxTexture(env->getTexture());

		env->draw();
		break;

	case 1:
		head->setSkyboxTexture(env1->getTexture());
		antenna->setSkyboxTexture(env1->getTexture());
		wing->setSkyboxTexture(env1->getTexture());
		body->setSkyboxTexture(env1->getTexture());

		glUseProgram(trackProgram);
		glUniformMatrix4fv(glGetUniformLocation(trackProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(trackProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(trackProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniform3fv(glGetUniformLocation(trackProgram, "color"), 1, glm::value_ptr(color));

		//glBegin(GL_LINE_STRIP);
	
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);

		for (int z = 0; z < rows - 1; z++)
		{
			glBegin(GL_TRIANGLE_STRIP);

			for (int x = 0; x < cols; x++)
			{
				glVertex3f(topLeft.x + (x * scale), terrainYVec[x][z], topLeft.z + (z * scale));
				glVertex3f(topLeft.x + (x * scale), terrainYVec[x][z + 1], topLeft.z + ((z + 1) * scale));

			}
			glEnd();

		}
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);

		/*glBegin(GL_LINES);
		for (int i = -10; i <= 10; i++)
		{
			glVertex3f((float)i, 0, (float)-10);
			glVertex3f((float)i, 0, (float)10);

			glVertex3f((float)-10, 0, (float)i);
			glVertex3f((float)10, 0, (float)i);
		}
		glEnd();*/

		glUseProgram(program);
		ship2world->draw(program, identity);
		squadJ->draw(program, identity);

		env1->draw();

		break;

	case 2:
		head->setSkyboxTexture(env2->getTexture());
		antenna->setSkyboxTexture(env2->getTexture());
		wing->setSkyboxTexture(env2->getTexture());
		body->setSkyboxTexture(env2->getTexture());

		squadA->draw(program, identity);
		env2->draw();
		break;

	case 3:
		head->setSkyboxTexture(env3->getTexture());
		antenna->setSkyboxTexture(env3->getTexture());
		wing->setSkyboxTexture(env3->getTexture());
		body->setSkyboxTexture(env3->getTexture());

		squadD->draw(program, identity);
		env3->draw();
		break;

	default:
		break;
	}
	
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

			if (camView) {
				// pilot = SoundEngine->play2D("pilot.mp3", GL_TRUE);
				// background->stop();
				// background->drop();
				background->setIsPaused(true); // pause the sound
				pilot->setIsPaused(false); // unpause the sound
			}
			else {
				// background = SoundEngine->play2D("breakout.mp3", GL_TRUE);
				// pilot->stop();
				// pilot->drop();
				background->setIsPaused(false); // unpause the sound
				pilot->setIsPaused(true); // pause the sound
			}
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
	
	// keyboard control directions
	switch (key) 
	{
	case GLFW_KEY_LEFT:
		if (action == GLFW_RELEASE) {
			turnL = false;
		}
		else {
			turnL = true;
		}
		break;

	case GLFW_KEY_RIGHT:
		if (action == GLFW_RELEASE) {
			turnR = false;
		}
		else {
			turnR = true;
		}
		break;
		
	case GLFW_KEY_UP:
		if (action == GLFW_RELEASE) {
			goForward = false;
		}
		else {
			goForward = true;
		}
		break;
		
	case GLFW_KEY_DOWN:
		if (action == GLFW_RELEASE) {
			goBackward = false;
		}
		else {
			goBackward = true;
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
