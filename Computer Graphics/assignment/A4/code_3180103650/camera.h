#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define FREEGLUT_STATIC

#include <stdio.h>	
#include <gl/freeglut.h>
#include<gl/GL.h>
#include<gl/glut.h>
#include<stdlib.h>
#include<cmath>


#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib"
#else
#define GL_Lib(name) name ".lib"
#endif
#endif
#pragma comment(lib, GL_Lib("freeglut_static"))

using namespace std;

class Camera
{
public:
	Camera() {
		camera_x = 10;
		camera_y = 0;
		camera_z = 5;

		ref_x = 0;
		ref_y = 0;
		ref_z = 0;

		upVec_x = 0;
		upVec_y = 0;
		upVec_z = 1;
	}
	~Camera() {

	}

	GLfloat camera_x;
	GLfloat camera_y;
	GLfloat camera_z;

	GLfloat ref_x;
	GLfloat ref_y;
	GLfloat ref_z;

	GLfloat upVec_x;
	GLfloat upVec_y;
	GLfloat upVec_z;

	const GLfloat adjust_step = 0.5;

private:	
};


