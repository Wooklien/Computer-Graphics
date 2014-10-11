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
	  static Vector3 lastPoint;

	  static void idleCallback(void);
	  static void reshapeCallback(int, int);
	  static void displayCallback(void);
	  static void processNormalKeys(unsigned char, int, int);
	  static void onLButtonDown(int button, int state, int x, int y);
	  static void onLButtonUp(int x, int y);
	  static void motionTracker(int x, int y);
	  static Vector3 trackBallMapping(int x, int y);

	  enum MovementType { ROTATE, ZOOM, NONE };  // Keep track of the current mode of interaction (which mouse button)
	  static MovementType Movement;                     //    Left-mouse => ROTATE, Right-mouse => ZOOM
};

#endif