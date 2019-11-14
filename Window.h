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

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"
#include "Node.h"
#include "Geometry.h"
#include "Transform.h"
#include "Skybox.h"
#include "BezierCurve.h"

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	static Geometry * head;
	static Geometry * antenna;
	static Geometry * eyeball;
	static Geometry * limb;
	static Geometry * sphere;
	static BezierCurve * track;
	static Transform * robot;
	static Transform * squad;
	static Transform * head2Robot;
	static Transform * antenna12Robot;
	static Transform * antenna22Robot;
	static Transform * eyeballL2Robot;
	static Transform * eyeballR2Robot;
	static Transform * arm12Robot;
	static Transform * arm22Robot;
	static Transform * arm32Robot;
	static Transform * arm42Robot;
	static Transform * arm52Robot;
	static Transform * arm62Robot;
	static Transform * sphere2Robot;
	static Transform * sphere2World;
	static Transform * env2World;
	static Transform * world;
	// static Transform* arms2Robot;

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

	static Skybox * env;

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
