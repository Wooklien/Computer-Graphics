#ifndef _CUBE_H_
#define _CUBE_H_

#include "Matrix4.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>

using namespace std;

class Cube
{
	public:
		Matrix4 matrix;                // model matrix
		double angle;                   // rotation angle [degrees]
	
	public:
		Cube();   // Constructor
		Matrix4& getMatrix();
		void setMatrix(Matrix4 &m, Matrix4 &n);
		void setAngle(double);
		void spin(double);      // spin cube [degrees]
};

class Window	  // output window related routines
{
  public:
	  static int width, height; 	            // window size

	  static void idleCallback(void);
	  static void reshapeCallback(int, int);
	  static void displayCallback(void);
	  static void processNormalKeys(unsigned char, int, int);
};

#endif