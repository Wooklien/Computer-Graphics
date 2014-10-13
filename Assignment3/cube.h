#ifndef _CUBE_H_
#define _CUBE_H_

#include "Matrix4.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include "objreader.h"

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

class Objects {
public:
	int nVerts;      // number of vertices in file
	float *vertices; // pointer to list of vertices
	float *normals;  // pointer to list of normals (number of normals equals number of vertices)
	int nIndices;    // number of indices for vertices
	int *indices;    // pointer to list of indices

	Objects(string filename);
};

class Window	  // output window related routines
{
  public:
	  static int width, height; 	            // window size
	  static Vector3 lastPoint;
	  static float mid_x, mid_y, mid_z;

	  static void idleCallback(void);
	  static void reshapeCallback(int, int);
	  static void displayCallback(void);
	  static void processNormalKeys(unsigned char, int, int);
	  static void processSpecialKeys(int, int, int);
	  static void onLButtonDown(int button, int state, int x, int y);
	  static void onLButtonUp(int x, int y);
	  static void motionTracker(int x, int y);
	  static Vector3 trackBallMapping(int x, int y);
	  static void drawObject(int, float *, float *);
	  static void drawObject();
	  static void drawCube();
	  static float fit_to_screen(int nVerts, float *vertices);

	  enum MovementType { ROTATE, ZOOM, NONE };  // Keep track of the current mode of interaction (which mouse button)
	  enum ObjectType{ F1, F2, F3, F4 };
	  static MovementType Movement;                     //    Left-mouse => ROTATE, Right-mouse => ZOOM
	  static ObjectType type;
};

#endif