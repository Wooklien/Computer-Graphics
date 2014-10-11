#include "cube.h"

static Cube cube;
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
Window::MovementType Window::Movement = NONE;

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
	
	// Draw sides of cube in object coordinate system:
	glBegin(GL_QUADS);
    glColor3f(red, green, blue);

    // Draw front face:
    glNormal3f(0.0, 0.0, 1.0);   
    glVertex3f(-5.0,  5.0,  5.0);
    glVertex3f( 5.0,  5.0,  5.0);
    glVertex3f( 5.0, -5.0,  5.0);
    glVertex3f(-5.0, -5.0,  5.0);
    
    // Draw left side:
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(-5.0,  5.0,  5.0);
    glVertex3f(-5.0,  5.0, -5.0);
    glVertex3f(-5.0, -5.0, -5.0);
    glVertex3f(-5.0, -5.0,  5.0);
    
    // Draw right side:
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f( 5.0,  5.0,  5.0);
    glVertex3f( 5.0,  5.0, -5.0);
    glVertex3f( 5.0, -5.0, -5.0);
    glVertex3f( 5.0, -5.0,  5.0);
  
    // Draw back face:
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(-5.0,  5.0, -5.0);
    glVertex3f( 5.0,  5.0, -5.0);
    glVertex3f( 5.0, -5.0, -5.0);
    glVertex3f(-5.0, -5.0, -5.0);
  
    // Draw top side:
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-5.0,  5.0,  5.0);
    glVertex3f( 5.0,  5.0,  5.0);
    glVertex3f( 5.0,  5.0, -5.0);
    glVertex3f(-5.0,  5.0, -5.0);
  
    // Draw bottom side:
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(-5.0, -5.0, -5.0);
    glVertex3f( 5.0, -5.0, -5.0);
    glVertex3f( 5.0, -5.0,  5.0);
    glVertex3f(-5.0, -5.0,  5.0);
	
	glEnd();

	glFlush();  
	glutSwapBuffers();
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

int main(int argc, char *argv[])
{
	float specular[]  = {1.0, 1.0, 1.0, 1.0};
	float shininess[] = {100.0};
	float position[]  = {0.0, 10.0, 1.0, 0.0};	// lightsource position
	
	glutInit(&argc, argv);      	      	      // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(Window::width, Window::height);      // set initial window size
	glutCreateWindow("OpenGL Cube for CSE167");    	      // open window and set window title

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
