#pragma once
#include<stdio.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "GameState.h"
/**************some global************/
GLfloat Gravity = 1;
GLfloat max_people_speed  = 40;//set the people max speed
GLfloat max_Monster_speed = 40;//set the monster max speed
GLfloat init_speed = 15;
GLfloat init_a = 1;
GLfloat init_monster_s = -5000;

class speed {
public:
	GLfloat V_run;// the speed to run
	GLfloat V_up;//向上跳跃的速度 其实就是Camera
	//GLfloat position_y;// Camera_y 
	GLfloat S;//路程
	GLfloat a;
	GLfloat g;
	GLfloat positiony;
	void init() {
		S = 0;
		V_run = 0;
		V_up = 0;
		positiony=0;
		g = Gravity;
		a = init_a;
	}
};
class people_speed:public speed {
public:
	people_speed() {
		S = 0;
	}
	void set(GLfloat test_a) {
		a = test_a;
	}
	void calculate_run_speed(GLfloat dt) {
		if (run_state == 0) {
			V_run = 0;
			return;
		}
		V_run += a * dt;
		if (V_run > max_people_speed) V_run = max_people_speed;
		//if (V_run < 0) V_run = 0;
		S += V_run * dt + a * dt*dt / 2;
	}
	void caculate_jump(GLfloat dt) {//此时的状态
		if (Game_start_state == 0) {
			if (down_down_state == 1) {
				positiony += V_up * dt + g * dt*dt / 2;
			}
			return;
		}
		
 		if (ground_type == BLANK) {
			g = -Gravity;
			if(jump_state!=1) down_down_state = 1;
		}
		if (down_down_state != 1&&jump_state == 0&&down_state==0) {
			if (ground_type != BLANK) {
				positiony = 0;
				V_up = 0;
				return;
			}
		}
		if (jump_state) g = -Gravity;
		if (down_state&&down_down_state != 1) g = Gravity;
		if (down_state&&ground_type == BLANK) {
			g = -Gravity, down_down_state = 1;
			Game_start_state == 0;
			if (V_up > 0) {
				V_up = 0;
			}
		}

		V_up += g * dt;//else
		positiony += V_up * dt + g * dt*dt / 2;
		if (down_down_state) {
			if (positiony <= -grid_size*10) {
				Game_over_state = 1;
				draw_all = 0;//画所有
			}
			
			return ;
		}
		else {
			if (jump_state&&positiony < 0) {
				jump_state = 0;//this time is not a jump state
			}
			else if (down_state&&positiony > 0) {
				down_state = 0;
			}
		}

	}
};

class monster_speed:public speed {
public:
	void set_a(GLfloat test_a) {
		a = test_a;
	}
	void calculate_run_speed(GLfloat dt) {
		
		V_run += a * dt;
		if (V_run > max_Monster_speed) V_run = max_Monster_speed;
		S += V_run * dt + a * dt*dt / 2;
	}
};

people_speed the_people_speed;
monster_speed the_monster_speed;
void chech_game_over() {
	if (the_people_speed.S <= the_monster_speed.S) {
		Game_over_state = 1;
	}
}