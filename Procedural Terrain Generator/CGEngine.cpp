#include "CGEngine.h"

static CGEngine engine;
static const float m_ROTSCALE = -2.1;

static Matrix4 n(
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0);

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
Vector3 Window::lastPoint = Vector3(0, 0, 0);
Window::MovementType Window::Movement = NONE;
Window::ObjectType Window::type = F1;

static int octave = 6;
static float gain = 0.97f;
static float lacunarity = 2.1f;

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback(void)
{
	// Process Mouse input
	glutMouseFunc(Window::onLButtonDown);
	glutMotionFunc(Window::motionTracker);
	displayCallback(); // call display routine to re-draw cube
}

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void Window::reshapeCallback(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);  // set new viewport size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
	glTranslatef(0, 0, -20);
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void Window::displayCallback(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(engine.getMatrix().getPointer());

	gluLookAt(0.0, 3.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	engine.terrain.draw();

	glFlush();  
	glutSwapBuffers();
}

void Window::drawObject(int nVerts, float *vertices, float *normals) {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < nVerts * 3; i += 3) {
		glNormal3f(normals[i], normals[i + 1], normals[i + 2]);
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
	}
	glEnd();
}

void Window::drawObject() {
	if (type == F1) {
	}
	else if (type == F2) {
	}
	else if (type == F3) {
		
	}
}

CGEngine::CGEngine() {
	Vector3 eye = Vector3(0, 100, 5);
	Vector3 look = Vector3(0, 0, 0);
	Vector3 up = Vector3(0, 10, 0);

	camera = new Camera(eye, look, up);
	engine.terrain.permute();
	engine.terrain.initHeightField(octave, gain, lacunarity);
	engine.terrain.smooth();
	//engine.getMatrix().scale(10, 10, 10);
}

Matrix4& CGEngine::getMatrix()
{
	return matrix;
}

void CGEngine::setMatrix(Matrix4 &m, Matrix4 &n) {
	m = n;
}

/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* CGEngine::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic LOD:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

// load image file into texture object
void CGEngine::loadTexture()
{
	GLuint texture[1];     // storage for one texture
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	// Load image file
	tdata = loadPPM("texture.ppm", twidth, theight);
	if (tdata == NULL) return;

	// Create ID for texture
	glGenTextures(1, &texture[0]);

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Objects::Objects(string filename) {
	char *c = const_cast<char*>(filename.c_str());
	ObjReader::readObj(c, nVerts, &vertices, &normals, nIndices, &indices);
}

int main(int argc, char *argv[])
{	
	glutInit(&argc, argv);      	      	      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(Window::width, Window::height);      // set initial window size
	glutCreateWindow("Procedural Terrain");    	      // open window and set window title
	glutFullScreen();

	glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
	glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
	glDisable(GL_CULL_FACE);     // disable backface culling to render both sides of polygons
	glShadeModel(GL_SMOOTH);             	      // set shading to smooth
	glMatrixMode(GL_PROJECTION); 

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse_p[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_specular_p[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse_s[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular_s[] = { 1.0, 1.0, 1.0, 1.0 };
	float position[] = { 0.0, 10.0, 0.0, 0.0 };

	// Generate light source:
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Generate light source
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  
	// Install callback functions:
	glutDisplayFunc(Window::displayCallback);
	glutReshapeFunc(Window::reshapeCallback);
	glutIdleFunc(Window::idleCallback);

	// To Prevent the cube from turning white when scaled down
	glEnable(GL_NORMALIZE);

	engine.loadTexture();
	glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
	engine.initTerrain();

	// Process keyboard input
	glutKeyboardFunc(Window::processNormalKeys);
	glutSpecialFunc(Window::processSpecialKeys);
    
	// Initialize cube matrix:
	engine.getMatrix().identity();
	//glutTimerFunc(50, CGEngine::rotate, 0);
    
	glutMainLoop();
	return 0;
}

void Window::processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) 
	{
		case 'd':
			engine.getMatrix().translation(-1, 0, 0);
			break;
		case 'a':
			engine.getMatrix().translation(1, 0, 0);
			break;
			/*Movement - y/Y movein y-direction*/
		case 'y':
			engine.getMatrix().translation(0, -1, 0);
			break;
		case 'Y':
			engine.getMatrix().translation(0, 1, 0);
			break;
			/*Movement - z/Z movein z-direction*/
		case 's':
			engine.getMatrix().translation(0, 0, -1);
			break;
		case 'w':
			engine.getMatrix().translation(0, 0, 1);
			break;
			// Resets Cube to inital position
		case 'i':
			engine.getMatrix().identity();
			break;
		case 'g':
			gain += 0.03f;
			engine.terrain.initHeightField(octave, gain, lacunarity);
			engine.terrain.smooth();
			break;
		case 'G':
			gain -= 0.03f;
			engine.terrain.initHeightField(octave, gain, lacunarity);
			engine.terrain.smooth();
			break;
		case 'l':
			lacunarity += 0.01f;
			engine.terrain.initHeightField(octave, gain, lacunarity);
			engine.terrain.smooth();
			break;
		case 'L':
			lacunarity -= 0.01f;
			engine.terrain.initHeightField(octave, gain, lacunarity);
			engine.terrain.smooth();
			break;
		case 'o':
			octave++;
			engine.terrain.initHeightField(octave, gain, lacunarity);
			engine.terrain.smooth();
			break;
		case 'O':
			octave--;
			engine.terrain.initHeightField(octave, gain, lacunarity);
			engine.terrain.smooth();
			break;
		case 't':
			engine.getMatrix().scale(
				0.90,
				0.90,
				0.90
				);
			break;
		case 'T':
			engine.getMatrix().scale(
				1.05,
				1.05,
				1.05
				);
			break;
	}

	Vector3 m = Vector3(
		engine.getMatrix().get(3, 0),
		engine.getMatrix().get(3, 1),
		engine.getMatrix().get(3, 2)
	);
	cout << octave << "::" << gain << "::" << lacunarity << endl;
	m.print();
}

void Window::processSpecialKeys(int key, int x, int y) {
	float scale = 1;
	switch (key)
	{
		case GLUT_KEY_F1:
			//type = F1;
			engine.terrain.wireframe = !engine.terrain.wireframe;
			break;
		case GLUT_KEY_F2:
			//type = F2;
			break;
		case GLUT_KEY_F10:
			//type = F10;
			break;
		case GLUT_KEY_F11:
			type = F11;
			glutPositionWindow(width/4, height/4);
			glutReshapeWindow(width, height);
			break;
	}
}

void Window::onLButtonDown(int button, int state, int x, int y) {
	Movement = ROTATE;

	lastPoint = trackBallMapping(x, y);

	glMatrixMode(GL_MODELVIEW);
}

void Window::onLButtonUp(int x, int y) {
	Movement = NONE;
}

void Window::motionTracker(int x, int y) {
	Vector3 direction;
	float rot_angle;
	Vector3 curPoint;

	switch (Movement) {
	case ROTATE:
		curPoint = trackBallMapping(x, y);
		// sphere location.
		direction = curPoint - lastPoint;
		float velocity = direction.magnitude();
		if (velocity > 0.0001) {
			Vector3 rotAxis;
			rotAxis.cross(lastPoint, curPoint);
			rot_angle = velocity * m_ROTSCALE;
			n.identity();
			engine.setMatrix(engine.getMatrix(), engine.getMatrix().multiply(n.rotate(rot_angle, rotAxis)));
		}
		break;
	}

	lastPoint = curPoint;
}

Vector3 Window::trackBallMapping(int x, int y) {
	Vector3 v;
	float d;
	v.x = (1.0*x - width) / width;
	v.y = (height - 1.0*y) / height;
	v.z = 0.0;
	d = v.magnitude();
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrt(1.001 - d*d);
	v.normalize();
	return v;
}

void CGEngine::initTerrain()
{
}
