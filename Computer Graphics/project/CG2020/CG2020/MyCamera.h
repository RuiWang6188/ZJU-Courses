#pragma once

GLfloat Camera_x, Camera_y, Camera_z;//
GLfloat Gaze_x, Gaze_y, Gaze_z;
GLfloat Camera_radius=grid_size/2;

const double PI = 2 * acos(0);
static GLfloat move_angle = -90.0;   
//如果初始角度设置为0则初始面向X轴正方向，设置为-90面向 
//Z轴负方向，符合默认情况下的设计习惯。 

double cameraDistanceX = 0;
double cameraDistanceY = 0;
double cameraAngleX = 0;
double cameraAngleY = 0;
double move_y_angle = -atan(40 / 150);//-90;
double times = 1; //缩放倍数


//鼠标点击事件
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;