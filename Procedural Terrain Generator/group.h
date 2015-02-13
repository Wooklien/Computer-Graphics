#pragma once

#include "node.h"
#include <vector>

class group : public node
{
public:
	group();

	bool hasChild();
	int getSize();
	void addChild(node *);
	bool removeChild(string name);
	vector<node*> getChildren();

	void draw(Matrix4, bool);
	void frustum_draw(Matrix4, bool);
private:
	vector<node *> child;
};

