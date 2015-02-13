#include "MatrixTransform.h"

MatrixTransform::MatrixTransform()
{
	myObject = Matrix4();
	myObject.identity();
}

MatrixTransform::MatrixTransform(Matrix4 m) {
	myObject = m;
}

Matrix4 MatrixTransform::getTransformationMatrix() {
	return myObject;
}

void MatrixTransform::draw(Matrix4 c, bool status) {
	Matrix4 C_new = c.multiply(myObject);

	for (unsigned int i = 0; i < getChildren().size(); i++) {
		getChildren()[i]->draw(C_new, status);
	}
}

void MatrixTransform::frustum_draw(Matrix4 c, bool status) {
	Matrix4 C_new = c.multiply(myObject);

	for (unsigned int i = 0; i < getChildren().size(); i++) {
		getChildren()[i]->frustum_draw(C_new, status);
	}
}

void MatrixTransform::setTransformationMatrix(Matrix4 m) {
	myObject = m;
}
