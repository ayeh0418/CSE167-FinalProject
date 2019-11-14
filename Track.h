#ifndef _TRACK_H_
#define _TRACK_H_

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
#include "BezierCurve.h"
#include "Geometry.h"

class Track : public Geometry
{
private:
	std::vector<BezierCurve> curves;
public:
	Track();
	~Track();
};

#endif

