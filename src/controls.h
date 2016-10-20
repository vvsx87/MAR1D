#ifndef _CONTROLS_H
#define _CONTROLS_H
#include <GLFW/glfw3.h> //for keypress keycodes (e.g. GLFW_KEY_W)
#include "helpers.h"
#include "objects.h"
#include "gamelogic.h"
#include "mechanics.h"
#include "audio.h"

#define k_xVel 0.05
#define k_yVel (3.9+abs(s.scene[s.pli].vx)*s.bigMario/5)
#define k_xVelMax  1.0
#define k_yVelMin -4.5
#define k_nJumpFrames 40

extern struct state s;

void cl_init();
void cl_update();
void cl_keypress(int, int, int, int);
void cl_click(int,int,int);
void cl_cursormove(double, double);
bool cl_move1(int, char, bool);
bool cl_move(int, char, int);
void cl_gravity(int);
void cl_jumpStart();
void cl_smallJump();
void cl_jumpEnd();
void cl_bigMario();
void cl_fireMario();
void cl_starman();
void cl_smallMario();
void cl_fire();
void cl_crouch();
void cl_uncrouch();
void cl_pipe();
void cl_delObjAt(int);
void cl_delObj(obj*);

#endif