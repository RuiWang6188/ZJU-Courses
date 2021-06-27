#pragma once
#include<gl/glut.h>
#include<cmath>
#include"initialize.h"
#include"header.h"

#define MOON_ORBIT_SCALE 150
#define GLOBAL_ORBIT_SCALE 3

void draw();
void drawMilky();
void drawSun();
void drawEarth();
void drawMoon();
void drawMars();
void drawJupiter();

void draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //drawMilky();
    drawSun();
    drawEarth();
    drawMars();
    drawJupiter();
}


void drawSun() {

    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    // glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    //glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    // glEnable(GL_LIGHTING);
    // GLfloat noSunLight[]={0,0,5,0};
    // glLightfv(GL_LIGHT0, GL_POSITION, noSunLight);
    glRotatef(sun.curSelfAngle, 0, 1.0f, 0);
    glEnable(GL_DEPTH_TEST);
    // GLfloat noSunLight[]={0,0,0,1};
// glLightfv(GL_LIGHT0, GL_POSITION, noSunLight);
    glCallList(sun.displayList);
    glRotatef(-1 * sun.curSelfAngle, 0, 1.0f, 0);
    glMatrixMode(GL_MODELVIEW);
    // glMatrixMode(GL_PROJECTION);
    glPopMatrix();


    glPopAttrib();
    // glPopMatrix();
}

void drawMoon() {
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glRotatef(moon.curSelfAngle, 0, 1.0f, 0);
    glTranslatef(moon.orbitRadius * MOON_ORBIT_SCALE * sin(moon.curOrbitAngle), 0, moon.orbitRadius * MOON_ORBIT_SCALE * cos(moon.curOrbitAngle));
    glEnable(GL_DEPTH_TEST);
    glCallList(moon.displayList);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


void drawEarth() {
    glPushMatrix();
    // glLoadIdentity();
    glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glMatrixMode(GL_MODELVIEW);

    // glColor3f(0,1.0f,0);
    // glBegin(GL_POLYGON);
    // for (int i = 0; i < 10000; i++)
    // {
    // 	glVertexf(earth.orbitRadius*cos(2*3.1415926/10000*i), earth.orbitRadius*sin(2*3.1415926/10000*i));		
    // }
    // glEnd();


    // glPushMatrix();
    //glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat noSunLight[] = { 0,0,0,1 };
    glLightfv(GL_LIGHT0, GL_POSITION, noSunLight);
    glEnable(GL_DEPTH_TEST);


    glRotatef(earth.orbitAngle, 0, 0, 1);

    glTranslatef(earth.orbitRadius * sin(earth.curOrbitAngle) * GLOBAL_ORBIT_SCALE, 0, earth.orbitRadius * cos(earth.curOrbitAngle) * GLOBAL_ORBIT_SCALE);

    //glTranslatef(0, 0, 0);
    glRotatef(earth.selfAngle, 0, 0, 1.0);

    glPushMatrix();
    glRotatef(earth.curSelfAngle, 0, 1.0f, 0);
    glCallList(earth.displayList);


    glPopMatrix();
    glRotatef(-1 * earth.selfAngle, 0, 0, 1.0);
    drawMoon();

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
    // glPopMatrix();
}

void drawMars() {
    glPushMatrix();
    // glLoadIdentity();
    glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glMatrixMode(GL_MODELVIEW);

    //glPushMatrix();
    //glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat noSunLight[] = { 0,0,0,1 };
    glLightfv(GL_LIGHT0, GL_POSITION, noSunLight);
    glEnable(GL_DEPTH_TEST);

    glRotatef(mars.orbitAngle, 0, 0, 1);

    glTranslatef(mars.orbitRadius * sin(mars.curOrbitAngle) * GLOBAL_ORBIT_SCALE, 0, mars.orbitRadius * cos(mars.curOrbitAngle) * GLOBAL_ORBIT_SCALE);
    //glTranslatef(0, 0, 0);
    glRotatef(mars.selfAngle, 0, 0, 1.0);

    glPushMatrix();
    glRotatef(mars.curSelfAngle, 0, 1.0f, 0);
    glCallList(mars.displayList);

    glPopMatrix();

    glRotatef(-1 * mars.selfAngle, 0, 0, 1.0);

    //glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
    glPopMatrix();
}

void drawJupiter() {
    glPushMatrix();
    // glLoadIdentity();
    glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glMatrixMode(GL_MODELVIEW);

    //glPushMatrix();
    //glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat noSunLight[] = { 0,0,0,1 };
    glLightfv(GL_LIGHT0, GL_POSITION, noSunLight);
    glEnable(GL_DEPTH_TEST);

    glRotatef(jupiter.orbitAngle, 0, 0, 1);

    glTranslatef(jupiter.orbitRadius * sin(jupiter.curOrbitAngle) * GLOBAL_ORBIT_SCALE, 0, jupiter.orbitRadius * cos(jupiter.curOrbitAngle) * GLOBAL_ORBIT_SCALE);
    //glTranslatef(0, 0, 0);
    glRotatef(jupiter.selfAngle, 0, 0, 1.0);

    glPushMatrix();
    glRotatef(jupiter.curSelfAngle, 0, 1.0f, 0);
    glCallList(jupiter.displayList);

    glPopMatrix();

    glRotatef(-1 * jupiter.selfAngle, 0, 0, 1.0);

    //glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
    glPopMatrix();
}