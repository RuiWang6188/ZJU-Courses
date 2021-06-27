#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include<cstdbool>
#include<cstring>
#include<gl/glut.h>
#include<cstdio>
#include<cstdlib>

class texture {

public:
    GLuint width;
    GLuint height;
    GLuint format;
    GLubyte* data;

public:
    bool loadTga(const char* path);

};


GLuint loadTexture(const char* path);