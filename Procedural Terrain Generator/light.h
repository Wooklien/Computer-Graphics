#pragma once
#include <GL/glut.h>

class light
{
public:
	GLenum source; // GL_LIGHT#

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];

	light();
	light(GLenum);

	void enable();
	void disable();

	void setAmbient(GLfloat*);
	void setDiffuse(GLfloat*);
	void setSpecular(GLfloat*);

	void setPosition(GLfloat*);
	void setSpotDirection(GLfloat*);
	void setSpotExponent(GLfloat);
	void setSpotCutoff(GLfloat);
};

