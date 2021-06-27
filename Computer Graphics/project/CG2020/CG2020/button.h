#pragma once

#include<stdio.h>
#include<iostream>
#include<gl/freeglut.h>
#include<gl/glut.h>
#include"utility.h"

#define BUTTON_NORMAL 0
#define BUTTON_FOCUS 1
#define BUTTON_CLICK 2

extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern int mouseX;
extern int mouseY;
extern bool isClick;

using namespace std;

class button {
public:
	int state;
	button(GLfloat x, GLfloat y, GLfloat width, GLfloat height, const char * data) {
		int i;

		this->centerX = x;
		this->centerY = y;
		this->width = width;
		this->height = height;
		this->state = BUTTON_NORMAL;

		vertexList[0][0] = centerX - width / 2;
		vertexList[0][1] = centerY - height / 2;
		vertexList[1][0] = centerX + width / 2;
		vertexList[1][1] = centerY - height / 2;
		vertexList[2][0] = centerX + width / 2;
		vertexList[2][1] = centerY + height / 2;
		vertexList[3][0] = centerX - width / 2;
		vertexList[3][1] = centerY + height / 2;

		for (i = 0; data[i] != '\0'; i++)
			content[i] = data[i];
		content[i] = '\0';
	}
	void drawButton() {
		state = isInButton() ? (isClick ? BUTTON_CLICK : BUTTON_FOCUS) : BUTTON_NORMAL;

		switch (state) {
		case BUTTON_NORMAL:
			glColor3f(0.7098, 0.7098, 0.7098);
			break;
		case BUTTON_FOCUS:
			glColor3f(0.7843, 0.7843, 0.7843);
			break;
		case BUTTON_CLICK:
			glColor3f(0.67, 0.67, 0.67);
			break;
		default:
			glColor3f(1, 0, 0);
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, SCR_WIDTH, SCR_HEIGHT, 0.0);
		glBegin(GL_POLYGON);
		glVertex2fv(vertexList[0]);
		glVertex2fv(vertexList[1]);
		glVertex2fv(vertexList[2]);
		glVertex2fv(vertexList[3]);
		glEnd();

		glColor3f(1, 1, 1);
		display_string(vertexList[0][0] + this->width / 5, vertexList[0][1] + this->height / 1.5, content, 2);
	}

	bool isInButton() {
		if (mouseX >= (centerX - width / 2) && mouseX <= (centerX + width / 2)
			&& mouseY >= (centerY - height / 2) && mouseY <= (centerY + height / 2)) {
			cout << "is in button !" << endl;
			return 1;
		}
		else
			return 0;
	}

private:
	GLfloat centerX;
	GLfloat centerY;
	GLfloat width;
	GLfloat height;
	GLfloat vertexList[4][2];
	char content[50];
};