#pragma once
void render();
static void init();
static void display();
static void reshape(int w, int h);
static void drag(int x, int y);
static void click(int button, int state, int x, int y);
static void wheel(int wheel, int dir, int x, int y);
static void keyboarddown(unsigned char key, int x, int y);
static void keyboardup(unsigned char key, int x, int y);
static void specialdown(int key, int x, int y);
static void specialup(int key, int x, int y);
void Timerfuc(int x);
static void reshape1(int w, int h);
void renderScenesw1();