#pragma once

/*draw_solar*/
static double Earth_year = 0, Mercury_year = 0;
static double month = 0;
static double Mercury_day = 0, Earth_day = 0, Sun_day = 0;//88 360

void draw_xz_circle(double r) { //在xy平面画静态轨道
	int n = 100, i = 0;
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	while (i++ < n) {
		glVertex3d(r*cos(i * 2 * PI / n), 0, r*sin(i * 2 * PI / n));//x,y,z
	}
	glEnd();
	glPopMatrix();
}
void draw_xy_circle(double r) {
	int n = 1000, i = 0;
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	while (i++ < n) {
		glVertex3d(r*cos(i * 2 * PI / n), r*sin(i * 2 * PI / n), 0);//x,y,z
	}
	glEnd();
	glPopMatrix();
}
void draw_solar() {
	glPushMatrix(); //保存此时的状态
	glColor3f(1, 0, 0);
	glRotated(Sun_day, 0, 1, 0);//Sun的自传
	glutWireSphere(25, 20, 20);//Sun

	glPopMatrix();//太阳的自转后后续无关
	glPushMatrix();
	glRotated(Earth_year, 0, 1, 0);//绕着y轴旋转，地球的公转
	glColor3f(0, 0, 1);
	draw_xz_circle(100);//画地球的公转轨道 
	glTranslated(100, 0, 0);//移动到x=100

	glPushMatrix();//
	glRotated(Earth_day, 0, 1, 0); //地球的自转
	glColor3f(0, 0, 1);
	glutWireSphere(10, 10, 10); //画的是地球
	glPopMatrix();//地球的自转和月球无关

	glRotated(month, 0, 0, 1);//月球围绕地球转
	glColor3f(0.3, 0.3, 0.3);
	draw_xy_circle(25);//月球的轨道
	glTranslated(25, 0, 0);//移动
	glutSolidSphere(1, 20, 20);//月球，先移动(25,0,0)在绕着地球转，最后在绕着太阳转	

	glPopMatrix();
	glRotated(8, 0, 0, 1); //8度表示水星轨道偏移的一定角度
	glRotated(Mercury_year, 0, 1, 0);//公转绕着y轴旋转
	glColor3f(0, 1, 0);
	draw_xz_circle(65);//画水星的公转轨道
	glTranslated(65, 0, 0);//
	glRotated(Mercury_day, 0, 1, 0);//自转
	glColor3f(0, 1, 0);
	glutWireSphere(5, 10, 10); //画的水星



	glPopMatrix();
}

