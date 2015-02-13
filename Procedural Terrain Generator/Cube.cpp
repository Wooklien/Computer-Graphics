#include "Cube.h"


void Cube::render(Matrix4 m) {
	glPushMatrix();
	glLoadMatrixf(m.getGLMatrix());
	glutSolidCube(1);
	glPopMatrix();
}
