#include "Terrain.h"

static float mask[8][2] =
{
	{ -1.0f, -1.0f }, { 1.0f, 0.0f }, { -1.0f, 0.0f }, { 1.0f, 1.0f },
	{ -1.0f, 1.0f }, { 0.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, -1.0f }
};

Terrain::Terrain()
{
	wireframe = false;
}


Terrain::~Terrain()
{
}

float  Terrain::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Terrain::random(float max){
	float r = rand();
	return (r / rand_m) * max - (max * 0.5);
}

float Terrain::dot(float grad[], float x, float y)
{
	return (grad[0] * x + grad[1] * y);
}

float Terrain::lerp(float t, float a, float b)
{
	return (1.0f - t) * a + b * t;
}

float Terrain::gradient(int h1, float i, float j, float k) {
	int h = 0;
	float u = 0.0f;
	float v = 0.0f;
	h = h1 & 15;
	u = h < 8 ? i : j,
		v = h < 4 ? j : h == 12 || h == 14 ? i : k;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

void Terrain::permute(){
	for (int i = 0; i < 256; i++)
		p[i] = (int)random(256);
	for (int i = 0; i < 256; i++)
	{
		p[256 + i] = p[i] = permutation[i];
	}
}

float Terrain::noise(int i, int j, float freq){
	int x = floor(j * freq);
	int y = floor(i * freq);

	//fractional grid points
	float fx = (j * freq) - x;
	float fy = (i * freq) - y;

	//indexing into the mask for the four nearby points
	int g1 = p[(x + p[y & 255]) & 255] & 7;
	int g2 = p[(x + 1 + p[y & 255]) & 255] & 7;
	int g3 = p[(x + p[(y + 1) & 255]) & 255] & 7;
	int g4 = p[(x + 1 + p[(y + 1) & 255]) & 255] & 7;

	//dot products that get the noise from each corner.
	float n1 = dot(mask[g1], fx, fy);
	float n2 = dot(mask[g2], fx - 1.0f, fy);
	float n3 = dot(mask[g3], fx, fy - 1.0f);
	float n4 = dot(mask[g4], fx - 1.0f, fy - 1.0f);

	//fade value calculated by Ken Perlin
	float sx = fade(fx);
	float sy = fade(fy);

	//interpolation along x axis
	float i1 = lerp(n1, n2, sx);
	float i2 = lerp(n3, n4, sx);

	return lerp(i1, i2, sy);
}

void Terrain::initHeightField(int oct, float gain, float lacunarity) {
	//initializes the four corners of the map
	perlin[0][0] = MIN_Z + random(1.0f);
	perlin[0][SIZE - 1] = MIN_Z + random(1.0f);
	perlin[SIZE - 1][0] = MIN_Z + random(1.0f);
	perlin[SIZE - 1][SIZE - 1] = MIN_Z + random(1.0f);

	float amp, freq, height;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			amp = 1.0f;
			freq = 1.0f / (float)SIZE;
			height = 0.0f;
			for (int k = 0; k < oct; ++k)
			{
				float lerped = noise(i, j, freq);
				height += lerped * amp;
				amp *= gain;
				freq *= lacunarity;
			}
			perlin[j][i] = height;

			//init to light green
			float r = 0.3;
			float g = 0.7;
			float b = 0.3;

			if (height < 0.0){
				r = 0.3;
				g = 0.3;
				b = 0.7;
			}
			color[j][i] = new Vector3(r, g, b);
		}
	}
}

void Terrain::draw(void) {
	glClearColor(0.4, 0.4, 0.4, 0.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, TerrainAmbDiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, TerrainSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, TerrainShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, NoEmit);
	float ambDiff[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	Vector3 tmp;
	for (int r = 0; r < SIZE; r++) {
		float z = MIN_Z + r * step;
		for (int c = 0; c < SIZE; c++) {
			float x = MIN_X + c * step;
			if (r < SIZE - 1 && c < SIZE - 1) {
				if(!wireframe) {
					glBegin(GL_TRIANGLE_STRIP);
				}
				else {
					glBegin(GL_LINE_STRIP);
				}
				ambDiff[0] = color[r][c]->x; ambDiff[1] = color[r][c]->y; ambDiff[2] = color[r][c]->z;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambDiff);
				tmp = computeNormalX(x, r, c, perlin, z, step);
				glNormal3f(tmp.getX(), tmp.getY(), tmp.getZ());
				glVertex3f(x, perlin[r][c], z);
				ambDiff[0] = color[r + 1][c]->x; ambDiff[1] = color[r + 1][c]->y; ambDiff[2] = color[r + 1][c]->z;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambDiff);
				glVertex3f(x, perlin[r + 1][c], z + step);
				ambDiff[0] = color[r][c + 1]->x; ambDiff[1] = color[r][c + 1]->y; ambDiff[2] = color[r][c + 1]->z;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambDiff);
				glVertex3f(x + step, perlin[r][c + 1], z);
				glEnd();

				if (!wireframe) {
					glBegin(GL_TRIANGLE_STRIP);
				}
				else {
					glBegin(GL_LINE_STRIP);
				}
				ambDiff[0] = color[r][c + 1]->x; ambDiff[1] = color[r][c + 1]->y; ambDiff[2] = color[r][c + 1]->z;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambDiff);
				glVertex3f(x + step, perlin[r][c + 1], z);
				ambDiff[0] = color[r + 1][c]->x; ambDiff[1] = color[r + 1][c]->y; ambDiff[2] = color[r + 1][c]->z;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambDiff);
				glVertex3f(x, perlin[r + 1][c], z + step);
				ambDiff[0] = color[r + 1][c + 1]->x; ambDiff[1] = color[r + 1][c + 1]->y; ambDiff[2] = color[r + 1][c + 1]->z;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambDiff);
				glVertex3f(x + step, perlin[r + 1][c + 1], z + step);
				glEnd();
			}
		}
	}
	// Flush output to the screen:
	glFlush();
}

Vector3 Terrain::computeNormalX(float x, int r, int c, float p[][SIZE], float z, int step) {
	Vector3 v1, v2, v3;
	Vector3 tmp = Vector3();
	Vector3 tmp2 = Vector3();
	v1 = Vector3(x, perlin[r][c], z);
	v2 = Vector3(x, perlin[r + 1][c], z + step);
	v3 = Vector3(x + step, perlin[r][c + 1], z);

	tmp.subtract(v2, v1);
	tmp2.subtract(v3, v1);
	tmp.cross(tmp, tmp2);
	tmp.normalize();
	return tmp;
}

Vector3 Terrain::computeNormalY(float x, int r, int c, float p[][SIZE], float z, int step) {
	Vector3 v1, v2, v3;
	Vector3 tmp = Vector3();
	Vector3 tmp2 = Vector3();
	v1 = Vector3(x + step, perlin[r][c + 1], z);
	v2 = Vector3(x, perlin[r + 1][c], z + step);
	v3 = Vector3(x + step, perlin[r + 1][c + 1], z + step);

	tmp.subtract(v2, v1);
	tmp2.subtract(v3, v1);
	tmp.cross(tmp, tmp2);
	tmp.normalize();
	return tmp;
}

void Terrain::smooth() {
	int x, y, sum;
	for (x = 1; x < SIZE - 1; x++)
	for (y = 1; y < SIZE - 1; y++)
	{
		sum = perlin[x][y];
		sum += perlin[x - 1][(y - 1)];
		sum += perlin[x][(y - 1)];
		sum += perlin[x + 1][(y - 1)];
		sum += perlin[x - 1][y];
		sum += perlin[x + 1][y];
		sum += perlin[x - 1][y + 1];
		sum += perlin[x][y + 1];
		sum += perlin[x + 1][y + 1];

		if (sum % 9 > 4)
			sum = (sum / 9) + 1;
		else
			sum = sum / 9;
		perlin[x][y] = sum;
	}
}
