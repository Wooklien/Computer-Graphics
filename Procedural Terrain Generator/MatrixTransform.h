#pragma once

#include "group.h"

class MatrixTransform : public group
{
public:
	MatrixTransform();
	MatrixTransform(Matrix4);

	void setTransformationMatrix(Matrix4);
	Matrix4 getTransformationMatrix();

	void draw(Matrix4, bool);
	void frustum_draw(Matrix4, bool);
private:
	Matrix4 myObject;
};

