#include "Sphere.h"

void Sphere::render(Matrix4 m) {
	glPushMatrix();
	glLoadMatrixf(m.getGLMatrix());
	glutSolidSphere(1, 10, 10);
	glPopMatrix();
}
