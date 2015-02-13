#include <iostream>
#include "objreader.h"

#define _CRT_SECURE_NO_WARNINGS

void ObjReader::get_indices(char *word, int *vindex, int *nindex)
{
	char *null = " ";
	char *ptr;
	char *tp;
	char *np;

	/* by default, the texture and normal pointers are set to the null string */
	tp = null;
	np = null;

	/* replace slashes with null characters and cause tp and np to point */
	/* to character immediately following the first or second slash */

	for (ptr = word; *ptr != '\0'; ptr++) 
  {
		if (*ptr == '/') {
			if (tp == null)
				tp = ptr + 1;
			else
				np = ptr + 1;

			*ptr = '\0';
		}
	}

	*vindex = atoi(word);
	*nindex = atoi(np);
}


void ObjReader::readObj(char* fileName, int &nVertices, float **vertices, float **normals, int &nIndices, int **indices)
{
	FILE * fp = fopen(fileName,"r");
	int nv=0, nn=0, nf=0;
	char line[81];

  if (fp == NULL)
  {
    std::cerr << "File not found: " << fileName << std::endl;
    return;
  }

	while ( fgets( line, 80, fp ) != NULL ) 
  {
		if (line[0] == 'v') {
			if (line[1] == 'n')
				nn++;
			else
				nv++;
		} 
    else if (line[0] == 'f') 
			nf++;
	}
	fseek(fp, 0, 0);

	// Arrays for normals, vertices
	float *n = new float[3*(nn>nf?nn:nf)];
	float *v = new float[3*nv];
	
	// Index arrays for normals, vertices
	int *vInd = new int[3*nf];
	int *nInd = new int[3*nf];

	int nvertices = 0;
	int nnormals = 0;
	int nindices  = 0;
	int ntriangles = 0;
	bool noNormals = false;

	while (fgets(line, 80, fp) != NULL) 
  {
		if (line[0] == 'v') // is this line for a vertex or vertex normal?
    {
			if (line[1] == 'n') // is this a normal vector?
      {
				float x, y, z;
				sscanf( &line[2], "%f %f %f\n", &x, &y, &z);
				float l = sqrt(x*x+y*y+z*z);
				x = x/l;
				y = y/l;
				z = z/l;
				n[nnormals]=x;
				nnormals++;
				n[nnormals]=y;
				nnormals++;
				n[nnormals]=z;
				nnormals++;
			} 
      else // not a normal, just a vertex
      {
				float x, y, z;
				sscanf( &line[1], "%f %f %f\n", &x, &y, &z);
				v[nvertices] = x;
				nvertices++;
				v[nvertices] = y;
				nvertices++;
				v[nvertices] = z;
				nvertices++;
			}
		} 
    else if (line[0] == 'f') // is this line for a face?
    {
			char s1[32], s2[32], s3[32];
			int vI, nI;
			sscanf( &line[1], "%s %s %s\n", s1, s2, s3);

			// Indices for first vertex
			get_indices(s1, &vI, &nI);
			vInd[nindices] = vI-1;
			if (nI)
				nInd[nindices] = nI-1;
			else
				noNormals = true;
      nindices++;
			
			// Indices for second vertex
			get_indices(s2, &vI, &nI);
			vInd[nindices] = vI-1;
			if (nI)
				nInd[nindices] = nI-1;
			else
				noNormals = true;
			nindices++;
			
			// Indices for third vertex
			get_indices(s3, &vI, &nI);
			vInd[nindices] = vI-1;
			if (nI)
				nInd[nindices] = nI-1;
			else
				noNormals = true;
			nindices++;

			ntriangles++;
		}
	}

	/* We don't currently support separate indices for normals, vertices, and 
	   texture coordinates. */
	*vertices = new float[ntriangles*9];
	if(!noNormals) *normals = new float[ntriangles*9]; else *normals=0;
	*indices = new int[ntriangles*3];
	nVertices = ntriangles*3;
	nIndices = ntriangles*3;

	for(int i=0; i<ntriangles; i++)
	{
		(*indices)[3*i] = 3*i;
		(*indices)[3*i+1] = 3*i+1;
		(*indices)[3*i+2] = 3*i+2;

		(*vertices)[9*i] = v[3*vInd[3*i]];
		(*vertices)[9*i+1] = v[3*vInd[3*i]+1];
		(*vertices)[9*i+2] = v[3*vInd[3*i]+2];

		(*vertices)[9*i+3] = v[3*vInd[3*i+1]];
		(*vertices)[9*i+4] = v[3*vInd[3*i+1]+1];
		(*vertices)[9*i+5] = v[3*vInd[3*i+1]+2];

		(*vertices)[9*i+6] = v[3*vInd[3*i+2]];
		(*vertices)[9*i+7] = v[3*vInd[3*i+2]+1];
		(*vertices)[9*i+8] = v[3*vInd[3*i+2]+2];

		if(!noNormals)
		{
			(*normals)[9*i] = n[3*nInd[3*i]];
			(*normals)[9*i+1] = n[3*nInd[3*i]+1];
			(*normals)[9*i+2] = n[3*nInd[3*i]+2];

			(*normals)[9*i+3] = n[3*nInd[3*i+1]];
			(*normals)[9*i+4] = n[3*nInd[3*i+1]+1];
			(*normals)[9*i+5] = n[3*nInd[3*i+1]+2];

			(*normals)[9*i+6] = n[3*nInd[3*i+2]];
			(*normals)[9*i+7] = n[3*nInd[3*i+2]+1];
			(*normals)[9*i+8] = n[3*nInd[3*i+2]+2];
		}
	}

	delete[] n;
	delete[] v;
	delete[] nInd;
	delete[] vInd;
}

