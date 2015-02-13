#include "material.h"

material::material() {
	model = GL_FRONT_AND_BACK;
}

material::material(GLenum m) {
	model = m;
}

void material::setAmbient(GLfloat *v) {
	glMaterialfv(model, GL_AMBIENT, v);
}

void material::setDiffuse(GLfloat *v) {
	glMaterialfv(model, GL_DIFFUSE, v);
}

void material::setAmbientAndDiffuse(GLfloat *v) {
	glMaterialfv(model, GL_AMBIENT_AND_DIFFUSE, v);
}

void material::setSpecular(GLfloat *v) {
	glMaterialfv(model, GL_SPECULAR, v);
}

void material::setShininess(GLfloat *v) {
	glMaterialfv(model, GL_SHININESS, v);
}

void material::setEmission(GLfloat *v) {
	glMaterialfv(model, GL_EMISSION, v);
}

void material::setColorIndexes(GLfloat *v) {
	glMaterialfv(model, GL_COLOR_INDEXES, v);
}