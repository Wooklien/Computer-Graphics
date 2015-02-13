#include "light.h"

light::light() {
	source = GL_LIGHT0;
}

light::light(GLenum src) {
	source = src;
}

void light::enable() {
	glEnable(source);

	glLightfv(source, GL_AMBIENT, ambient);
	glLightfv(source, GL_DIFFUSE, diffuse);
	glLightfv(source, GL_SPECULAR, specular);
}

void light::disable() {
	glDisable(source);

	GLfloat black[] = { 0, 0, 0, 0 };
	glLightfv(source, GL_DIFFUSE, black);
	glLightfv(source, GL_SPECULAR, black);
}

void light::setAmbient(GLfloat *v) {
	for (int i = 0; i<4; i++)
		ambient[i] = v[i];
	glLightfv(source, GL_AMBIENT, v);
}

void light::setDiffuse(GLfloat *v) {
	for (int i = 0; i<4; i++)
		diffuse[i] = v[i];
	glLightfv(source, GL_DIFFUSE, v);
}

void light::setSpecular(GLfloat *v) {
	for (int i = 0; i<4; i++)
		specular[i] = v[i];
	glLightfv(source, GL_SPECULAR, v);
}

void light::setPosition(GLfloat *v) {
	glLightfv(source, GL_POSITION, v);
}

void light::setSpotDirection(GLfloat *v) {
	glLightfv(source, GL_SPOT_DIRECTION, v);
}

void light::setSpotExponent(GLfloat n) {
	glLightf(source, GL_SPOT_EXPONENT, n);
}

void light::setSpotCutoff(GLfloat n) {
	glLightf(source, GL_SPOT_CUTOFF, n);
}
