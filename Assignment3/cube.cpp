#include "cube.h"

static Cube cube;
static Objects bunny("bunny.obj");
static Objects teddy("teddy.obj");
static Objects dragon("dragon.obj");
static double spin_rate = 0.01;
static double red = 3.0, green = 1.0, blue = 2.0;
static const float m_ROTSCALE = -2.1;

static Matrix4 n(
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0
	);

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
Vector3 Window::lastPoint = Vector3(0, 0, 0);
float Window::mid_x = 0;
float Window::mid_y = 0;
float Window::mid_z = 0;
Window::MovementType Window::Movement = NONE;
Window::ObjectType Window::type = F1;

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
	width = w;height = h;
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
	glLoadMatrixd(cube.getMatrix().getPointer());
	glColor3f(red, green, blue);

	drawObject();

	glFlush();  
	glutSwapBuffers();
}

void Window::drawCube() {
	// Draw sides of cube in object coordinate system:
	glBegin(GL_QUADS);

	// Draw front face:
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-5.0, 5.0, 5.0);
	glVertex3f(5.0, 5.0, 5.0);
	glVertex3f(5.0, -5.0, 5.0);
	glVertex3f(-5.0, -5.0, 5.0);

	// Draw left side:
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-5.0, 5.0, 5.0);
	glVertex3f(-5.0, 5.0, -5.0);
	glVertex3f(-5.0, -5.0, -5.0);
	glVertex3f(-5.0, -5.0, 5.0);

	// Draw right side:
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(5.0, 5.0, 5.0);
	glVertex3f(5.0, 5.0, -5.0);
	glVertex3f(5.0, -5.0, -5.0);
	glVertex3f(5.0, -5.0, 5.0);

	// Draw back face:
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-5.0, 5.0, -5.0);
	glVertex3f(5.0, 5.0, -5.0);
	glVertex3f(5.0, -5.0, -5.0);
	glVertex3f(-5.0, -5.0, -5.0);

	// Draw top side:
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-5.0, 5.0, 5.0);
	glVertex3f(5.0, 5.0, 5.0);
	glVertex3f(5.0, 5.0, -5.0);
	glVertex3f(-5.0, 5.0, -5.0);

	// Draw bottom side:
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-5.0, -5.0, -5.0);
	glVertex3f(5.0, -5.0, -5.0);
	glVertex3f(5.0, -5.0, 5.0);
	glVertex3f(-5.0, -5.0, 5.0);

	glEnd();
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
		drawCube();
	}
	else if (type == F2) {
		drawObject(bunny.nVerts, bunny.vertices, bunny.normals);
	}
	else if (type == F3) {
		drawObject(teddy.nVerts, teddy.vertices, teddy.normals);
	}
	else if (type == F4) {
		drawObject(dragon.nVerts, dragon.vertices, dragon.normals);
	}
}

Cube::Cube()
{
	angle = 0.0;
}

Matrix4& Cube::getMatrix()
{
	return matrix;
}

void Cube::setMatrix(Matrix4 &m, Matrix4 &n) {
	m = n;
}

void Cube::setAngle(double a)
{
	angle = a;
}

void Cube::spin(double deg)
{
	if (cube.angle > 360.0 || cube.angle < -360.0) cube.angle = 0.0;
	cube.setMatrix(cube.getMatrix(), cube.getMatrix().multiply(cube.getMatrix().rotateY(deg)));
}

Objects::Objects(string filename) {
	char *c = const_cast<char*>(filename.c_str());
	ObjReader::readObj(c, nVerts, &vertices, &normals, nIndices, &indices);
}

int main(int argc, char *argv[])
{
	float specular[]  = {1.0, 1.0, 1.0, 1.0};
	float shininess[] = {100.0};
	float position[]  = {0.0, 10.0, 1.0, 0.0};	// lightsource position
	
	glutInit(&argc, argv);      	      	      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(Window::width, Window::height);      // set initial window size
	glutCreateWindow("Opengl 3D Demo");    	      // open window and set window title

	glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
	glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
	glDisable(GL_CULL_FACE);     // disable backface culling to render both sides of polygons
	glShadeModel(GL_SMOOTH);             	      // set shading to smooth
	glMatrixMode(GL_PROJECTION); 
  
	// Generate material properties:
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
  
	// Generate light source:
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
  
	// Install callback functions:
	glutDisplayFunc(Window::displayCallback);
	glutReshapeFunc(Window::reshapeCallback);
	glutIdleFunc(Window::idleCallback);

	// To Prevent the cube from turning white when scaled down
	glEnable(GL_NORMALIZE);

	// Process keyboard input
	glutKeyboardFunc(Window::processNormalKeys);
	glutSpecialFunc(Window::processSpecialKeys);
    
	// Initialize cube matrix:
	cube.getMatrix().identity();
    
	glutMainLoop();
	return 0;
}

void Window::processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) 
	{
		case 'c':
			spin_rate = -spin_rate;
			break;
			/*Movement - x/X movein x-direction*/
		case 'r':
			if (cube.angle > 360.0 || cube.angle < -360.0) cube.angle = 0.0;
			cube.setMatrix(cube.getMatrix(), cube.getMatrix().multiply(cube.getMatrix().rotateZ(-0.1)));
			break;
		case 'R':
			if (cube.angle > 360.0 || cube.angle < -360.0) cube.angle = 0.0;
			cube.setMatrix(cube.getMatrix(), cube.getMatrix().multiply(cube.getMatrix().rotateZ(0.1)));
			break;
		case 'x':
			cube.getMatrix().translation(-1, 0, 0);
			break;
		case 'X':
			cube.getMatrix().translation(1, 0, 0);
			break;
			/*Movement - y/Y movein y-direction*/
		case 'y':
			cube.getMatrix().translation(0, -1, 0);
			break;
		case 'Y':
			cube.getMatrix().translation(0, 1, 0);
			break;
			/*Movement - z/Z movein z-direction*/
		case 'z':
			cube.getMatrix().translation(0, 0, -1);
			break;
		case 'Z':
			cube.getMatrix().translation(0, 0, 1);
			break;
			// Resets Cube to inital position
		case 'i':
			cube.getMatrix().identity();
			cube.setAngle(0);
			red = 3.0, green = 1.0, blue = 2.0;
			break;
			/*Scale cube - s/S (small/large)*/
		case 's':
			cube.getMatrix().scale(
				0.98, 
				0.98, 
				0.98
			);
			break;
		case 'S':
			cube.getMatrix().scale(
				1.02, 
				1.02, 
				1.02
			);
			break;

			/*Color Change: 1 = R, 2 = B, 3 = G, 4 = Yellow*/
		case '1':
			red = 2.0;
			green = 0.0;
			blue = 0.0;
			break;
		case '2':
			red = 0.0;
			green = 2.0;
			blue = 0.0;
			break;
		case '3':
			red = 0.0;
			green = 0.0;
			blue = 2.0;
			break;
		case '4':
			red = 3.0;
			green = 3.0;
			blue = 0.0;
			break;
	}

	Vector3 m = Vector3(
		cube.getMatrix().get(3, 0),
		cube.getMatrix().get(3,1), 
		cube.getMatrix().get(3,2)
	);

	m.print();
}

void Window::processSpecialKeys(int key, int x, int y) {
	float scale = 1;
	switch (key)
	{
	case GLUT_KEY_F1:
		type = F1;
		scale = 1;
		break;
	case GLUT_KEY_F2:
		type = F2;
		scale = fit_to_screen(bunny.nVerts, bunny.vertices); 
		break;
	case GLUT_KEY_F3:
		type = F3;
		scale = fit_to_screen(teddy.nVerts, teddy.vertices);
		break;
	case GLUT_KEY_F4:
		type = F4;
		scale = fit_to_screen(dragon.nVerts, dragon.vertices);
		break;
	}

	cube.getMatrix().identity();
	cube.getMatrix().translation(-mid_x, -mid_y, -mid_z);
	cube.getMatrix().scale(scale, scale, scale);
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
			cube.setMatrix(cube.getMatrix(), cube.getMatrix().multiply(n.rotate(rot_angle, rotAxis)));
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

float Window::fit_to_screen(int nVerts, float *vertices) {
	double dbl_max = 500;
	double min[3] = { dbl_max, dbl_max, dbl_max };
	double max[3] = { -dbl_max, -dbl_max, -dbl_max };

	int x = nVerts * 3;
	for (int i = 0; i < x; i += 3) {
		if (min[0] > vertices[i]) {
			min[0] = vertices[i];
		}

		if (max[0] < vertices[i]) {
			max[0] = vertices[i];
		}
	}

	for (int i = 1; i < x; i += 3) {
		if (min[1] > vertices[i]) {
			min[1] = vertices[i];
		}

		if (max[1] < vertices[i]) {
			max[1] = vertices[i];
		}
	}

	for (int i = 2; i < x; i += 3) {
		if (min[2] > vertices[i]) {
			min[2] = vertices[i];
		}

		if (max[2] < vertices[i]) {
			max[2] = vertices[i];
		}
	}

	float sx = max[0] - min[0];
	float sy = max[1] - min[1];
	float sz = max[2] - min[2];
	float d = sqrt(sx*sx + sy*sy + sz*sz);

	mid_x = (30/d)*(max[0] + min[0]) * 0.5;
	mid_y = (30/d)*(max[1] + min[1]) * 0.5;
	mid_z = (30/d)*(max[2] + min[2]) * 0.5;
	cout << "Offset(x,y,z): " << mid_x << ", " << mid_y << ", " << mid_z << "\n";
	cout << "Scale Factor: " << d << endl;
	return 30/d;
}
