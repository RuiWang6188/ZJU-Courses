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
	glutCreateWindow("A8");
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
	else if (xoffset < 0) {
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
	glEnable(GL_LIGHTING);
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//GLfloat lightColor[] = { 0.5,0.5,0.5,1 };
	GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //光源的位置在世界坐标系圆心，齐次坐标形式
	GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //RGBA模式的环境光，为0
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式的漫反射光，全白光
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //RGBA模式下的镜面光 ，全白光
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

	//开启灯光
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	// SUN
	glPushMatrix();
	glColor3f(0.99, 0.72, 0.075);
	glRotatef((GLfloat)sun_rotate, 0, 0, 1);
	GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的环境光颜色，为0
	GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的漫反射光颜色，为0
	GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的镜面反射光颜色，为0
	GLfloat sun_mat_emission[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //定义材质的辐射广颜色，为偏红色
	GLfloat sun_mat_shininess = 0.0f;
	//glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

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

	GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };  //定义材质的环境光颜色，骗蓝色
	GLfloat earth_mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //定义材质的漫反射光颜色，偏蓝色
	GLfloat earth_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //定义材质的镜面反射光颜色，红色
	GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射光颜色，为0
	GLfloat earth_mat_shininess = 30.0f;
	//glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);
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




