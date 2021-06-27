#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include"BaseElement.h"
#include"MyCamera.h"
#include"GameState.h"
#include"Solar.h"
#include"texture.h"
extern Texture My_texture;
void Draw_Grid() {
	int i, j;
	if (draw_all > 0) {
		for (i = 0; i < grid_num; i++) {
			for (j = 0; j < grid_num; j++) {
				if (i == 3) {
					int p = 0;
				}
					
				glColor3f(0, 0, 1);
				if (show_texture) mygrid[i][j].draw_wall(My_texture.wall_texture);
				else mygrid[i][j].draw_wall(); 
				if(ctrl_grid[i][j]) mygrid[i][j].draw_grid();

				if (ctrl_grid[i][j] == COIN) {
					glPushMatrix();
					glTranslatef(mygrid[i][j].position[0], 2 * grid_size / 2, mygrid[i][j].position[2]);

					glRotated(Sun_day, 0, 1, 0);
					the_coin.draw_coin();
					glPopMatrix();
				}
				if (ctrl_grid[i][j] == ISROD_) {
					glPushMatrix();
					glTranslatef(mygrid[i][j].position[0], 2 * grid_size / 2, mygrid[i][j].position[2]);
					the_rod.draw_ROD();
					glPopMatrix();
				}
				if (ctrl_grid[i][j] == ISROD_1) {
					glPushMatrix();
					glTranslatef(mygrid[i][j].position[0], 2 * grid_size / 2, mygrid[i][j].position[2]);
					glRotatef(90, 0, 1, 0);
					the_rod.draw_ROD();
					glPopMatrix();
				}
				if (ctrl_grid[i][j] == fire_) {
					glPushMatrix();
					glEnable(GL_BLEND);
					glDepthMask(GL_FALSE);
					glTranslatef(mygrid[i][j].position[0], 0, mygrid[i][j].position[2]);
					the_particle.draw_particle();
					glDepthMask(GL_TRUE);
					glDisable(GL_BLEND);
					glPopMatrix();
				}
				if (ctrl_grid[i][j] == fire_1) {
					glPushMatrix();
					glEnable(GL_BLEND);
					glDepthMask(GL_FALSE);
					glTranslatef(mygrid[i][j].position[0], 0, mygrid[i][j].position[2]);
					glRotatef(90, 0, 1, 0);
					//the_particle.draw_particle();
					glDepthMask(GL_TRUE);
					glDisable(GL_BLEND);
					glPopMatrix();
				}
			}
		}
	}
	else if(draw_all<0){//-3 
		for (i = draw_all; i <= -draw_all; i++) {
			for (j = draw_all; j <= -draw_all; j++) {
				glColor3f(0, 0, 1);
				int ii = Correct(Camera_x / grid_size + i);
				int jj = Correct(-Camera_z / grid_size + j);
				mygrid[ii][jj].draw_grid();

				if (show_texture) mygrid[ii][jj].draw_wall(My_texture.wall_texture);
				else mygrid[ii][jj].draw_wall();
				if (ctrl_grid[ii][jj] == COIN) {
					glPushMatrix();
					glTranslatef(mygrid[ii][jj].position[0], 2*grid_size / 2, mygrid[ii][jj].position[2]);

					glRotated(Sun_day, 0, 1, 0);
					the_coin.draw_coin();
					glPopMatrix();
				}
				if (ctrl_grid[ii][jj] == ISROD_) {
					glPushMatrix();
					glTranslatef(mygrid[ii][jj].position[0], 2 * grid_size / 2, mygrid[ii][jj].position[2]);
					the_rod.draw_ROD();
					glPopMatrix();
				}
				if (ctrl_grid[ii][jj] == ISROD_1) {
					glPushMatrix();
					glTranslatef(mygrid[ii][jj].position[0], 2 * grid_size / 2, mygrid[ii][jj].position[2]);
					glRotatef(90, 0, 1, 0);
					the_rod.draw_ROD();
					glPopMatrix();
				}
				if (ctrl_grid[ii][jj] == fire_) {
					glPushMatrix();
					glEnable(GL_BLEND);
					glDepthMask(GL_FALSE);
					glTranslatef(mygrid[ii][jj].position[0],0, mygrid[ii][jj].position[2]);
					//the_particle.draw_particle();
					glDepthMask(GL_TRUE);
					glDisable(GL_BLEND);
					glPopMatrix();
				}
				if (ctrl_grid[ii][jj] == fire_1) {
					glPushMatrix();
					glEnable(GL_BLEND);
					glDepthMask(GL_FALSE);
					glTranslatef(mygrid[ii][jj].position[0], 0, mygrid[ii][jj].position[2]);
					glRotatef(90, 0, 1, 0);
					//the_particle.draw_particle();
					glDepthMask(GL_TRUE);
					glDisable(GL_BLEND);
					glPopMatrix();
				}
			}
		}

	}

} 