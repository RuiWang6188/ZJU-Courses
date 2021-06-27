#define _CRT_SECURE_NO_WARNINGS

#include"header.h"

#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib"
#else
#define GL_Lib(name) name ".lib"
#endif
#endif
#pragma comment(lib, GL_Lib("freeglut_static"))

Camera myCamera;

static void dragMouse(int x, int y)
{
	GLfloat xoffset = x - lastX;
	GLfloat yoffset = y - lastY;
	lastX = x;
	lastY = y;

	if (xoffset > 0) {
		myCamera.ref_x += myCamera.adjust_step;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
	}
	else if (xoffset < 0) {
		myCamera.ref_x -= myCamera.adjust_step;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
	}

	if (yoffset > 0) {
		myCamera.ref_y -= myCamera.adjust_step;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
	}
	else if (yoffset < 0) {
		myCamera.ref_y += myCamera.adjust_step;
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

void keyBoardDown(unsigned char keyBoardDown, int x, int y) {
	switch (keyBoardDown) {
	case 87: //W
		myCamera.camera_z -= 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
		break;
	case 83: //S
		myCamera.camera_z += 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
		break;
	case 65: //A
		myCamera.ref_x -= 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
		break;
	case 68: //D
		myCamera.ref_x += 0.5;
		glLoadIdentity();
		gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
		glutPostRedisplay();
		break;
	}
}


void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45.0f, whRatio, 0.005f, 10000000.0f);
	// glOrtho(-4, 4, -4, 4, 0, 100);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// 重置变换矩阵（回到世界坐标系）

	gluLookAt(myCamera.camera_x, myCamera.camera_y, myCamera.camera_z, myCamera.ref_x, myCamera.ref_y, myCamera.ref_z, myCamera.upVec_x, myCamera.upVec_y, myCamera.upVec_z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_LIGHTING);
	// GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	// GLfloat light_pos[] = {5,5,5,1};

	// glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	// glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	// glEnable(GL_LIGHT0);

	GLfloat lightColor[] = { 0.5,0.5,0.5,1 };
	GLfloat lightPos[] = { 0.8,0.8,0.8,1 };
	const GLfloat lightType[] = { 7.0f };
	GLfloat lightEmmision[] = { 0,0,0,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lightColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightPos);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, lightType);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, lightEmmision);
	glEnable(GL_LIGHT0);

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
		// glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
		//glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);

	if (isPlanetRun) {

		sun.curSelfAngle += timeFactor / sun.selfRotatTime;
		earth.curSelfAngle += timeFactor / earth.selfRotatTime;
		earth.curOrbitAngle += timeFactor / earth.orbitRotatTime;
		moon.curOrbitAngle += timeFactor / moon.orbitRotatTime;
		mars.curSelfAngle += timeFactor / mars.selfRotatTime;
		mars.curOrbitAngle += timeFactor / mars.orbitRotatTime;
		jupiter.curSelfAngle += timeFactor / jupiter.selfRotatTime;
		jupiter.curOrbitAngle += timeFactor / jupiter.orbitRotatTime;
	}

	draw();
	glEnable(GL_DEPTH_TEST);
	// glutSolidCube(1);
	glutSwapBuffers();
}



int main(int argc, char* argv[])
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1280, 960);
	int windowHandle = glutCreateWindow("A9");

	initialize();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyBoardDown);
	glutMotionFunc(dragMouse);
	glutMouseFunc(onMouse);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}


