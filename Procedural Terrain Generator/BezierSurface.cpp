#include "BezierSurface.h"

int LOD = 20;
void BezierSurface::draw() {
	int i, j;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// Use The Good Calculations
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);
	glEnable(GL_COLOR_MATERIAL);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (i = 0; i<LOD; i++) {
		float u = (float)i / (LOD - 1);
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < LOD; j++) {
			float v = (float)j / (LOD - 1);
			Vector3 p = Calculate(u, v);
			Vector3 n = CalculateNormals(u, v);
			glNormal3f(n.x, n.y, n.z);
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
	}

	for (i = 0; i<LOD; i++) {
		float u = (float)i / (LOD - 1);
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < LOD; j++) {
			float v = (float)j / (LOD - 1);
			Vector3 p = Calculate(v, u);
			Vector3 n = CalculateNormals(v, u);
			glNormal3f(n.x, n.y, n.z);
			glVertex3f(p.x, p.y, p.z);
			//glVertex3d(ctrlpoints[j][i].x, ctrlpoints[j][i].y, ctrlpoints[j][i].z);
		}
		glEnd();
	}
	glDisable(GL_COLOR_MATERIAL);
}

void BezierSurface::Flag() {
	ctrlpoints[0][0] = Vector3(-1.5, -1.5, 4.0);	// Set The Bezier Vertices
	ctrlpoints[0][1] = Vector3(-0.5, -1.5, 2.0);
	ctrlpoints[0][2] = Vector3(0.5, -1.5, -1.0);
	ctrlpoints[0][3] = Vector3(1.5, -1.5, 2.0);
	ctrlpoints[1][0] = Vector3(-1.5, -0.5, 1.0);
	ctrlpoints[1][1] = Vector3(-0.5, -0.5, 3.0);
	ctrlpoints[1][2] = Vector3(0.5, -0.5, 0.0);
	ctrlpoints[1][3] = Vector3(1.5, -0.5, -1.0);
	ctrlpoints[2][0] = Vector3(-1.5, 0.5, 4.0);
	ctrlpoints[2][1] = Vector3(-0.5, 0.5, 0.0);
	ctrlpoints[2][2] = Vector3(0.5, 0.5, 3.0);
	ctrlpoints[2][3] = Vector3(1.5, 0.5, 4.0);
	ctrlpoints[3][0] = Vector3(-1.5, 1.5, -2.0);
	ctrlpoints[3][1] = Vector3(-0.5, 1.5, -2.0);
	ctrlpoints[3][2] = Vector3(0.5, 1.5, 0.0);
	ctrlpoints[3][3] = Vector3(1.5, 1.5, -1.0);

	ambient[0] = .3f; ambient[1] = .3f; ambient[2] = .3f; ambient[3] = 1.0f;
	diffuse[0] = .5f; diffuse[1] = .5f; diffuse[2] = .5f; diffuse[3] = 1.0f;
	specular[0] = .5; specular[1] = .5; specular[2] = .5; specular[3] = 1.0f;
	shininess[0] = 100;
}

Vector3 BezierSurface::bernstein(float u, Vector3 *p) {
	Vector3	a, b, c, d, r;

	float it = 1.0f - u;
	float b0 = u*u*u;
	float b1 = 3 * u*u*it;
	float b2 = 3 * u*it*it;
	float b3 = it*it*it;

	a.scale(b0, p[0]);
	b.scale(b1, p[1]);
	c.scale(b2, p[2]);
	d.scale(b3, p[3]);

	r.add(a + b, c + d);

	return r;
}

Vector3 BezierSurface::bernsteinNormal(float u, Vector3 *p) {
	Vector3	a, b, c, d, r;

	float it = 1.0f - u;

	float b0 = 3 * u*u;
	float b1 = 6 * u*it;
	float b2 = 3 * it*it;
	float b3 = 0;

	a.scale(b0, p[0]);
	b.scale(b1, p[1]);
	c.scale(b2, p[2]);
	d.scale(b3, p[3]);

	r.add(a + b, c + d);

	return r;
}

Vector3 BezierSurface::Calculate(float u, float v) {
	Vector3 temp[4];
	temp[0] = Calculate(u, 0);
	temp[1] = Calculate(u, 1);
	temp[2] = Calculate(u, 2);
	temp[3] = Calculate(u, 3);

	return bernstein(v, temp);
}

Vector3 BezierSurface::Calculate(float t, int row) {
	Vector3 p;

	float it = 1.0f - t;

	float b0 = t*t*t;
	float b1 = 3 * t*t*it;
	float b2 = 3 * t*it*it;
	float b3 = it*it*it;

	p.x = b0*ctrlpoints[row][0].x +
		b1*ctrlpoints[row][1].x +
		b2*ctrlpoints[row][2].x +
		b3*ctrlpoints[row][3].x;

	p.y = b0*ctrlpoints[row][0].y +
		b1*ctrlpoints[row][1].y +


		b2*ctrlpoints[row][2].y +
		b3*ctrlpoints[row][3].y;

	p.z = b0*ctrlpoints[row][0].z +
		b1*ctrlpoints[row][1].z +
		b2*ctrlpoints[row][2].z +
		b3*ctrlpoints[row][3].z;

	return p;
}

Vector3 BezierSurface::CalculateNormals(float t, int row) {
	Vector3 p;

	float it = 1.0f - t;

	float b0 = 3 * t*t;
	float b1 = 6 * t*it;
	float b2 = 3 * it*it;

	float x = b0 * (ctrlpoints[row][1].x - ctrlpoints[row][0].x) +
		b1 * (ctrlpoints[row][2].x - ctrlpoints[row][1].x) +
		b2 * ctrlpoints[row][3].x - ctrlpoints[row][2].x;

	float y = b0 * (ctrlpoints[row][1].y - ctrlpoints[row][0].y) +
		b1 * (ctrlpoints[row][2].y - ctrlpoints[row][1].y) +
		b2 * ctrlpoints[row][3].y - ctrlpoints[row][2].y;

	p.x = y;
	p.y = -x;
	p.z = 0;

	return p;
}

Vector3 BezierSurface::CalculateNormals(float u, float v) {
	Vector3 temp[4];
	temp[0] = CalculateNormals(u, 0);
	temp[1] = CalculateNormals(u, 1);
	temp[2] = CalculateNormals(u, 2);
	temp[3] = CalculateNormals(u, 3);

	return bernsteinNormal(v, temp);
}


