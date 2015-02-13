#pragma once

#include "node.h"

class geode : public node
{
public:
	geode();
	geode(Matrix4);

	void setModeView(Matrix4);
	Matrix4 getModelView();

	void draw(Matrix4, bool);
	void frustum_draw(Matrix4, bool);
	virtual void render(Matrix4) = 0;
	void drawWireFrame(Matrix4, bool);
private:
	Matrix4 myObject;
	void getBounds(Matrix4);
};

