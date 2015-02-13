#pragma once

#include <string>
#include "Matrix4.h"

using namespace std;

class node
{
public:
	node();
	node(string name);

	bool hasParent();
	node *getParent();
	void setParent(node *);

	string getName();
	void setName(string name);

	void setBoundingBox(Vector3, Vector3);

	virtual void draw(Matrix4 m, bool) = 0;
	virtual void frustum_draw(Matrix4 m, bool) = 0;

	Vector3 boundingBox;
	Vector3 min, max;
	float radius;
private:
	node *parent;
	string name;
};

