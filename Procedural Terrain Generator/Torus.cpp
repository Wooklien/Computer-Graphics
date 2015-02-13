#include "Torus.h"

void Torus::render(Matrix4 m) {
	glPushMatrix();
	glLoadMatrixf(m.getGLMatrix());
	glutSolidTorus(1, 2, 10, 10);
	glPopMatrix();
}
