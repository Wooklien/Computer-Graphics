#pragma once
#include <vector>

#include <GL/glut.h>
#include "Vector3.h"

using namespace std;

class BezierSurface
{
public:
	void draw();
	void print();
	void Flag();
	Vector3 bernstein(float u, Vector3 *);
	Vector3 bernsteinNormal(float u, Vector3 *);
	Vector3 Calculate(float u, float v);
	Vector3 Calculate(float t, int row);
	Vector3 CalculateNormals(float, int);
	Vector3 CalculateNormals(float, float);
	GLuint patch;

protected:
	Vector3 ctrlpoints[4][4];

	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess[1];
};
