#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <glm/gtc/matrix_access.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Object.h"
// #include "Cube.h"
// #include "PointCloud.h"
#include "shader.h"
#include "Node.h"
#include "Geometry.h"
#include "Transform.h"
#include "Skybox.h"
// #include "BezierCurve.h"
// #include "Track.h"
#include "PerlinNoise.h"


class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	static Geometry* eyeball;
	//static Track * track;
	static Geometry* head;
	static Geometry* body;
	static Geometry* antenna;
	static Geometry* wing;
	static Geometry* sphere;
	static Transform * spaceship;
	static Transform * ship2world;
	static Transform * body2ship;
	static Transform * head2ship;
	static Transform * head22ship;
	static Transform * wind12ship;
	static Transform * wind22ship;
	static Transform * wind32ship;
	static Transform * wind42ship;
	static Transform * antenna12ship;
	static Transform * antenna22ship;
	static Transform * antenna32ship;
	static Transform * antenna42ship;
	static Transform* ball2ship;
	static bool turnL, turnR, goForward, goBackward;
	static float angle;

	// Andrew's alien
	static Transform * robotA;
	static Transform * squadA;
	static Transform * head2RobotA;
	static Transform * antenna12RobotA;
	static Transform * antenna22RobotA;
	static Transform * eyeballL2RobotA;
	static Transform * eyeballR2RobotA;
	static Transform * arm12RobotA;
	static Transform * arm22RobotA;
	static Transform * arm32RobotA;
	static Transform * arm42RobotA;
	static Transform * arm52RobotA;
	static Transform * arm62RobotA;
	static Transform * sphere2RobotA;
	static Transform * sphere2WorldA;

	// Jonathan's alien
	static Transform* squadJ;
	static Transform* transformLeftArm;
	static Transform* transformRightArm;
	static Transform* transformRightLeg;
	static Transform* transformLeftLeg;
	static Transform* transformHead;
	static Transform* transformLeftEye;
	static Transform* transformRightEye;
	static Transform* transformLeftAntenna;
	static Transform* transformRightAntenna;
	static Transform* transformGroupArmy;
	static Transform* transformBody;
	static glm::mat4 tLArmMatrix;
	static glm::mat4 tRArmMatrix;
	static glm::mat4 tLLegMatrix;
	static glm::mat4 tRLegMatrix;
	static glm::mat4 tHeadMatrix;
	static glm::mat4 tLAntenna;
	static glm::mat4 tRAntenna;
	static glm::mat4 tLEye;
	static glm::mat4 tREye;
	static bool rotateForward;
	static float rotAngle;

	// Daniel's alien
	static Transform* squadD;
	static Transform * robotD;
	static Transform * body2Bot;
	static Transform* head2Bot;
	static Transform* leftEye;
	static Transform* rightEye;
	static Transform* leftArm;
	static Transform* rightArm;
	static Transform* leftLeg;
	static Transform* rightLeg;
	static Transform* sprout;
	static Transform* leftLeaf;
	static Transform* rightLeaf;
	static bool forwardMotion;

	// static Transform * env2World;
	// static Transform * world;
	
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, skyboxProgram, trackProgram, projectionLoc, viewLoc, modelLoc, colorLoc, viewPosLoc;
	static glm::vec3 lastPos;
	static bool pressed;
	static int mode;
	static int timer;
	static float rotSpeed;
	static float floatSpeed;

	static double fov;
	static double maxFov;
	static double near;
	static double far;
	static double nearW;
	static double farW;
	static double nearH;
	static double farH;
	static glm::vec3 right;
	static glm::vec3 nc;
	static glm::vec3 fc;
	static glm::vec3 normalR;
	static glm::vec3 normalL;
	static glm::vec3 normalU;
	static glm::vec3 normalD;
	static glm::vec3 normalN;
	static glm::vec3 normalF;
	static glm::vec3 rightP;
	static glm::vec3 leftP;
	static glm::vec3 upP;
	static glm::vec3 downP;
	static glm::vec3 nearP;
	static glm::vec3 farP;
	static bool showRobot;
	static int cullNum;
	static bool culling;
	static float flying;

	static Skybox* env;
	static Skybox* env1;
	static Skybox* env2;
	static Skybox* env3;
	static int curveCount;
	static int prevCount;
	static int nextCount;
	static int controlPtCount;
	static int curveCountUpdate;
	static glm::vec3 lastPt;
	static bool pause;
	static double oldTime;
	static double newTime;
	static double leftover;
	static bool camView;

	/*
     * Planet Numbers:
	 *  0 : Galaxy
	 *  1 : Planet1 red sky
	 *  2 : Planet2 blue sky
	 *  3 : Planet3 Lake
	 */
	static int planetNumber;

	static std::vector<std::vector<std::string>> skyboxVec;

	static glm::vec3 eyeVec;

	static int cols1, cols2, cols3;
	static int rows1, rows2, rows3;
	static int scale1, scale2, scale3;
	static int terrainHeight;
	static int terrainWidth;
	static int terrainYValue;
	static int YTerrainMagnitude;
	static std::vector<std::vector<float>> terrainYVec1, terrainYVec2, terrainYVec3;
	static PerlinNoise pn;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackBallMapping(glm::vec2 point);
};

#endif