#pragma once
#include<gl/freeglut.h>
#include<gl/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>


/*******  Mode Macro Definition   *******/
#define MODE_INTRO 0
#define MODE_HELP 1
#define MODE_GAME 2
#define MODE_OVER 3


/******** some utility functions declaration ********/
void display_string(int x, int y, const char* string, int font);
void ReadBmpFile(const char* fileName, GLubyte*& pixelData);



/******** functions in main.cpp declaration ********/
void init(void);
void display(void);
void onMouse(int button, int state, int x, int y);
void passiveMouse(int x, int y);
void myMenu(int value);

