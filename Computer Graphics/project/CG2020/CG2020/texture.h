#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include<math.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>
//带d是
//当前是dbug编译，就在后面加d

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/

#define SKYFRONT 0						// Give Front ID = 0
#define SKYBACK  1						// Give Back  ID = 1
#define SKYLEFT  2						// Give Left  ID = 2
#define SKYRIGHT 3						// Give Right ID = 3
#define SKYUP    4						// Give Up    ID = 4
#define SKYDOWN  5						// Give Down  ID = 5

#define OBJTYPE1 1	// f X/X/X Y/Y/Y Z/Z/Z
#define OBJTYPE2 2	// f X Y Z
#define OBJTYPE3 3

#define BMP_Header_Length 54  //图像数据在内存块中的偏移量
#define TEXTURE_NUM 5
bool power_of_two(int n);
GLuint load_texture(const char* file_name);
GLuint SOIL_load_texture(const char* fileName);

class Texture {
public:
	void init() {
		wall_texture = SOIL_load_texture("images/wall.bmp");
		fire_texture = SOIL_load_texture("images/Particle.bmp");
		ground_texture = SOIL_load_texture("images/ground.bmp");
		grass_texture=SOIL_load_texture("images/grass.bmp");
		coin_texture = SOIL_load_texture("images/coin.bmp");

		// load skybox images and generate textures
		skyboxTexture[SKYFRONT] = SOIL_load_texture("./images/sky_texture/skybox3_pz.bmp");
		skyboxTexture[SKYBACK] = SOIL_load_texture("./images/sky_texture/skybox3_nz.bmp");
		skyboxTexture[SKYLEFT] = SOIL_load_texture("./images/sky_texture/skybox3_nx.bmp");
		skyboxTexture[SKYRIGHT] = SOIL_load_texture("./images/sky_texture/skybox3_px.bmp");
		skyboxTexture[SKYUP] = SOIL_load_texture("./images/sky_texture/skybox3_py.bmp");
		skyboxTexture[SKYDOWN] = SOIL_load_texture("./images/sky_texture/skybox3_ny.bmp");

		// game mode texture
		mode_texture[0] = SOIL_load_texture("./images/INTRO.bmp");
		mode_texture[1] = SOIL_load_texture("./images/HELP.bmp");
		mode_texture[2] = SOIL_load_texture("./images/OVER.bmp");

		// obj texture
		obj_texture[0] = SOIL_load_texture("./images/ufo_texture.jpg");

	}
	GLint wall_texture;
	GLint fire_texture;
	GLint ground_texture;
	GLint grass_texture;
	GLint coin_texture;
	GLint skyboxTexture[6];
	GLint mode_texture[TEXTURE_NUM];
	GLint obj_texture[TEXTURE_NUM];
	void Draw_Skybox(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLfloat length);
};
