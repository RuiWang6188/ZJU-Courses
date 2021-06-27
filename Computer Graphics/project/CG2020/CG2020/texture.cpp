#pragma once
#include"texture.h"

GLuint SOIL_load_texture(const char* fileName)
{
	int width, height;
	GLint last_texture_ID;
	GLuint texture_ID = 0;
	unsigned char* image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &texture_ID);

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);//在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//指当纹理图像被使用到一个大于它的形状上时,应该如何处理
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);
	return texture_ID;
}

void Texture::Draw_Skybox(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLfloat length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;

	// Draw Front side
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[SKYFRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[SKYBACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[SKYLEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[SKYRIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z + length);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[SKYUP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[SKYDOWN]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length); // glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);	//glVertex3f(x, y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);	//glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);   //glVertex3f(x + width, y + height, z + length);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
