#include "Cone.h"


void Cone::render(Matrix4 m) {
	glPushMatrix();
	glLoadMatrixf(m.getGLMatrix());
	glutSolidCone(1, 2, 10, 10);
	glPopMatrix();
}
