#ifndef _BASEELEMENT_H
#define _BASEELEMENT_H

#define FREEGLUT_STATIC
#include"texture.h"
#include"GameState.h"
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "texture.h"
#include <math.h>
//may need texture
/**************base define and implement********************/
#define grid_num 25
GLfloat sub_window_size = 150;
#define grid_size 300 //188 //188*2
int score = 0;
Texture My_texture;

/*
只有>0 才表示当前的grid可以通过，同时也是画墙的标准
ctrl_grid 1: can Go with nothing
ctrl_grid 2: Coin
ctrl_grid 3：stone
*/
#define NOTHING 1
#define COIN    2
#define ISROD_  3 //朝向
#define ISROD_1 4
#define fire_   5
#define fire_1  6

#define NORMAL 1
#define BLANK 2
GLfloat Light_Position[8][4] = {
	{0,grid_size,0,1},{grid_size*grid_num,grid_size,grid_size/2,1},{0,grid_size,grid_size*grid_num,1},{grid_size*grid_num,grid_size,grid_size*grid_num,1},
	{0,grid_size,0,1},{0,grid_size,0,1},{0,grid_size,0,1},{0,grid_size,0,1}
};
GLfloat Light_Color[12][4] = {
	{1,1,1,1},{1,0,0,1},{0,1,0,1},{0,0,1,1},
	{1,1,0,1},{0,1,1,1},{1,0,1,1},{0.8f,0.5f,0.3f,1},
	{0.5f,0.5f,1.0f,1},{0.75f,0.5f,1.0f,1},{1.0f,0.5f,1.0f,1},{1.0f,0.5f,0.75f,1}
};
static GLfloat Light_Strenth[11] = { 0,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f,0.8f,0.9f,1.0f };

class Light {
public:
	int light_num;
	GLfloat L_x, L_y, L_z;//光源位置 相对于房间来说
	GLfloat Size;
	//int dir_or_positon;
	GLfloat d_x, d_y, d_z;//direction

	int light_type;//0：聚光灯  1：可改变定点光源  2：平行光
	int li, lo, lp;//对应变化的三个变量  lp位置  lo颜色  li强度

	Light() {
		Size = 5;
		//dir_or_positon = 1;
		li=10, lo=0, lp=0;
		light_type = 0;
		//L_x = grid_size / 2;
		//L_y = 3 * grid_size;
		//L_z = -grid_size / 2;
	}
	void set_light_position(GLfloat x,GLfloat y,GLfloat z, GLfloat gx, GLfloat gy, GLfloat gz,int type) {
		L_x = x, L_y = y, L_z = z;
		d_x = gx - x, d_y = gy - y, d_z = gz - z;
	}
	void Draw_Light() {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glutSolidSphere(Size, 10, 10);
		glPopMatrix();
	}
	void set_Light() {
		glEnable(GL_LIGHTING);   //开关:使用光
		glEnable(GL_LIGHT0 + light_num);     //打开0#灯
		glEnable(GL_DEPTH_TEST);
		//材质反光性设置
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  //镜面反射参数
		GLfloat mat_shininess[] = { 128.0 };               //高光指数
		//GLfloat light_position[] = { grid_size*grid_num/2, grid_size*grid_num, -grid_size*grid_num/2, 1.0 };
		GLfloat light_position[] = {1, 1, -1,1};
		GLfloat spot_position[] = { 0.0, 0.0, -1 };
		GLfloat the_light_color[] = { 1,1,1,1 };

		if (light_type == 0) {//可移动聚光灯
			light_position[0] = L_x, light_position[1] = L_y, light_position[2] = L_z;
			light_position[3] = 1;
			spot_position[0] = (GLfloat)d_x/(grid_size/2) ; spot_position[1] = 0; spot_position[2] = (GLfloat)d_z/ (grid_size / 2);
			glLightf(GL_LIGHT0 + light_num, GL_SPOT_CUTOFF, 45);//设置聚光灯的角度,只对位置光源有效
			
		}
		else if (light_type == 1) {
			light_position[0] = Light_Position[lp][0], light_position[1] = Light_Position[lp][1], light_position[2] = Light_Position[lp][2];
			light_position[3] = 1;
			spot_position[0] = 0; spot_position[1] = -1; spot_position[2] = 0;
			glLightf(GL_LIGHT0 + light_num, GL_SPOT_CUTOFF, 45);//点辐射光源，设置聚光灯的角度,只对位置光源有效
		}
		else {
			//平行光
			light_position[0] = Light_Position[lp][0], light_position[1] = Light_Position[lp][1], light_position[2] = Light_Position[lp][2];
			light_position[3] = 0;//无限远；
			glLightf(GL_LIGHT0 + light_num, GL_SPOT_CUTOFF, 180);//点辐射光源，设置聚光灯的角度,只对位置光源有效
		}
		
		//最后一个参数为0表示该光源是directional
		 //最后一个参数非0表示该光源是position
		GLfloat Light_Model_Ambient[] = { 0.0 , 0.0 , 0.0 , 1.0 }; //环境光参数
		GLfloat light_ambient[] = { 1, 1, 1, 1.0 };//0的环境强度

		//材质属性
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		
		the_light_color[0]= Light_Color[lo][0] * Light_Strenth[li];
		the_light_color[1]= Light_Color[lo][1] * Light_Strenth[li];
		the_light_color[2]= Light_Color[lo][2] * Light_Strenth[li];

		//灯光设置
		glLightfv(GL_LIGHT0 + light_num, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0 + light_num, GL_SPOT_DIRECTION, spot_position);//指定聚光灯的方向
		glLightf(GL_LIGHT0 +  light_num, GL_SPOT_EXPONENT, 0.8);//聚光指数，光锥中心的光强最大，越靠边的光强越小。 缺省值 均匀照射
		glLightfv(GL_LIGHT0 + light_num, GL_POSITION, light_position);//
		glLightfv(GL_LIGHT0+  light_num, GL_DIFFUSE, the_light_color);   //散射光属性
		glLightfv(GL_LIGHT0 + light_num, GL_SPECULAR, the_light_color);  //镜面反射光
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);  //默认的全局光参数,只要设置为黑色就可以

		//衰减
		//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
		//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
		//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);
		glDepthFunc(GL_LESS); //glDepthFunc()来设置深度测试函数
		
	}
	void close() {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0 + light_num);
	}

};
Light the_light[7];

/*Particle*/

#define MAX_PARTICLES 1000 //最大的粒子数

class particle {
public:
	bool active;
	GLfloat fire_grid_size;
	float life;//lift time 
	float fade;//the decreasing of life
	float r, g, b;//RGB
	float x, y, z;//position
	float xi, yi, zi;//mov direction
	float xg, yg, zg;//Acceleration
};

static GLfloat colors[12][3] =     // Rainbow Of Colors 
{
  {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
  {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
  {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};

class Particles {
public:
	GLfloat length, wide, height;
	particle paticle[MAX_PARTICLES];
	float xspeed, yspeed, zspeed;
	GLuint loop, col, delay, PAR_texture;
	float slowdown = 2.0f;
	GLint area = 1000;
	Particles() {
			length = grid_size ;
			wide = grid_size / 16;
			height = grid_size / 24;
	}
	void init_particle() {
		xspeed = 0, zspeed = 0;
		yspeed = 10;

		for (loop = 0; loop < MAX_PARTICLES; ++loop)
		{
			paticle[loop].fire_grid_size = 4.5f;
			paticle[loop].active = true;
			paticle[loop].life = 1.0f;//full life is 1.0f
			//fe decrease rate(a random value add 0.003f : never equals 0)
			paticle[loop].fade = float(rand() % 100) / 100.0f + 0.003f;

			paticle[loop].x = float((rand() % area) - area / 2) * length / (area); //
			paticle[loop].y = 0;// float((rand() % 50) - 50) * grid_size / 50;
			paticle[loop].z = float((rand() % area) - area / 2) *wide / (area);

			paticle[loop].r = colors[0][0];
			paticle[loop].g = colors[0][1];
			paticle[loop].b = colors[0][2];

			paticle[loop].xi = 0;// float((rand() % 50) - 25.0f) * 100.0f;
			paticle[loop].yi = float((rand() % 50) - 25.0f) * 100.0f;
			paticle[loop].zi = 0;// float((rand() % 50) - 25.0f) * 100.0f;

			paticle[loop].xg = 0.0f;
			paticle[loop].yg = 4.2f;
			paticle[loop].zg = 0.f;
		}
	}
	void draw_particle() {
		for (loop = 0; loop < MAX_PARTICLES; ++loop)
		{
			if (paticle[loop].active)//the particle is alive
			{
				float x = paticle[loop].x;
				float y = paticle[loop].y;
				//our scene is moved into the screen
				float z = paticle[loop].z;


				glColor4f(paticle[loop].r, paticle[loop].g, paticle[loop].r,
					//use life as alpha value:
					//as particle dies,it becomes more and more transparent
					paticle[loop].life);

				//draw particles : use triangle strip instead of quad to speed
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, My_texture.fire_texture);
				glBegin(GL_TRIANGLE_STRIP);
				//top right
				glTexCoord2f(0, 0);
				glVertex3f(x - paticle[loop].fire_grid_size, y - paticle[loop].fire_grid_size / 2, z);
				//top left
				glTexCoord2f(0, 1);
				glVertex3f(x - paticle[loop].fire_grid_size, y + paticle[loop].fire_grid_size / 2, z);
				//bottom right
				glTexCoord2f(1, 0);
				glVertex3f(x + paticle[loop].fire_grid_size, y + paticle[loop].fire_grid_size / 2, z);
				//bottom left
				glTexCoord2f(1, 1);
				glVertex3f(x + paticle[loop].fire_grid_size, y - paticle[loop].fire_grid_size / 2, z);

				//top right
				glTexCoord2d(0, 0);
				glVertex3f(x, y - paticle[loop].fire_grid_size / 2, z - paticle[loop].fire_grid_size / 2);
				//top left
				glTexCoord2d(0, 1);
				glVertex3f(x, y - paticle[loop].fire_grid_size / 2, z + paticle[loop].fire_grid_size / 2);
				//bottom right
				glTexCoord2d(1, 0);
				glVertex3f(x, y + paticle[loop].fire_grid_size / 2, z + paticle[loop].fire_grid_size / 2);
				//bottom left
				glTexCoord2d(1, 1);
				glVertex3f(x, y + paticle[loop].fire_grid_size / 2, z - paticle[loop].fire_grid_size / 2);

				glEnd();
				glDisable(GL_TEXTURE_2D);

				paticle[loop].fire_grid_size -= 0.01f;

				//Move On The X Axis By X Speed 
				paticle[loop].x += paticle[loop].xi / (slowdown * 100);
				//Move On The Y Axis By Y Speed 
				paticle[loop].y += paticle[loop].yi / (slowdown * 100);
				//Move On The Z Axis By Z Speed 
				paticle[loop].z += paticle[loop].zi / (slowdown * 100);

				//add gravity or resistance : acceleration
				paticle[loop].xi += paticle[loop].xg;
				paticle[loop].yi += paticle[loop].yg;
				paticle[loop].zi += paticle[loop].zg;

				//decrease particles' life
				paticle[loop].life -= paticle[loop].fade;

				if (paticle[loop].y >= grid_size / 2) {
					paticle[loop].life = -1;
				}

				//if particle is dead,rejuvenate it
				if (paticle[loop].life < 0.0f)
				{
					paticle[loop].fire_grid_size = 3.5f;

					paticle[loop].life = 1.0f;//alive again
					paticle[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;

					//reset its position
					paticle[loop].x = float((rand() % area) - area / 2) * grid_size / (area); //
					paticle[loop].y = 0;// float((rand() % 50) - 50) * grid_size / 50;
					paticle[loop].z = float((rand() % area) - area / 2) *grid_size / (area * 16);

					//X Axis Speed And Direction 
					paticle[loop].xi = 0;// xspeed + float((rand() % 60) - 32.0f);
					// Y Axis Speed And Direction 
					paticle[loop].yi = yspeed + float((rand() % 60) - 30.0f);
					// Z Axis Speed And Direction 
					paticle[loop].zi = 0;// float((rand() % 60) - 30.0f);

					paticle[loop].r = colors[col][0];			// Select Red From Color Table
					paticle[loop].g = colors[col][1];			// Select Green From Color Table
					paticle[loop].b = colors[col][2];			// Select Blue From Color Table
				}
			}
		}
	}
};

Particles the_particle;

class ROD {
public:
	GLfloat length, wide, height;
	ROD() {
		length = grid_size;
		wide = grid_size / 5;
		height = grid_size / 5;
	}
	void draw_ROD(){
			glColor3f(0, 1, 0);
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//150, 75, 0
			GLfloat R = 150.0 / 255, G = 75.0 / 255, B = 0;
			GLfloat mat_ambient[] = { R, G, B, 1.0f };
			GLfloat mat_diffuse[] = { R, G, B, 1.0f };
			GLfloat mat_specular[] = { R, G, B, 1.0f };
			GLfloat mat_emission[] = { R, G, B, 0.0f };
			GLfloat mat_shininess[] = { 128.f };
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
			glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

			glBindTexture(GL_TEXTURE_2D, My_texture.wall_texture);
			glBegin(GL_QUADS);// GL_POLYGON);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(-length/2, -height/2, wide/2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-length / 2, height / 2, wide / 2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(+length / 2, height / 2, wide / 2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(+length / 2, -height / 2, wide / 2);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, -height / 2, -wide / 2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-length / 2, height / 2, -wide / 2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(+length / 2, height / 2, -wide / 2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(+length / 2, -height / 2, -wide / 2);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, height / 2, wide / 2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(length / 2, height / 2, wide / 2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(length / 2, height / 2, -wide / 2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-length / 2, height / 2, -wide / 2);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, -height / 2, wide / 2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(length / 2, -height / 2, wide / 2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(length / 2, -height / 2, -wide / 2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-length / 2, -height / 2, -wide / 2);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
};
ROD the_rod;
class grid {
public:
	int walltype[4];// 0前 1后 2左 3右边  若值为0 表示没有墙
	int type;
	int is_rod;
	/*
	-1:can not go
	0:normal
	1:coin
	...
	*/
	GLfloat position[3];//zΪ0
	int livestate;//the state of live


	void init(GLfloat x, GLfloat y, GLfloat z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		livestate = 1;
		memset(walltype, 0, sizeof(int) * 4);
		type = NORMAL;//
		is_rod = 0;
	}
	void draw_grid() {
		glPushMatrix();
		glPushMatrix();

		GLfloat mat_ambient[] = { 0.5f, 0.5f, .5f, 0.10f };
		GLfloat mat_diffuse[] = { 0.5f, 0.5f, .5f, 1.0f };
		GLfloat mat_specular[] = { 0.5f, 0.5f, 1.5f, 1.0f };
		GLfloat mat_emission[] = { 0.5, 0.5f, 1.f, 0.0f };
		GLfloat mat_shininess = 128.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT,mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION,mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

		if (is_rod != 0) {
			glTranslatef(position[0],position[1]+grid_size,position[2]);
			if (is_rod == ISROD_1) {
				glRotatef(90, 0, 1, 0);
			}
			the_rod.draw_ROD();
		}
		glPopMatrix();

		glColor3f(0, 0.3, 0.4);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, My_texture.ground_texture);
		glBegin(GL_QUADS);// GL_POLYGON);
			if (type != BLANK) {
				glTexCoord2f(0.0f, 0.0f); glVertex3f(position[0] - grid_size / 2, position[1], position[2] + grid_size / 2);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(position[0] - grid_size / 2, position[1], position[2] - grid_size / 2);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(position[0] + grid_size / 2, position[1], position[2] - grid_size / 2);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(position[0] + grid_size / 2, position[1], position[2] + grid_size / 2);
			}
			

			//roof
			glVertex3f(position[0] - grid_size / 2, position[1] + 3*grid_size, position[2] - grid_size / 2);
			glVertex3f(position[0] - grid_size / 2, position[1] + 3 * grid_size, position[2] - grid_size / 2);
			glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] - grid_size / 2);
			glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] + grid_size / 2);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	
	void draw_wall(GLint text) {
		glColor3f(1, 0, 0);
		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		GLfloat mat_ambient[] = { 1.f, 0.5f,0.2f, 1.0f };
		GLfloat mat_diffuse[] = { 1.0f, 0.5f, 0.2f, 1.0f };
		GLfloat mat_specular[] = { 1.0f, 0.5f, 0.2f, 1.0f };
		GLfloat mat_emission[] = { 1, 0.5f, 0.2f, 0.0f };
		GLfloat mat_shininess = 80.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,mat_emission);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		
		glEnable(GL_TEXTURE_2D);
		//text = My_texture.fire_texture;
		if(walltype[0]!=0){//前
			glBindTexture(GL_TEXTURE_2D, text);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(position[0] - grid_size / 2, position[1] , position[2] - grid_size / 2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(position[0] - grid_size / 2, position[1] + 3 * grid_size, position[2] - grid_size / 2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] - grid_size / 2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(position[0] + grid_size / 2, position[1] , position[2] - grid_size / 2);
			glEnd();
		}
		if (walltype[1] != 0) {//后
			glBindTexture(GL_TEXTURE_2D, text);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(position[0] - grid_size / 2, position[1], position[2] + grid_size / 2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(position[0] - grid_size / 2, position[1] + 3 * grid_size, position[2] + grid_size / 2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] + grid_size / 2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(position[0] + grid_size / 2, position[1], position[2] + grid_size / 2);
			glEnd();
		}
		if (walltype[2] != 0) {//左
			glBindTexture(GL_TEXTURE_2D, text);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(position[0] - grid_size / 2, position[1], position[2] + grid_size / 2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(position[0] - grid_size / 2, position[1]+ 3 * grid_size, position[2] + grid_size / 2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(position[0] - grid_size / 2, position[1] + 3 * grid_size, position[2] - grid_size / 2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(position[0] - grid_size / 2, position[1], position[2] - grid_size / 2);
			glEnd();
		}
		if (walltype[3] != 0) {//右边
			glBindTexture(GL_TEXTURE_2D, text);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(position[0] + grid_size / 2, position[1], position[2] + grid_size / 2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] + grid_size / 2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] - grid_size / 2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(position[0] + grid_size / 2, position[1], position[2] - grid_size / 2);			
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	void draw_wall() {
		glPushMatrix();
		glColor3f(1, 0, 0);
		if (walltype[0] != 0) {//前
			glBegin(GL_QUADS);
				glVertex3f(position[0] - grid_size / 2, position[1], position[2] - grid_size / 2);
				glVertex3f(position[0] - grid_size / 2, position[1] + 3*grid_size, position[2] - grid_size / 2);
				glVertex3f(position[0] + grid_size / 2, position[1] + 3*grid_size, position[2] - grid_size / 2);
				glVertex3f(position[0] + grid_size / 2, position[1], position[2] - grid_size / 2);
			glEnd();
		}
		if (walltype[1]!= 0) {//后
			glBegin(GL_QUADS);
				glVertex3f(position[0] - grid_size / 2, position[1], position[2] + grid_size / 2);
				glVertex3f(position[0] - grid_size / 2, position[1] + 3 * grid_size, position[2] + grid_size / 2);
				glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] + grid_size / 2);
				glVertex3f(position[0] + grid_size / 2, position[1], position[2] + grid_size / 2);
			glEnd();
		}
		if (walltype[2] != 0) {//左
			glBegin(GL_QUADS);
				glVertex3f(position[0] - grid_size / 2, position[1], position[2] + grid_size / 2);
				glVertex3f(position[0] - grid_size / 2, position[1] + 3 * grid_size, position[2] + grid_size / 2);
				glVertex3f(position[0] - grid_size / 2, position[1] + 3 * grid_size, position[2] - grid_size / 2);
				glVertex3f(position[0] - grid_size / 2, position[1], position[2] - grid_size / 2);
			glEnd();
		}
		if (walltype[3] != 0) {//右边
			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(position[0] + grid_size / 2, position[1], position[2] + grid_size / 2);
				glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] + grid_size / 2);
				glVertex3f(position[0] + grid_size / 2, position[1] + 3 * grid_size, position[2] - grid_size / 2);
				glVertex3f(position[0] + grid_size / 2, position[1], position[2] - grid_size / 2);			
			glEnd();
		}
		glPopMatrix();
	}

};
class coin {
public:
	GLfloat Size;
	coin() {
		Size = grid_size / 10;
	}
	void draw_coin() {
		glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//GL_MODULATE);
		glColor3f(1, 0, 0);
		GLfloat mat_ambient[] = { 0.8f, 0.f,   0.0f, 0.3f };
		GLfloat mat_diffuse[] = { 0.8f, 0.0f,  0.0f, 0.3f };
		GLfloat mat_specular[] = { 0.8f, 0.0f, 0.0f, 0.3f };
		GLfloat mat_emission[] = { 0.8f, 0.f,  0.f, 0.0f };
		GLfloat mat_shininess = 80.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, My_texture.fire_texture );

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,-Size*2, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(Size/2, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0, Size*2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-Size/2, 0 , 0);
		glEnd();
		//glutWireSphere(Size, 10, 10);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

};
class grass {
public:
	GLfloat Size;
	grass() {
		Size = grid_size / 8;
	}
	void draw_grass() {
		glPushMatrix();
		glColor3f(1, 0, 0);
		GLfloat mat_ambient[] = { 0.8f, 0.f,   0.0f, 1.0f };
		GLfloat mat_diffuse[] = { 0.8f, 0.0f,  0.0f, 1.0f };
		GLfloat mat_specular[] = { 0.8f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_emission[] = { 0.8f, 0.f,  0.f, 0.0f };
		GLfloat mat_shininess = 100.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	}

};
class fire{
public:
	GLfloat Size;
	fire() {

	}
	void draw_fire() {

	}
};

/******global variable****************/
/*
#define NOTHING 1
#define COIN    2
#define ISROD_  3 //朝向
#define ISROD_1 4
#define fire_   5
#define fire_1  6
*/
grid mygrid[grid_num][grid_num];//25 x 25 each 25 x 25      
int ctrl_init_grid[grid_num][grid_num] = {//easy to decide the type  注意这里的i列 对应_ 另外的对应| 指的是rod 和fir 类型
	{1,1,1,1,3,1,1,2,1,1,3,1,2,1,3,1,1,2,1,1,3,1,1,2,1},
	{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
	{1,0,2,1,2,5,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,5,2,0,1},
	{1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
	{1,0,4,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,2,0,1},
	{1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1},
	{1,0,2,0,4,0,2,1,2,1,3,1,2,1,3,1,2,1,2,0,2,0,2,0,1},
	{1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,6,0,1,0,1,0,1},
	{1,1,2,1,2,1,2,1,2,1,3,1,2,1,1,1,2,1,2,1,2,1,2,1,1},
	{1,0,4,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,4,0,1},
	{1,0,2,0,4,0,2,0,6,0,0,0,1,0,0,0,6,0,2,0,6,0,2,0,1},
	{1,0,1,0,1,0,1,0,1,0,0,2,2,2,0,0,1,0,1,0,1,0,1,0,1},
	{1,0,2,0,2,0,2,0,2,1,1,2,2,2,1,1,2,0,2,0,2,0,2,0,1},
	{1,0,4,0,2,0,1,0,1,0,0,2,2,2,0,0,1,0,1,0,2,0,4,0,1},
	{1,0,2,0,6,0,2,0,2,0,0,0,1,0,0,0,2,0,2,0,6,0,2,0,1},
	{1,0,1,0,1,0,1,0,4,0,0,0,1,0,0,0,4,0,6,0,1,0,1,0,1},
	{1,1,2,1,2,1,2,1,2,1,1,2,1,1,2,1,2,1,2,1,1,1,2,1,1},
	{1,0,4,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,4,0,1},
	{1,0,2,0,2,0,2,1,2,1,1,2,1,1,2,1,2,1,1,0,2,0,2,0,1},
	{1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1},
	{1,0,2,0,4,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,2,0,1},
	{1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
	{1,0,4,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,5,2,0,1},
	{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
	{1,1,2,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,2,1,1}
};
int ctrl_grid[grid_num][grid_num];
int ground_grid[grid_num][grid_num];//easy to decide the type 

/*
0: 不可过 nothing
1:coin
2:no coin
*/
coin the_coin;


int check_coin(GLfloat x,GLfloat y,GLfloat z) {
 	if (ctrl_grid[int(x/grid_size)][int(z/grid_size)]) {
		if (ctrl_grid[int(x / grid_size)][int(z / grid_size)] == COIN) {//球体
			//这里应该写距离金币的距离。
			GLfloat Coin_x = int(x / grid_size) * grid_size+grid_size/2;
			GLfloat Coin_y = grid_size ;
			GLfloat Coin_z = int(z / grid_size) * grid_size+grid_size/2;
			if (sqrt((Coin_x - x)*(Coin_x - x) + (Coin_y - y)*(Coin_y - y) + (Coin_z - z)*(Coin_z - z)) < grid_size / 5) {
				ctrl_grid[int(x / grid_size)][int(z / grid_size)] = NOTHING;
				score++;
			}
		}
		if (mygrid[int(x / grid_size)][int(z / grid_size)].type == BLANK) {
			ground_type = BLANK;
		}
		else ground_type = NORMAL;
		return 1;
	}
	else return 0;
}

int Correct(int x) {
	if (x <= 0) return 0;
	if (x >= grid_num - 1) return grid_num - 1;
	return x;
}

void init_ctrl_grid() {
	int i, j;
	int p = 0;
	memcpy(ctrl_grid, ctrl_init_grid, sizeof(int)*grid_num*grid_num);
	//memset(ctrl_grid, 0, sizeof(int)*grid_num*grid_num);
	//初始化ground
	ground_grid[5][0] = BLANK;
	ground_grid[15][0] = BLANK;
	ground_grid[20][0] = BLANK;
	
	ground_grid[5][24] = BLANK;
	ground_grid[15][24] = BLANK;
	ground_grid[20][24] = BLANK;
	/*for (i = 0; i < grid_num; i++) {
		for (j = 0; j < grid_num; j++) {
			if (i == 0 || i == grid_num - 1) {
				ctrl_grid[i][j] = NOTHING;
				p++;
				if(p%3==0) ctrl_grid[i][j] = COIN;
			}
			if (j % 5 == 0 || j == grid_num - 1) {
				ctrl_grid[i][j] = NOTHING;
				p++;
				if (p % 3 == 0) ctrl_grid[i][j] = COIN;
			}
		}
	}
	*/
}
class AABB_2D {
public:
	GLfloat min_x, max_x;
	GLfloat min_y, max_y;
	void set(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2) {
		min_x = x1;
		max_x = x2;
		min_y = y1;
		max_y = y2;
	}
};

bool check_2D_collide(AABB_2D A,AABB_2D B) {
	if (A.max_x < B.min_x) return 0;
	if (A.max_y < B.min_y) return 0;
	if (B.max_x < A.min_x) return 0;
	if (B.max_y < A.min_y) return 0;

	return 1;
}
int check_collide(GLfloat x, GLfloat y, GLfloat z) {//碰撞检测 
	/*
	AABB 盒 
	人体：
	长:grid_size/2;
	宽:grid_size/2;
	高:grid_size; //这个高度意味着不蹲下绝对会碰撞
	
	柱子、fire：
	length = grid_size;
	wide = grid_size / 5;
	height = grid_size / 5;

	*/
	//柱子中心位置
 	int flag = 0;
	GLfloat Rod_x = int(x / grid_size) * grid_size + grid_size / 2;
	GLfloat Rod_y = grid_size;
	GLfloat Rod_z = int(z / grid_size) * grid_size + grid_size / 2;
	bool checkxy=0, checkxz=0, checkyz=0;
	AABB_2D A_xy, A_xz, A_yz;
	AABB_2D B_xy, B_xz, B_yz;
	if (ctrl_grid[int(x / grid_size)][int(z / grid_size)] == ISROD_) {
		Rod_y = grid_size;
		A_xy.set(Rod_x-the_rod.length/2,Rod_x+the_rod.length/2,Rod_y-the_rod.height/2,Rod_y+the_rod.height/2); //xy
		A_xz.set(Rod_x - the_rod.length / 2, Rod_x + the_rod.length/2, Rod_z - the_rod.wide / 2, Rod_z + the_rod.wide / 2);
		A_yz.set(Rod_y - the_rod.height / 2, Rod_y+ the_rod.height/2, Rod_z - the_rod.wide / 2, Rod_z + the_rod.wide / 2);
		flag = 1;
	}
	else if (ctrl_grid[int(x / grid_size)][int(z / grid_size)] == ISROD_1) {
		Rod_y = grid_size;
		A_xy.set (Rod_x - the_rod.wide / 2, Rod_x + the_rod.wide/ 2, Rod_y - the_rod.height / 2, Rod_y + the_rod.height/ 2); //xy
		A_xz.set (Rod_x - the_rod.wide / 2, Rod_x + the_rod.wide  / 2, Rod_z - the_rod.length / 2, Rod_z + the_rod.length / 2);
		A_yz.set (Rod_y - the_rod.height / 2, Rod_y + the_rod.height / 2, Rod_z - the_rod.length / 2, Rod_z + the_rod.length / 2);
		flag = 1;
	}
	else if (ctrl_grid[int(x / grid_size)][int(z / grid_size)] == fire_1) {
		Rod_y = 0;
		A_xy.set(Rod_x - the_particle.wide / 2, Rod_x + the_particle.wide / 2, Rod_y- the_particle.height/2, Rod_y +the_particle.height/2); //xy
		A_xz.set(Rod_x - the_particle.wide / 2, Rod_x + the_particle.wide / 2, Rod_z - the_particle.length / 2, Rod_z + the_particle.length / 2);
		A_yz.set(Rod_y- the_particle.height/2, Rod_y + the_particle.height/2 , Rod_z - the_particle.length / 2, Rod_z + the_particle.length / 2);
		flag = 2;
	}
	if (ctrl_grid[int(x / grid_size)][int(z / grid_size)] == fire_) {
  		Rod_y = grid_size / (3 * 2);
		A_xy.set(Rod_x - the_particle.length / 2, Rod_x + the_particle.length / 2, Rod_y - the_particle.height/2, Rod_y + the_particle.height/2); //xy
		A_xz.set(Rod_x - the_particle.length / 2, Rod_x + the_particle.length / 2, Rod_z - the_particle.wide / 2, Rod_z + the_particle.wide / 2);
		A_yz.set(Rod_y - the_particle.height/2, Rod_y + the_particle.height/2 , Rod_z - the_particle.wide / 2, Rod_z + the_particle.wide / 2);
		flag = 2;
	}
	if (flag) {

		B_xy.set(x - grid_size / 4, x + grid_size / 4, y -grid_size, y);
		B_xz.set(x - grid_size / 4, x + grid_size / 4, z - grid_size / 4, z + grid_size / 4);
		B_yz.set(y-grid_size, y , z - grid_size / 4, z + grid_size / 4);
		checkxy =check_2D_collide(A_xy,B_xy) ;
		checkxz = check_2D_collide(A_xz, B_xz);
		checkyz = check_2D_collide(A_yz, B_yz);
	}
	if (checkxy&&checkxz&&checkyz) {
		return flag;
	}

	return 0;
}


#endif