#define _CRT_SECURE_NO_WARNINGS
#define FREEGLUT_STATIC

#pragma once
#include<iostream>
#include <stdio.h>
#include<math.h>
#include<string.h>
#include<string>
#include "BaseElement.h"
#include"MyCamera.h"
#include"Solar.h"
#include"SpeedUnit.h"
#include"my_draw.h"
#include"objLoader.h"
#include"button.h"
#include"utility.h"
#include"ParticleSystem.h"

#include"gl/glui.h"

#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib"
#else
#define GL_Lib(name) name ".lib"
#endif
#endif
#pragma comment(lib, GL_Lib("freeglut_static"))

//#pragma comment(lib,"glui32.lib")
#pragma comment(lib,"GLAUX.LIB")
#pragma comment(lib, "legacy_stdio_definitions.lib")

//带d是
//当前是dbug编译，就在后面加d
int init_once = 1;
using namespace std;
void display1();
extern Texture My_texture;

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

int mouseX, mouseY;//前一时刻鼠标的位置

bool isClick = false;
int mode;	 // specify the mode (intro / help / game / back)
button* startButton, *helpButton, *exitButton, *backButton;
objLoader* ufo;
objLoader* cattle;

extern GLubyte* pixelData_INTRO;
extern GLubyte* pixelData_HELP;

void render() {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}
static void init() {
	memset(ctrl_grid, 0, sizeof(int)*grid_num);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	Camera_x = grid_size / 2;
	Camera_y = grid_size;
	Camera_z = -grid_size / 2;
	Gaze_x = Camera_x+Camera_radius* cos(move_angle * 2 * PI / 360);
	//Gaze_y = float(Camera_y +Camera_radius * tan(move_y_angle * 2 * PI / 360));
	Gaze_y = grid_size;
	Gaze_z = Camera_z+Camera_radius * sin(move_angle * 2 * PI / 360);
	
	//base init;
	int i, j;
	init_ctrl_grid();
	init_game_state();
	score = 0;
	
	for (i = 0; i < grid_num; i++) {
		for (j = 0; j < grid_num; j++) {
			mygrid[i][j].init(i*grid_size + grid_size / 2, 0, -(j*grid_size + grid_size / 2));
		
			mygrid[i][j].type = NORMAL;
			if(ground_grid[i][j]) mygrid[i][j].type =BLANK;
			
			if (ctrl_grid[i][j]) {

				if (i == 0)  mygrid[i][j].walltype[2] = 1;//left
				if (i == grid_num - 1)  mygrid[i][j].walltype[3] = 1;//right
				if (j == 0)  mygrid[i][j].walltype[1] = 1;//back
				if (j == grid_num - 1)  mygrid[i][j].walltype[0] = 1;//front

				if (j - 1 >= 0 && ctrl_grid[i][j - 1] == 0) mygrid[i][j].walltype[1] = 1;//后
				if (j + 1 < grid_num &&ctrl_grid[i][j + 1] == 0) mygrid[i][j].walltype[0] = 1;//前
				if (i - 1 >= 0 && ctrl_grid[i - 1][j] == 0) mygrid[i][j].walltype[2] = 1;//左
				if (i + 1 < grid_num&&ctrl_grid[i + 1][j] == 0) mygrid[i][j].walltype[3] = 1;//右
			}
			
		}
	}

	//speed
	the_people_speed.init();
	the_monster_speed.init();
	the_monster_speed.S = init_monster_s;
	//
	//texture and button
	if (init_once) {
		My_texture.init();

		ufo = new objLoader("./images/UFO_Empty.obj", OBJTYPE1, 250.0, 2 * grid_size, -550.0, 30);
		//cattle = new objLoader("./images/cattle.obj", OBJTYPE2, 250, 0, -250, 2);

		mode = MODE_INTRO;//;
		init_once = 0;
	}

	
	//light
	for (int i = 0; i < 7; i++) {
		the_light[i].light_num = i;
	}
	glPushMatrix();
	glTranslatef(0, 0, 0);

	glPopMatrix();
	
	if (mode == MODE_GAME) {
		glMatrixMode(GL_PROJECTION);//
		glLoadIdentity();//单位矩阵
		gluPerspective(125, 0.25*SCR_WIDTH / SCR_HEIGHT, 1, 10000);
		
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, SCR_WIDTH, SCR_HEIGHT, 0.0);
	
	}

	//Particle
	the_particle.init_particle();
}

static void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render();
	
	glPushMatrix();//需要保存，原因是平移可能就会把当前的矩阵平移走

	glScalef(times, times, times);//缩放
	glTranslatef(cameraDistanceX, cameraDistanceY, 0);//平移
	glRotatef(cameraAngleX, 1, 0, 0);//x轴旋转
	glRotatef(cameraAngleY, 0, 1, 0);//y轴旋转

	if (mode == MODE_INTRO) {
		the_light[0].close();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		// Method 3: My_texture.mode_texutre reading via SOIL
		glBindTexture(GL_TEXTURE_2D, My_texture.mode_texture[0]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0F);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, SCR_HEIGHT);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(SCR_WIDTH, SCR_HEIGHT);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(SCR_WIDTH, 0.0f);

		glEnd();
		glDisable(GL_TEXTURE_2D);

		startButton->drawButton();
		helpButton->drawButton();
		exitButton->drawButton();

		if (startButton->state == BUTTON_CLICK) {
			mode = MODE_GAME;
			init();
		}
		else if (helpButton->state == BUTTON_CLICK) {
			mode = MODE_HELP;
			init();
		}
		else if (exitButton->state == BUTTON_CLICK) {
			cout << "Exit!" << endl;
			exit(0);
		}
	}
	else if (mode == MODE_HELP) {
		the_light[0].close();
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_TEXTURE_2D);
		// Method 3: texture reading via SOIL	
		glBindTexture(GL_TEXTURE_2D, My_texture.mode_texture[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0F);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, SCR_HEIGHT);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(SCR_WIDTH, SCR_HEIGHT);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(SCR_WIDTH, 0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		backButton->drawButton();
		
		if (backButton->state == BUTTON_CLICK)
			mode = MODE_INTRO;

	}
	else if (mode == MODE_OVER) {  
		the_light[0].close();
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_TEXTURE_2D);
		// Method 3: texture reading via SOIL
		glBindTexture(GL_TEXTURE_2D, My_texture.mode_texture[2]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0F);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, SCR_HEIGHT);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(SCR_WIDTH, SCR_HEIGHT);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(SCR_WIDTH, 0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		backButton->drawButton();

		if (backButton->state == BUTTON_CLICK) {
			mode = MODE_INTRO;
			init();
		}
	}
	else if(mode==MODE_GAME){	// mode == MODE_GAME
		draw_all = 1;
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (down_down_state == 1) draw_all = 1;
		My_texture.Draw_Skybox(Camera_x, Camera_y, Camera_z, grid_size*grid_num*2,grid_size*20, grid_size*grid_num*2);

		Draw_Grid();
		ufo->draw();
		glPopMatrix();
	}
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

static void reshape(int w, int h) {
	SCR_WIDTH = w;
	SCR_HEIGHT = h;
	if (mode == MODE_INTRO) {
		startButton = new button((GLfloat)175.0 / 800.0 * SCR_WIDTH, (GLfloat)380.0 /600.0 * SCR_HEIGHT, 100, 50, "START");
		helpButton = new button(400.0 / 800 * SCR_WIDTH, 380.0 / 600 * SCR_HEIGHT, 100, 50, "  HELP");
		exitButton = new button(625.0 / 800 * SCR_WIDTH, 380.0 / 600 * SCR_HEIGHT, 100, 50, "  EXIT");
		backButton = new button(700.0 / 800 * SCR_WIDTH, 520.0 / 600 * SCR_HEIGHT, 100, 50, " BACK");
	}
	if (mode == MODE_GAME) {
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);//视口
		glMatrixMode(GL_PROJECTION);//
		glLoadIdentity();//单位矩阵
		gluPerspective(125, 0.25*w / h, 1, 10000);
		//glOrtho(0.0,w, 0.0, h, -1.0, 1.0);

		//glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(Camera_x, Camera_y+ the_people_speed.positiony, Camera_z, Gaze_x, Gaze_y, Gaze_z, 0, 1, 0);
		//	printf("reshape\n");
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, SCR_WIDTH, SCR_HEIGHT, 0.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0,0,0,0,0,1,0,1,0);
		//	printf("reshape\n");
	}
	

	glutPostRedisplay();
}
static void drag(int x, int y) {
	//printf("drag\n");
	//printf("pos:(%d %d)\n", x, y);
	//cameraAngleX = cameraAngleY = 0;
	//cameraDistanceX = cameraDistanceY = 0;
	//if (mode != MODE_GAME) return;

	if (mouseLeftDown)
	{
		if (en_rotate) {
			cameraAngleY += (x - mouseX) * 0.1f;
			cameraAngleX += (y - mouseY) * 0.001f;
		}
		else {
			move_angle+= (x - mouseX) * 1;
			//move_y_angle += ((y - mouseY)) * 1;
		}
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		if (en_rotate) {
			cameraDistanceX = (x - mouseX) * 0.002f;
			cameraDistanceY = -(y - mouseY) * 0.002f;
		}
		
		mouseY = y;
		mouseX = x;
	}
	glLoadIdentity();
 	Gaze_x = Camera_x + Camera_radius * cos(move_angle * 2 * PI / 360);
	//Gaze_y = float(Camera_y - Camera_radius * tan(move_y_angle * 2 * PI / 360));
	Gaze_z = Camera_z + Camera_radius * sin(move_angle * 2 * PI / 360);
	if (mode == MODE_GAME) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(Camera_x, Camera_y + the_people_speed.positiony, Camera_z, Gaze_x, Gaze_y - the_people_speed.positiony, Gaze_z, 0, 1, 0);
	}
	else {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	}
	
	glutPostRedisplay();
}
static void click(int button, int state, int x, int y) {
	//printf("click:%d\n", button);
	//printf("pos:(%d %d)\n", x, y);
	mouseX = x;
	mouseY = y;
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
	isClick = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) ? true : false;
	glutPostRedisplay();
}
static void wheel(int wheel, int dir, int x, int y) {
	//printf("wheel:%d\n", wheel);
	//printf("pos:(%d %d)\n", x, y);

	if (dir > 0) {
		times = -0.008f + times;
		glutPostRedisplay();
	}
	else {
		times = times + 0.008f;
	}
	glutPostRedisplay();
}
static void keyboarddown(unsigned char key, int x, int y) {
	//printf("key down: %d\n", key);
	//printf("pos:(%d %d)\n", x, y);
	if (mode != MODE_GAME) return;

	int px, pz,px_now,pz_now;
	GLfloat speed1 = the_people_speed.V_run;
	GLfloat C_x, C_z;
	pz_now = -int(Camera_z/grid_size);
	px_now = int(Camera_x/grid_size);
	
	GLfloat dx, dy;
	dx = cos(move_angle * 2 * PI / 360);
	dy = sin(move_angle * 2 * PI / 360);

	//int((Camera_z + (speed1 + 150 / 2) * (-1))
	C_x = Camera_x + (speed1 + grid_size / 2) * cos(move_angle * 2 * PI / 360);
	C_z = (Camera_z + (speed1 + grid_size / 2) * sin(move_angle * 2 * PI / 360));
	float x2 = (Camera_z + (speed1 + grid_size / 2) * sin(move_angle * 2 * PI / 360));
	pz = -int(C_z/ grid_size);
	px = int(C_x/ grid_size);
	run_state = 1;
	if ('w' == key) {
		if (Game_start_state==0) return;
		/*check grid_size/2*/
		/*if ((Camera_x  < px_now * grid_size + grid_size / 16) || (Camera_x > px_now * grid_size + grid_size *3/ 16)) {
			if (ctrl_grid[px][pz] != 1) {
				run_state = 0; return;
			}
		}
		if ((-Camera_z < pz_now * grid_size + grid_size / 16) || (-Camera_z  > px_now * grid_size + grid_size*3/ 16)) {
			if (ctrl_grid[px][pz] != 1) {
				run_state = 0; return;
			}
		}*/
		if (ctrl_grid[px][pz] == 0) {
			run_state = 0; return;
		}

		if (C_x < 0 || C_x >= grid_num * grid_size) {
			run_state = 0; return;
		}
		if (C_z > 0 || C_z <= -grid_num * grid_size) {
			run_state = 0; return;
		}
		if (px >= grid_num || px < 0) {
			run_state = 0; return;
		}
		if (pz >= grid_num || pz < 0) {
			run_state = 0; return;
		}
		//if (ctrl_grid[px][pz] ==-1) return;
		//if(ctrl_grid[px_now][pz]!=-1)
		Camera_z += speed1 * sin(move_angle * 2 * PI / 360);
		//if(ctrl_grid[px][pz_now]!=-1) 
		Camera_x += speed1 * cos(move_angle * 2 * PI / 360);

	}
	if ('S' == key) {
		Camera_z -= speed1 * sin(move_angle * 2 * PI / 360);
		Camera_x -= speed1 * cos(move_angle * 2 * PI / 360);
	}
	if ('A' == key) {
		move_angle -= 2;
	}
	if ('D' == key) {
		move_angle += 2;//+
	}
	if ('J' == key) {
		if (jump_state == 0) {
			jump_state = 1;//+
			the_people_speed.V_up = init_speed;
		}
	}
	if ('K' == key) {
		if (down_state == 0) {
			down_state = 1;//+
			the_people_speed.V_up = -init_speed;
		}
	}
	if (32 == key) {//空格
		if (mode == MODE_GAME) {
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);//视口
			glMatrixMode(GL_PROJECTION);//
			glLoadIdentity();//单位矩阵
			gluPerspective(125, 0.25*SCR_WIDTH / SCR_HEIGHT, 1, 10000);
			//glOrtho(0.0,w, 0.0, h, -1.0, 1.0);

			//glLoadIdentity();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			//gluLookAt(Camera_x, Camera_y+ the_people_speed.positiony, Camera_z, Gaze_x, Gaze_y, Gaze_z, 0, 1, 0);
			//printf("reshape\n");
			if (Game_over_state != 1) Game_start_state = !Game_start_state;
		}
	}
	if (key == 'R') {
		init();
	}
	if (key == 'P') {//改变位置，定义8个  lp位置 lo颜色  li强度
		the_light[0].lp=(the_light[0].lp+1)%8;
	}
	if (key == 'O') {//改变光的颜色，定义12个等级
		the_light[0].lo = (the_light[0].lo+1) % 12;
	}
	if (key == 'I') {//光的强度11
		the_light[0].li = (the_light[0].li+1) % 11;//
	}
	if (key == 'U') {//改变光照类型，可移动聚光灯、定点光源还是平行光
		the_light[0].light_type= (the_light[0].light_type+1)%3;//
	}
	
	if (move_angle < -180) {
		move_angle += 360;
	}
	if (move_angle > 180) {
		move_angle -= 360;
	}
	glLoadIdentity();
	Gaze_x = Camera_x + Camera_radius * cos(move_angle * 2 * PI / 360);
	Gaze_z = Camera_z + Camera_radius * sin(move_angle * 2 * PI / 360);

	if (check_coin(Camera_x,Camera_y+ the_people_speed.positiony, -Camera_z)) {
		//display1();
	}
	int flagg;
	flagg = check_collide(Camera_x, Camera_y + the_people_speed.positiony, -Camera_z);
	if (flagg) {
		the_people_speed.V_run = init_speed/4;//10 碰撞了则速度-- 
		if (flagg == 1) {
			down_state = 1;//+
			the_people_speed.V_up = -init_speed/4;
		}
		if (flagg == 2) {
			jump_state = 1;//+
			the_people_speed.V_up = init_speed/8;
		}
	}

	if (mode == MODE_GAME) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(Camera_x, Camera_y + the_people_speed.positiony, Camera_z, Gaze_x, Gaze_y - the_people_speed.positiony, Gaze_z, 0, 1, 0);
	}
	else {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	}
	/*最好是放在lookat 之后，如果是放在视点变换前面就是随着摄像机移动*/
	the_light[0].set_light_position(Camera_x, Camera_y, Camera_z, Gaze_x, Gaze_y, Gaze_z, 1);
	the_light[0].set_Light();
	glutPostRedisplay();
}
static void keyboardup(unsigned char key, int x, int y) {
	//printf("key up: %d\n", key);
	//printf("pos:(%d %d)\n", x, y);

	glutPostRedisplay();
}
static void specialdown(int key, int x, int y) {
	if (mode != MODE_GAME) return;
	//printf("special down: %d\n", key);
	//printf("pos:(%d %d)\n", x, y);
	if (key == GLUT_KEY_UP) {
		move_y_angle += 2;
	}
	if (key == GLUT_KEY_DOWN) {
		move_y_angle -= 2;
	}
	if (move_y_angle < -89) {//表示不能再转动
		move_y_angle = -89;
	}
	if (move_y_angle > 89) {
		move_y_angle = 89;
	}
	Gaze_y = float(Camera_y + Camera_radius * tan(move_y_angle * 2 * PI / 360));
	glLoadIdentity();
	if (mode == MODE_GAME) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(Camera_x, Camera_y + the_people_speed.positiony, Camera_z, Gaze_x, Gaze_y - the_people_speed.positiony, Gaze_z, 0, 1, 0);
	}
	else {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	}
	glutPostRedisplay();
}
static void specialup(int key, int x, int y) {
	//printf("special up: %d\n", key);
	//printf("pos:(%d %d)\n", x, y);
	glutPostRedisplay();
}
void Timerfuc(int x) {
	//specialdown(GLUT_KEY_UP, 0, 0);
	double step = 201;//设置地球的转动速度为step度/100ms；

	Earth_day += step;
	Mercury_day += step / 59; //水星自转动
	month = (month + step / 30);//每次转12*step度
	Earth_year += step / 360; //每次转step度,其实是表示速度
	Mercury_year += step / 88;
	Sun_day += step / 25;

	if (Earth_year > 360) {
		Earth_year -= 360;
	}
	if (Mercury_year < 0) {
		Earth_year += 360;
	}
	if (Mercury_year > 360) {
		Mercury_year -= 360;
	}
	if (Mercury_year < 0) {
		Mercury_year += 360;
	}
	if (month > 360) {
		month -= 360;
	}
	if (month < 0) {
		month += 360;
	}
	if (Earth_day > 360) {
		Earth_day -= 360;

	}
	if (Earth_day < 0) {
		Earth_day += 360;
	}
	if (Mercury_day < 0) {
		Mercury_day += 360;
	}
	if (Mercury_day > 360) {
		Mercury_day -= 360;
	}
	if (Game_start_state != 0) {
		the_people_speed.calculate_run_speed(1);//假设dt 为1
		the_people_speed.caculate_jump(1);
		the_monster_speed.calculate_run_speed(1);
	}
	
	chech_game_over();
	if (Game_over_state == 1) {
		Game_start_state = 0;
		mode = MODE_OVER;
		//init();
	}
	keyboarddown('w',0,0);
	glutPostRedisplay();
	glutTimerFunc(50, Timerfuc, 1);//由于调用一次才计数一次
	
}
GLuint mainw, subw1,subw2;

static void reshape1(int w, int h) {
	glViewport(0, 0, w, h);//视口
	glMatrixMode(GL_PROJECTION);//
	glLoadIdentity();//单位矩阵
	//gluPerspective(125, 0.25*w / h, 1, 10000);
	glOrtho(0.0,w, 0.0, h, -1.0, 1.0);
}
static void reshape2(int w, int h) {
	glViewport(0, 0, w, h);//视口
	glMatrixMode(GL_PROJECTION);//
	glLoadIdentity();//单位矩阵
	//gluPerspective(125, 0.25*w / h, 1, 10000);
	glOrtho(0.0,w, 0.0, h, -1.0, 1.0);

}
void display1() {
	
	if (mode != MODE_GAME) {
		//glutHideOverlay();
	}
	else {
		//glutShowOverlay();
	}
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glClearColor(1.f, 1.f,1.f, 1.0f);
	glLoadIdentity();
	glOrtho(0.0, 150, 0.0, 150, -1.0, 1.0);
	glShadeModel(GL_SMOOTH);
	
	glPushMatrix();
	
	
	GLfloat px, pz;
	GLfloat gx, gz;
	px =  sub_window_size*Camera_x / (grid_size*grid_num);
	pz = -sub_window_size*Camera_z / (grid_size*grid_num);
	gx = px + 15* cos(move_angle * 2 * PI / 360);
	//Gaze_x = Camera_x + Camera_radius * cos(move_angle * 2 * PI / 360);
	gz = pz - 15* sin(move_angle * 2 * PI / 360);
	Gaze_z = Camera_z + Camera_radius * sin(move_angle * 2 * PI / 360);
	int i, j; 
	glPointSize(5);
	glBegin(GL_POINTS);
	for (i = 0; i < grid_num; i++) {
		for (j = 0; j < grid_num; j++) {
			if (ctrl_grid[i][j] >= 1) {
				glColor3f(0.5, 0.55, 0.54);
				glVertex2f((i + 1 / 2)*sub_window_size / grid_num, (j + 1 / 2)*sub_window_size / grid_num);
			}
			if (ctrl_grid[i][j] == COIN) {
				glColor3f(0, 0, 1);
				glVertex2f((i + 1 / 2)*sub_window_size / grid_num, (j + 1 / 2)*sub_window_size / grid_num);
			}
		}
	}
	glColor3f(0, 1, 0);
	glVertex2f(px, pz);
	glEnd();
	
	glBegin(GL_LINES);
		glVertex2f(px, pz);
		glVertex2f(gx, gz);
	glEnd();

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	//glutPostRedisplay();
}
void display2() {
	/*if (mode != MODE_GAME) {
		glutHideWindow();
	}
	else {
		glutShowWindow();
	}*/
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glClearColor(1.f, 1.f,1.f, 1.0f);
	glLoadIdentity();
	glOrtho(0.0, 150, 0.0, 150, -1.0, 1.0);
	glShadeModel(GL_SMOOTH);

	glPushMatrix();

	//score
	//char ufo[64];sprintf(ufo, "%d", score);
	string s(to_string(score));
	s="Score:"+s;
	glRasterPos2f(0, 135);
	for (int i = 0; i < s.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s[i]);

	//people s
	string s1(to_string(the_people_speed.S));
	s1 = "p_d:" + s1;
	glRasterPos2f(0, 120);
	for (int i = 0; i < s1.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s1[i]);

	//
	string s2(to_string(the_monster_speed.S));
	s1 = "m_d:" + s2;
	glRasterPos2f(0, 105);
	for (int i = 0; i < s1.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s1[i]);
	
	//
	string s3(to_string(the_people_speed.S-the_monster_speed.S));
	s1 = "d_d:" + s3;
	glRasterPos2f(0, 90);
	for (int i = 0; i < s1.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s1[i]);

	//
	string s4("playing");
	string s5("Game_over");
	if (Game_over_state==0) s1 = s4;
	else s1 = s5;
	glRasterPos2f(0, 75);
	for (int i = 0; i < s1.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s1[i]);

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	//glutPostRedisplay();
}
void glutIdle() {
	//glutPostRedisplay();
	
	//if (mode == MODE_GAME) reshape1(0, 0);
	//else reshape1(sub_window_size,sub_window_size);

	int currentWindow = glutGetWindow();
	glutSetWindow(subw1);
	glutPostRedisplay();
	if (mode != MODE_GAME) glutHideWindow();
	else glutShowWindow();
	
	glutSetWindow(subw2);
	glutPostRedisplay();
	if (mode != MODE_GAME) glutHideWindow();
	else glutShowWindow();

	glutSetWindow(mainw);
	glutPostRedisplay();	
}

void myglutIdle() {
	//判断是否为当前窗口
	if (glutGetWindow() != mainw)
		glutSetWindow(mainw);

	//glutPostRedisplay 标记当前窗口需要重新绘制
	glutPostRedisplay();
}
void passiveMouse(int x, int y) {
	mouseX = x;
	mouseY = y;
	glutPostRedisplay();
}

void myMenu(int value) {

	if (value == 0) {
		mode = MODE_INTRO;
		init();
	}
	else if (value == 1) {
		mode = MODE_HELP;
		init();
	}
	else if (value == 2)
		exit(0);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutInitWindowPosition(0, 0);
	mainw=glutCreateWindow("Test");
	init();

	glutSetWindow(mainw);
	glutDisplayFunc(display);//display 函数自己写
	glutReshapeFunc(reshape);//调整窗口大小的时候，调用该函数
	glutMotionFunc(drag);//拖动组件
	glutCreateMenu(myMenu);
	glutAddMenuEntry("mainmenu", 0);
	glutAddMenuEntry("help", 1);
	glutAddMenuEntry("exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutPassiveMotionFunc(passiveMouse);
	glutMouseFunc(click);//按下弹起

	glutMouseWheelFunc(wheel);//鼠标滚轮
	glutKeyboardFunc(keyboarddown);//按键
	glutKeyboardUpFunc(keyboardup);//like 'a' ','
	glutSpecialFunc(specialdown);//'ctrl' 
	glutSpecialUpFunc(specialup);//
	glutTimerFunc(1000, Timerfuc, 1);//毫秒数, 回调函数指针, 区别值
	//if (mode == MODE_GAME) {
	subw1 = glutCreateSubWindow(mainw, 0, 0, sub_window_size, sub_window_size);
	//glutInitWindowPosition(650, 50);
	//glutCreateWindow("test1");//<-第二个glut窗口
	glutSetWindow(subw1);
	glutDisplayFunc(display1);
	//glutReshapeFunc(reshape1);//调整窗口大小的时候，调用该函数

	subw2 = glutCreateSubWindow(mainw, 800 - sub_window_size, 0, sub_window_size, sub_window_size);
	glutSetWindow(subw2);
	glutDisplayFunc(display2);
	//glutReshapeFunc(reshape2);//调整窗口大小的时候，调用该函数
	glutIdleFunc(glutIdle);
	
	glutMainLoop();
	free(pixelData_INTRO);
	return 0;
}