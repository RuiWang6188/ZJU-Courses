#define _CRT_SECURE_NO_WARNINGS
#define FREEGLUT_STATIC

#include"camera.h"

using namespace std;

static int sun_rotate, planet1_rotate, planet2_rotate, satellite_rotate;
static int planet1_revolute, planet2_revolute, satellite_revolute;

GLfloat lastX = 400, lastY = 300;

Camera myCamera;

static void init(void);
static void display(void);
static void idleDisplay(void);
static void reshape(int w, int h);
static void keyboardDown(unsigned char key, int x, int y);
static void onMouse(int button, int state, int x, int y);
static void dragMouse(int x, int y);

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("A4");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMotionFunc(dragMouse);
	glutMouseFunc(onMouse);
	glutKeyboardFunc(keyboardDown);
	glutIdleFunc(idleDisplay);
	glutMainLoop();
	return 0;
}

static void dragMouse(int x, int y)
{
	GLfloat xoffset = x - lastX;
	GLfloat yoffset = y - lastY;
	lastX = x;
	lastY = y;

	if (xoffset > 0) {
		myCamera.ref_y += 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
	}
	else if(xoffset < 0){
		myCamera.ref_y -= 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
	}

	if (yoffset > 0) {
		myCamera.ref_z -= 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
	}
	else if (yoffset < 0) {
		myCamera.ref_z += 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
	}
}

static void onMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		lastX = x;
		lastY = y;
	}
}


static void init(void)
{
	
	glClearColor(0.f, 0.f, 0.f, 1.0f);
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// SUN
	glPushMatrix();
	glColor3f(0.99, 0.72, 0.075);
	glRotatef((GLfloat)sun_rotate, 0, 0, 1);
	glutWireSphere(1.5, 50, 25);
	glPopMatrix();

	//Planet1
	glColor3f(0.88, 0.66, 0.37);
	glPushMatrix();
		//P1 revolution
	glRotatef((GLfloat)planet1_revolute, 0, 0, 1);
	glTranslatef(2.0, 0.0, 0.0);
	glPushMatrix();
		//P1 rotation
	glRotatef((GLfloat)planet1_rotate, 0, -1, 2);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();

	//Satellite of Planet1
	glColor3f(1.0, 1.0, 1.0);
	glRotatef((GLfloat)satellite_revolute, 0, 1, 1);
	glTranslatef(1.0, 0.0, 0.0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();
	glPopMatrix();

	//Planet2
	glColor3f(0.15, 0.77, 0.29);
	glPushMatrix();
		//P2 revolution
	glRotatef((GLfloat)planet2_revolute, 0, 0, 1);
	glTranslatef(5.0, 0.0, 0.0);
		//P2 rotation
	glRotatef((GLfloat)planet2_rotate, 0, -1, 2);
	glutSolidSphere(0.3, 20, 20);
	glPopMatrix();


	glutSwapBuffers();
}

static void idleDisplay(void)
{
	sun_rotate = (sun_rotate + 5) % 360;

	planet1_revolute = (planet1_revolute + 4) % 360;
	planet1_rotate = (planet1_rotate + 2) % 360;

	planet2_revolute = (planet2_revolute + 8) % 360;
	planet2_rotate = (planet2_rotate + 4) % 360;
	
	satellite_revolute = (satellite_revolute + 10) % 360;

	glutPostRedisplay();
	Sleep(100);
}

static void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //设置视口
	glMatrixMode(GL_PROJECTION); //指明当前矩阵为投影矩阵
	glLoadIdentity(); //将当前矩阵置换为单位矩阵
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0); //生成对称透视型视景体
	glMatrixMode(GL_MODELVIEW); //指明当前矩阵为模型视图矩阵
	glLoadIdentity(); //将当前矩阵置换为单位矩阵

	gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
}

static void keyboardDown(unsigned char key, int x, int y)
{
	switch (key) {
	case 87: //W
		myCamera.camera_x -= 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
		break;
	case 83: //S
		myCamera.camera_x += 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
		break;
	case 65: //A
		myCamera.ref_y -= 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
		break;
	case 68: //D
		myCamera.ref_y += 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
		break;
	}
}




