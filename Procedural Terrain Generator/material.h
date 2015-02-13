#pragma once
#include <GL/glut.h>

using namespace std;

class material
{
public:
	GLenum model; // front and/or back

	material();
	material(GLenum);

	void setAmbient(GLfloat*);
	void setDiffuse(GLfloat*);
	void setAmbientAndDiffuse(GLfloat*);
	void setSpecular(GLfloat*);
	void setShininess(GLfloat*);
	void setEmission(GLfloat*);
	void setColorIndexes(GLfloat*);

};
