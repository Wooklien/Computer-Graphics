#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <string>
#include <iostream>

using namespace std;

class Vector3 {
	public:
		float x, y, z;
		Vector3();
		Vector3(float, float, float);
		
		//Defined Functions
		void set(float, float, float);
		// Get X,Y,Z coordinates
		float getX();
		float getY();
		float getZ();
		// Return X,Y,Z Coordinates with indexing
		float operator[](const int i);

		// Add functions and Overloading + operator
		void add(Vector3&);
		void add(Vector3 &a, Vector3 &b);
		Vector3 operator+(const Vector3 &other) const;

		// Subraction functions and Overloading - operator
		void subtract(Vector3 &a);
		void subtract(Vector3 &a, Vector3 &b);
		Vector3 operator-(const Vector3 &other) const;

		// Negation
		void negate();
		void negate(Vector3 &a);
		
		// Vector Scaling
		void scale(float s);
		void scale(float s, Vector3 &a);

		// Dot product and Cross product
		float dot(Vector3 &a);
		void cross(Vector3 &a, Vector3 &b);
		
		float magnitude();
		void normalize();
		void print();
};

#endif