#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include <string>
#include "Vector3.h"
#include "Vector4.h"
#include <math.h>
#include <iostream>

using namespace std;

class Matrix4
{
	public:
		double m[4][4];
		Matrix4();
		Matrix4(double, double, double, double, 
			double, double, double, double, 
			double, double, double, double, 
			double, double, double, double);
		Matrix4(Matrix4&);

		double* getPointer();
		double get(int, int);
		void identity();
		Matrix4 multiply(Matrix4&);
		Vector4 multiply(Vector4&);
	
		Matrix4 rotateX(double);
		Matrix4 rotateY(double);
		Matrix4 rotateZ(double);
		void rotate(double, Vector3&);

		void scale(double, double, double);

		void translation(double, double, double);

		void print();

		void transpose();
};

#endif