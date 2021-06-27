#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define FREEGLUT_STATIC

#pragma once

#include<cstdlib>
#include <stdio.h>	
#include <gl/freeglut.h>
#include<gl/GL.h>
#include<stdlib.h>
#include<gl/glut.h>
#include<cmath>
#include"camera.h"
#include"planet.h"
#include"tga.h"
#include"initialize.h"
#include"draw.h"

#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib"
#else
#define GL_Lib(name) name ".lib"
#endif
#endif
#pragma comment(lib, GL_Lib("freeglut_static"))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double lastX = 300, lastY = 400;
bool isPlanetRun = true;
float timeFactor = 1.0f;