#pragma once

#include<gl/glut.h>
#include"planet.h"
#include"tga.h"


// construct path of tga 
#define MAKE_TEXTURE_PATH(name) ("texture/" name ".tga")

#define SUN_RADIUS_SCALE 80
#define MOON_RADIUS_SCALE 10


// define planet item
planet sun;
planet moon;
planet earth;
planet mars;
planet jupiter;

void initialize();

// different type
void initPlanet();
void initTexture();
void initList();

// detailed display list initialization
void initSunDisplay();
void initEarthDisplay();
void initMoonDisplay();
void initMarsDisplay();
void initJupiterDisplay();

// ==============function definition ===================//



// main intialize function
void initialize() {
    initPlanet();
    initTexture();
    initList();
}

// initialize planet
void initPlanet() {
    //  iniialize sun
    sun.texturePath = (char*)MAKE_TEXTURE_PATH("sun");
    sun.selfRadius = 109;
    sun.selfAngle = 7.25;
    sun.selfRotatTime = 25.05;
    sun.displayList = 0;
    sun.curSelfAngle = 0;

    // initialzie earth;
    earth.texturePath = (char*)MAKE_TEXTURE_PATH("earth");
    earth.selfRadius = 1;
    earth.selfAngle = 23.4392811;
    earth.selfRotatTime = 0.99726968;
    earth.orbitAngle = 0.00005;
    earth.orbitRadius = 1;
    earth.orbitRotatTime = 365.256363004;
    earth.displayList = 0;
    earth.curSelfAngle = 0;
    earth.curOrbitAngle = 0;

    // initialize moon;
    moon.texturePath = (char*)MAKE_TEXTURE_PATH("moon");
    moon.selfRadius = 0.273;
    moon.selfAngle = 27.321661;
    moon.selfRotatTime = 6.687;
    moon.orbitAngle = 5.145;
    moon.orbitRadius = 0.00257;
    moon.orbitRotatTime = 27.321661;
    moon.displayList = 0;
    moon.curSelfAngle = 0;
    moon.curOrbitAngle = 0;

    // initialize mars
    mars.texturePath = (char*)MAKE_TEXTURE_PATH("mars");
    mars.selfRadius = 0.5320;
    mars.selfAngle = 1.025957;
    mars.selfRotatTime = 25.19;
    mars.orbitAngle = 1.850;
    mars.orbitRadius = 1.523679;
    mars.orbitRotatTime = 686.971;
    mars.displayList = 0;
    mars.curOrbitAngle = 0;
    mars.curSelfAngle = 0;

    // initialize jupiter
    jupiter.texturePath = (char*)MAKE_TEXTURE_PATH("jupiter");
    jupiter.selfRadius = 10.97 / 5;
    jupiter.selfAngle = 9.925;
    jupiter.selfRotatTime = 3.13;
    jupiter.orbitAngle = 1.303;
    jupiter.orbitRadius = 5.20260 / 2;
    jupiter.orbitRotatTime = 4332.59;
    jupiter.displayList = 0;
    jupiter.curOrbitAngle = 0;
    jupiter.curSelfAngle = 0;
}


void initTexture() {
    sun.textureName = loadTexture(sun.texturePath);
    earth.textureName = loadTexture(earth.texturePath);
    moon.textureName = loadTexture(moon.texturePath);
    mars.textureName = loadTexture(mars.texturePath);
    jupiter.textureName = loadTexture(jupiter.texturePath);
}

void initList() {
    initSunDisplay();
    initEarthDisplay();
    initMoonDisplay();
    initMarsDisplay();
    initJupiterDisplay();
}


void initSunDisplay() {

    sun.displayList = glGenLists(1);

    glNewList(sun.displayList, GL_COMPILE);

    GLUquadricObj* quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GLU_TRUE);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sun.textureName);
    gluSphere(quadric, sun.selfRadius / SUN_RADIUS_SCALE, 100, 100);

    glBindTexture(GL_TEXTURE_2D, 0);

    gluDeleteQuadric(quadric);

    glEndList();
}

void initEarthDisplay() {

    earth.displayList = glGenLists(1);

    glNewList(earth.displayList, GL_COMPILE);

    GLUquadricObj* quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GLU_TRUE);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earth.textureName);
    gluSphere(quadric, earth.selfRadius / 5, 100, 100);

    glBindTexture(GL_TEXTURE_2D, 0);

    gluDeleteQuadric(quadric);

    glEndList();
}

void initMoonDisplay() {

    moon.displayList = glGenLists(1);

    glNewList(moon.displayList, GL_COMPILE);

    GLUquadricObj* quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GLU_TRUE);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, moon.textureName);
    gluSphere(quadric, moon.selfRadius / 5, 100, 100);

    glBindTexture(GL_TEXTURE_2D, 0);

    gluDeleteQuadric(quadric);

    glEndList();
}


void initMarsDisplay() {

    mars.displayList = glGenLists(1);

    glNewList(mars.displayList, GL_COMPILE);

    GLUquadricObj* quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GLU_TRUE);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mars.textureName);
    gluSphere(quadric, mars.selfRadius / 5, 100, 100);

    glBindTexture(GL_TEXTURE_2D, 0);

    gluDeleteQuadric(quadric);

    glEndList();
}

void initJupiterDisplay() {

    jupiter.displayList = glGenLists(1);

    glNewList(jupiter.displayList, GL_COMPILE);

    GLUquadricObj* quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GLU_TRUE);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, jupiter.textureName);
    gluSphere(quadric, jupiter.selfRadius / 5, 100, 100);

    glBindTexture(GL_TEXTURE_2D, 0);

    gluDeleteQuadric(quadric);

    glEndList();
}