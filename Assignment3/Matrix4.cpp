#include "Matrix4.h"

Matrix4::Matrix4()
{
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<4; ++j)
		{
			m[i][j] = 0;
		}
	}
}

Matrix4::Matrix4(
	double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33 )
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;
	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;
	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;
	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

Matrix4::Matrix4(Matrix4 &a) {
	m[0][0] = a.m[0][0];
	m[0][1] = a.m[0][1];
	m[0][2] = a.m[0][2];
	m[0][3] = a.m[0][3];
	m[1][0] = a.m[1][0];
	m[1][1] = a.m[1][1];
	m[1][2] = a.m[1][2];
	m[1][3] = a.m[1][3];
	m[2][0] = a.m[2][0];
	m[2][1] = a.m[2][1];
	m[2][2] = a.m[2][2];
	m[2][3] = a.m[2][3];
	m[3][0] = a.m[3][0];
	m[3][1] = a.m[3][1];
	m[3][2] = a.m[3][2];
	m[3][3] = a.m[3][3];
}

double* Matrix4::getPointer()
{
	return &m[0][0];
}

double Matrix4::get(int x, int y) {
	return m[x][y];
}

void Matrix4::identity()
{
	double ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<4; ++j)
		{
			m[i][j] = ident[i][j];
		}
	}
}

Matrix4 Matrix4::multiply(Matrix4 &a) {
	double n[4][4];
	double sum;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			sum = 0;
			for (int x = 0; x < 4; ++x)
			{
				sum += m[x][j] * a.m[i][x];
			}
			n[i][j] = sum;
		}
	}

	return Matrix4(
		n[0][0], n[0][1], n[0][2], n[0][3], 
		n[1][0], n[1][1], n[1][2], n[1][3], 
		n[2][0], n[2][1], n[2][2], n[2][3], 
		n[3][0], n[3][1], n[3][2], n[3][3]
	);
}

Vector4 Matrix4::multiply(Vector4 &a) {
	float n[4];
	float sum;

	for (int i = 0; i < 4; ++i) 
	{
		sum = 0;
		for (int j = 0; j < 4; ++j) 
		{
			sum += (float)m[i][j] * a[j];
		}
		n[i] = sum;
	}

	return Vector4(n[0], n[1], n[2], n[3]);
}

Matrix4 Matrix4::rotateX(double angle) {
	Matrix4 n = Matrix4(
		1, 0, 0, 0, 
		0, cos(angle), -sin(angle), 0,  
		0, sin(angle), cos(angle), 0, 
		0, 0, 0, 1
	);

	return n;
}

Matrix4 Matrix4::rotateY(double angle) {
	Matrix4 n = Matrix4(
		cos(angle), 0, sin(angle), 0, 
		0, 1, 0, 0, 
		-sin(angle), 0, cos(angle), 0, 
		0, 0, 0, 1
	);

	return n;
}

Matrix4 Matrix4::rotateZ(double angle) {
	Matrix4 n = Matrix4(
		cos(angle), -sin(angle), 0, 0, 
		sin(angle), cos(angle), 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1
	);

	return n;
}

Matrix4 Matrix4::rotate(double angle, Vector3 &a) {
	// Transformations slide 44
	Vector3 b = Vector3(a.getX(), a.getY(), a.getZ());
	b.normalize();

	Matrix4 rm = Matrix4(
		b.x*b.x + cos(angle)*(1 - b.x*b.x),
		b.x*b.y*(1 - cos(angle)) - b.z*sin(angle),
		b.x*b.z*(1 - cos(angle)) + b.y*sin(angle),
		0,

		b.x*b.y*(1 - cos(angle)) + b.z*sin(angle),
		b.y*b.y + cos(angle)*(1 - b.y*b.y),
		b.y*b.z*(1 - cos(angle)) - b.x*sin(angle),
		0,

		b.x*b.z*(1 - cos(angle)) - b.y*sin(angle),
		b.y*b.z*(1 - cos(angle)) + b.x*sin(angle),
		b.z*b.z + cos(angle)*(1 - b.z*b.z),
		0,

		0,
		0,
		0,
		1
		);
	return rm;
}

void Matrix4::scale(double a, double b, double c)
{
	Matrix4 n = Matrix4(
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		0, 0, 0, 1
	);

	Matrix4 r = this->multiply(n);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = r.get(i, j);
		}
	}
}

void Matrix4::translation(double a, double b, double c)
{
	m[3][0] += a;
	m[3][1] += b;
	m[3][2] += c;
}

void Matrix4::print()
{

	for (int i=0; i<4; i++)
	{
		Matrix4 n = Matrix4(*this);
		n.transpose();

		cout << "[";
		for (int j=0; j<4; j++)
		{
			cout << ("%f", n.m[i][j]);
			if (j != 3)
				cout << "\t";
		}

		cout << "]" << endl;
	}
	cout << endl;
}

void Matrix4::transpose()
{
	double t[4][4];
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			t[i][j] = m[j][i];
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = t[i][j];
		}
	}

}