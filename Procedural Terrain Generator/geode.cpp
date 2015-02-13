#include "geode.h"


geode::geode()
{
	myObject = Matrix4();
	myObject.identity();
}

geode::geode(Matrix4 m) {
	myObject = m;
	myObject.identity();
}

void geode::setModeView(Matrix4 m) {
	myObject = m;
}

Matrix4 geode::getModelView() {
	return myObject;
}

void geode::draw(Matrix4 c, bool status) {
	getBounds(c);
	setModeView(c);
	render(myObject);
	drawWireFrame(c, status);
}

void geode::frustum_draw(Matrix4 c, bool status) {
	getBounds(c);
	drawWireFrame(c, status);
	float m[16], p[16];

	glGetFloatv(GL_PROJECTION_MATRIX, p);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	glPushMatrix();
	glLoadMatrixf(p);
	glMultMatrixf(m);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();

	Matrix4 m2w = Matrix4(
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	Vector4 NEARP = Vector4(
		m2w.get(2, 0) + m2w.get(3, 0),
		m2w.get(2, 1) + m2w.get(3, 1),
		m2w.get(2, 2) + m2w.get(3, 2),
		m2w.get(2, 3) + m2w.get(3, 3));

	Vector4 FARP = Vector4(
		-m2w.get(2, 0) + m2w.get(3, 0),
		-m2w.get(2, 1) + m2w.get(3, 1),
		-m2w.get(2, 2) + m2w.get(3, 2),
		-m2w.get(2, 3) + m2w.get(3, 3));

	Vector4 BOTTOM = Vector4(
		m2w.get(1, 0) + m2w.get(3, 0),
		m2w.get(1, 1) + m2w.get(3, 1),
		m2w.get(1, 2) + m2w.get(3, 2),
		m2w.get(1, 3) + m2w.get(3, 3));

	Vector4 TOP = Vector4(
		-m2w.get(1, 0) + m2w.get(3, 0),
		-m2w.get(1, 1) + m2w.get(3, 1),
		-m2w.get(1, 2) + m2w.get(3, 2),
		-m2w.get(1, 3) + m2w.get(3, 3));

	Vector4 LEFT = Vector4(
		m2w.get(0, 0) + m2w.get(3, 0),
		m2w.get(0, 1) + m2w.get(3, 1),
		m2w.get(0, 2) + m2w.get(3, 2),
		m2w.get(0, 3) + m2w.get(3, 3));

	Vector4 RIGHT = Vector4(
		-m2w.get(0, 0) + m2w.get(3, 0),
		-m2w.get(0, 1) + m2w.get(3, 1),
		-m2w.get(0, 2) + m2w.get(3, 2),
		-m2w.get(0, 3) + m2w.get(3, 3));

	NEARP.normalize();
	FARP.normalize();
	BOTTOM.normalize();
	TOP.normalize();
	LEFT.normalize();
	RIGHT.normalize();

	float distance;
	int count = 0;
	Vector4 bounding = { boundingBox.x, boundingBox.y, boundingBox.z, 1 };
	bounding = m2w.multiply(bounding);
	bounding.normalize();
	Vector4 plane[6] = { NEARP, FARP, BOTTOM, TOP, LEFT, RIGHT };
	for (int i = 0; i < 6; i++) {
		distance = plane[i][0] * bounding.x + plane[i][1] * bounding.y + plane[i][2] * bounding.z + plane[i][3];
		if (distance < -radius) {
			break;
		}
		else {
			count++;
		}
	}

	if (count == 6) {
		render(c);
	}
}

void geode::getBounds(Matrix4 m) {
	m.identity();
	m.scale(2, 2, 2);
	for (int i = 0; i < 4; i++) {
		if (m.get(0, i) < min.x) {
			min.x = m.get(0, i);
		}

		if (m.get(0, i) > max.x) {
			max.x = m.get(0, i);
		}

		if (m.get(1, i) < min.y) {
			min.y = m.get(1, i);
		}

		if (m.get(1, i) > max.y) {
			max.y = m.get(1, i);
		}

		if (m.get(2, i) < min.z) {
			min.z = m.get(2, i);
		}

		if (m.get(2, i) > max.z) {
			max.z = m.get(2, i);
		}
	}

	setBoundingBox(min, max);
}

void geode::drawWireFrame(Matrix4 c, bool status) {
	if (status) {
		glPushMatrix();
		glLoadMatrixf(c.getGLMatrix());
		glutWireSphere(radius, 10, 10);
		glPopMatrix();
	}
}

