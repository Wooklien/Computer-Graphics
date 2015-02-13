#include "group.h"

group::group()
{
	child.clear();
}

bool group::hasChild() {
	return (child.size() > 0) ? true : false;
}
int group::getSize() {
	return child.size();
}
void group::addChild(node * n) {
	child.push_back(n);
	n->setParent(this);
}

bool group::removeChild(string name) {
	for (vector<node *>::iterator iter = child.begin(); iter != child.end(); iter++) {
		if (name == (*iter)->getName()) {
			child.erase(iter);
			return true;
		}
	}
	return false;
}

vector<node*> group::getChildren() {
	return child;
}

void group::draw(Matrix4 c, bool status) {
	for (vector<node *>::iterator iter = child.begin(); iter != child.end(); iter++) {
		draw(c, status);
	}
}

void group::frustum_draw(Matrix4 c, bool status) {
	for (vector<node *>::iterator iter = child.begin(); iter != child.end(); iter++) {
		frustum_draw(c, status);
	}
}
