#include "Tetrahedron.h"


void Tetrahedron::render(Matrix4 m) {
	glPushMatrix();
	glLoadMatrixf(m.getGLMatrix());
	glutSolidTetrahedron();
	glPopMatrix();
}
