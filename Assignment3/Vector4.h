#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <string>
#include <iostream>

using namespace std;

class Vector4 {
	public:
		float x, y, z, w;
		Vector4();
		Vector4(float, float, float);
		Vector4(float, float, float, float);

		// Setting X,Y,Z,W Coordinate Separately
		void set(float, float, float, float);
		void setX(float);
		void setY(float);
		void setZ(float);
		void setW(float);

		// Return X,Y,Z,W Coordinate
		float getX();
		float getY();
		float getZ();
		float getW();
		// Overloading [] Operator, return X,Y,Z,W with index
		float operator[] (const int);

		// Vector addition and Overloading + operator
		void add(Vector4&);
		void add(Vector4&, Vector4&);
		Vector4 operator+(const Vector4&) const;

		// Vector subtraction and Overloading - operator
		void subtract(Vector4&);
		void subtract(Vector4&, Vector4&);
		Vector4 operator-(const Vector4&) const;

		void dehomogenize();
		void print();
};

#endif