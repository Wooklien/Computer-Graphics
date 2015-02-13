#include "node.h"


node::node()
{
	name = "root";
	parent = NULL;
	boundingBox = Vector3(0, 0, 0);
	min = Vector3(INT_MAX, INT_MAX, INT_MAX);
	max = Vector3(INT_MIN, INT_MIN, INT_MIN);
	radius = 0.0;
}

node::node(string name) {
	this->name = name;
	parent = NULL;
}

bool node::hasParent() {
	if (parent == nullptr) {
		return false;
	}
	return true;
}

node * node::getParent() {
	return parent;
}

void node::setParent(node * n) {
	parent = n;
}

string node::getName() {
	return name;
}

void node::setName(string name) {
	this->name = name;
}

void node::setBoundingBox(Vector3 min, Vector3 max) {
	float x = (max.x - min.x)/2;
	float y = (max.y - min.y)/2;
	float z = (max.z - min.z)/2;

	boundingBox.set(x, y, z);
	radius = 0.5 * sqrt(x*x + y*y + z*z);
}
