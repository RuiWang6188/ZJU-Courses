#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define FREEGLUT_STATIC

#include <iostream>

#include"camera.h"

#define PI 3.1415926535

static int mytime = 0;
static GLfloat times = 1.0f;
static GLfloat LastX = 0.0f, LastY = 0.0f, angle_0 = 0.0f, angle_1 = 0.0f;

GLUnurbsObj* theNurb;
Camera myCamera;

using namespace std;

GLfloat ctrlpoints[5][6][3] = {
	{
		{-2.0, 0.9, -1.1},
		{-1.7, 0.9, -1.3},
		{-1, 0.9, -1.4},
		{0, 0.9, -1.4},
		{1, 0.85, -1.3},
		{1.8, 0.7, -1.2}
	},
	{
		{-3.0, 1, -0.7},
		{-1.7, 1.5, -0.7},
		{-1, 2, -0.7},
		{0, 1.8, -0.7},
		{1, 1.5, -0.7},
		{2.2, 0.8, -0.7}
	},
	{
		{-3.5, 1, 0},
		{-1.7, 1.5, 0},
		{-1, 2, 0},
		{0, 1.9, 0},
		{1, 1.5, 0},
		{2.3, 0.85, 0}
	},
	{
		{-3.0, 1, 0.7},
		{-1.7, 1.5, 0.7},
		{-1, 2, 0.7},
		{0, 1.8, 0.7},
		{1, 1.5, 0.7},
		{2.2, 0.8, 0.7}
	},
	{
		{-2.0, 0.9, 1.1},
		{-1.7, 0.9, 1.3},
		{-1, 0.9, 1.4},
		{0, 0.9, 1.4},
		{1, 0.85, 1.3},
		{1.8, 0.7, 1.2}
	}
};

GLfloat ctrlPoints_carBody[7][6][3] = {
	{
		{-5, -1, -1.5},
		{-4, -1, -1.8},
		{-2.8, -1, -2},
		{1.0, -1, -2},
		{3.3, -1, -2},
		{4, -1, -2}
	},
	{
		{-5, -1, -1.5},
		{-4.5, 1, -1.8},
		{-3.0, 1.2, -2},
		{1.5, 1.2, -2},
		{3.3, 1, -2},
		{4, -1, -2}
	},
	{
		{-5.5, -1, -1.4},
		{-4.5, 1, -1.4},
		{-3.5, 2, -1.4},
		{1.8, 2, -1.4},
		{3.3, 1, -1.4},
		{4, -1, -1.4}
	},
	{
		{-5.8, -1, 0},
		{-4.5, 1, 0},
		{-3.5, 2, 0},
		{2.3, 2, 0},
		{3.3, 1, 0},
		{4, -1, 0}
	},
	{
		{-5.5, -1, 1.4},
		{-4.5, 1, 1.4},
		{-3.5, 2, 1.4},
		{1.8, 2, 1.4},
		{3.3, 1, 1.4},
		{4, -1, 1.4}
	},
	{
		{-5, -1, 1.5},
		{-4.5, 1, 1.8},
		{-3.0, 1.2, 2},
		{1.5, 1.2, 2},
		{3.3, 1, 2},
		{4, -1, 2}
	},
	{
		{-5, -1, 1.5},
		{-4, -1, 1.8},
		{-2.8, -1, 2},
		{1.0, -1, 2},
		{3.3, -1, 2},
		{4, -1, 2}
	},
};


GLfloat ctrlPoints_carChassis[7][6][3] = {
	{
		{-5, -1, -1.5},
		{-4, -1, -1.8},
		{-2.8, -1, -2},
		{1.0, -1, -2},
		{3.3, -1, -2},
		{4, -1, -2}
	},
	{
		{-5, -1, -1.5},
		{-4.5, -1, -1.8},
		{-3.0, -1, -2},
		{1.5, -1, -2},
		{3.3, -1, -2},
		{4, -1, -2}
	},
	{
		{-5.5, -1, -1.4},
		{-4.5, -1, -1.4},
		{-3.5, -1, -1.4},
		{1.8, -1, -1.4},
		{3.3, -1, -1.4},
		{4, -1, -1.4}
	},
	{
		{-5.8, -1, 0},
		{-4.5, -1, 0},
		{-3.5, -1, 0},
		{2.3, -1, 0},
		{3.3, -1, 0},
		{4, -1, 0}
	},
	{
		{-5.5, -1, 1.4},
		{-4.5, -1, 1.4},
		{-3.5, -1, 1.4},
		{1.8, -1, 1.4},
		{3.3, -1, 1.4},
		{4, -1, 1.4}
	},
	{
		{-5, -1, 1.5},
		{-4.5, -1, 1.8},
		{-3.0, -1, 2},
		{1.5, -1, 2},
		{3.3, -1, 2},
		{4, -1, 2}
	},
	{
		{-5, -1, 1.5},
		{-4, -1, 1.8},
		{-2.8, -1, 2},
		{1.0, -1, 2},
		{3.3, -1, 2},
		{4, -1, 2}
	},
};


void init(void);
void display(void);
void reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void onMouseWheel(int button, int dir, int x, int y);
void keyboardDown(unsigned char key, int x, int y);
void drawCar();
void drawCarWheel(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks);
GLvoid myDrawCircle(float cx, float cy, float cz, float r, int num_segments);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 750);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(Mouse);
	glutMotionFunc(onMouseMove);
	glutMouseWheelFunc(onMouseWheel);
	glutKeyboardFunc(keyboardDown);

	glutMainLoop();
	return 0;
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);


	//允许深度测试
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	//打开自动法矢量开关
	glEnable(GL_AUTO_NORMAL);
	//允许正则化法矢量
	glEnable(GL_NORMALIZE);

	theNurb = gluNewNurbsRenderer(); 
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 5.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	drawCar();
	glutSwapBuffers();

	glutPostRedisplay();
	glLoadIdentity();
	gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);

	//cout << "myCamera.camera_x = " << myCamera.camera_x;
	//cout << "\t myCamera.camera_y = " << myCamera.camera_y;
	//cout << "\tez = " << myCamera.camera_z;
	//cout << "\nox = " << myCamera.ref_x;
	//cout << "\toy = " << myCamera.ref_y;
	//cout << "\toz = " << myCamera.ref_z;
	//cout << "\nupx = " << myCamera.upVec_x;
	//cout << "\tupy = " << myCamera.upVec_y;
	//cout << "\tupz = " << myCamera.upVec_z;

	glFlush();
	Sleep(50);
}




void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 1.0f, 20.0f);

	if (w <= h)
		glOrtho(-1.5f, 1.5f, -1.5f * (GLfloat)h / (GLfloat)w, 1.5f * (GLfloat)h / (GLfloat)w, -10.0f, 10.0f);
	else
		glOrtho(1.5f * (GLfloat)w / (GLfloat)h, 1.5f * (GLfloat)w / (GLfloat)h, -1.5f, 1.5f, -10.0f, 10.0f);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
		LastX = x, LastY = y;
}

void onMouseMove(int x, int y) {
	angle_0 = (x - LastX) * 0.1f / 180.0f * PI;
	angle_1 = (y - LastY) * 0.1f / 180.0f * PI;
	myCamera.camera_x = (myCamera.camera_x - myCamera.ref_x) * cos(angle_1) + (myCamera.camera_z - myCamera.ref_z) * sin(angle_1) + myCamera.ref_x;
	myCamera.camera_z = -(myCamera.camera_x - myCamera.ref_x) * sin(angle_1) + (myCamera.camera_z - myCamera.ref_z) * cos(angle_1) + myCamera.ref_z;
	myCamera.camera_x = (myCamera.camera_x - myCamera.ref_x) * cos(angle_0) - (myCamera.camera_y - myCamera.ref_y) * sin(angle_0) + myCamera.ref_x;
	myCamera.camera_y = (myCamera.camera_x - myCamera.ref_x) * sin(angle_0) + (myCamera.camera_y - myCamera.ref_y) * cos(angle_0) + myCamera.ref_y;
	LastX = x, LastY = y;
}

void onMouseWheel(int button, int dir, int x, int y) {
	if (dir < 0) {
		myCamera.camera_x = (myCamera.camera_x - myCamera.ref_x) / 1.1 + myCamera.ref_x;
		myCamera.camera_y = (myCamera.camera_y - myCamera.ref_y) / 1.1 + myCamera.ref_y;
		myCamera.camera_z = (myCamera.camera_z - myCamera.ref_z) / 1.1 + myCamera.ref_z;
	}
	else {
		myCamera.camera_x = (myCamera.camera_x - myCamera.ref_x) * 1.1 + myCamera.ref_x;
		myCamera.camera_y = (myCamera.camera_y - myCamera.ref_y) * 1.1 + myCamera.ref_y;
		myCamera.camera_z = (myCamera.camera_z - myCamera.ref_z) * 1.1 + myCamera.ref_z;
	}
}

void keyboardDown(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'W':
		myCamera.ref_z--;
		myCamera.camera_z--;
		break;

	case 'S':
		myCamera.ref_z++;
		myCamera.camera_z++;
		break;

	case 'A':
		myCamera.ref_y++;
		myCamera.camera_y++;
		break;

	case 'D':
		myCamera.ref_y--;
		myCamera.camera_y--;
		break;

	default:
		break;
	}
}


void drawCar()
{
	glPushMatrix();
	glColor3f(0.439, 0.502, 0.565);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 6, 0, 1, 18, 5, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.078, 0.576);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 6, 0, 1, 18, 7, &ctrlPoints_carChassis[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.86, 0.078, 0.235);
	GLfloat sknots[14] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	GLfloat tknots[12] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	gluBeginSurface(theNurb); // 开始曲面绘制 
	gluNurbsSurface(theNurb, 14, sknots, 12, tknots, 6 * 3, 3, &ctrlPoints_carBody[0][0][0], 7, 6, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb); // 结束曲面绘制  
	glPopMatrix();

	GLUquadricObj* quadratic;
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		if (i == 0)
			glTranslatef(-3.0f, -1.0f, -2.0f);
		if (i == 1)
			glTranslatef(-3.0f, -1.0f, 1.5f);
		if (i == 2)
			glTranslatef(2.0f, -1.0f, -2.2f);
		if (i == 3)
			glTranslatef(2.0f, -1.0f, 1.8f);
		glColor3f(0.69, 0.77, 0.87);
		quadratic = gluNewQuadric();
		drawCarWheel(quadratic, 0.8, 0.8, 0.4, 64, 64);
		glPopMatrix();
	}

}

void drawCarWheel(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks)
{
	gluCylinder(quad, base, top, height, slices, stacks);
	myDrawCircle(0.0, 0.0, height, top, slices);
	myDrawCircle(0.0, 0.0, 0.0, base, slices);
}


GLvoid myDrawCircle(float cx, float cy, float cz, float r, int num_segments)
{
	GLfloat vertex[4];

	const GLfloat delta_angle = 2.0 * PI / num_segments;
	glBegin(GL_TRIANGLE_FAN);

	vertex[0] = cx;
	vertex[1] = cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);

	for (int i = 0; i < num_segments; i++)
	{
		vertex[0] = std::cos(delta_angle * i) * r + cx;
		vertex[1] = std::sin(delta_angle * i) * r + cy;
		vertex[2] = cz;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
	}

	vertex[0] = 1.0 * r + cx;
	vertex[1] = 0.0 * r + cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);
	glEnd();
}




