#pragma once

#include "geode.h"
#include <GL/glut.h>

using namespace std;

class Sphere :
	public geode
{
public:
	void render(Matrix4);
};

