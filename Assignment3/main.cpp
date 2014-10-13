#include "objreader.h"

int nVerts;      // number of vertices in file
float *vertices; // pointer to list of vertices
float *normals;  // pointer to list of normals (number of normals equals number of vertices)
int nIndices;    // number of indices for vertices
int *indices;    // pointer to list of indices

// This file shows a sample usage scenario to load the Bunny data set

void main(int argc, char** argv)
{
  ObjReader::readObj("bunny.obj", nVerts, &vertices, &normals, nIndices, &indices);
}
