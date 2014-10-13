#ifndef _OBJ_READER_
#define _OBJ_READER_

class ObjReader
{
	public:
		static void readObj(char* fileName, int &nVertices, float **vertices, float **normals, int &nIndices, int **indices);

	private:
		static void get_indices(char *word, int *vindex, int *nindex);
};

#endif 

