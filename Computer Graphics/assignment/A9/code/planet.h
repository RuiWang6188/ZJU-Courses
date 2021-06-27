#pragma once
#include<gl/glut.h>

class planet {

public:
	char* texturePath;

	GLuint textureName;

	GLuint displayList;

	// self parameter
	GLdouble selfRadius;
	// self angle
	GLdouble selfAngle;
	// self rotate time
	GLdouble selfRotatTime;


	// orbit parameter
	GLdouble orbitRadius;
	GLdouble orbitAngle;
	GLdouble orbitRotatTime;

	// running parameter
	GLfloat curSelfAngle;
	GLfloat curOrbitAngle;
};