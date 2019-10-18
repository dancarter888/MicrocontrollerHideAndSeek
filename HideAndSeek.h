/**
@file       HideAndSeek.h
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Header file for the primary game module.
**/

#ifndef HIDE_AND_SEEK_H
#define HIDE_AND_SEEK_H

/** Library Modules */
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "led.h"
#include "../fonts/font3x5_1.h"

/** Application Modules */
#include "nav_controller.h"
#include "ir_controller.h"
#include "score_calc.h"
#include "board_display.h"

#define MAX_ROUNDS 8

//HideAndSeek
void show_score(void);

void add_to_score(int overlap, int is_seeking);

void scoring(int tlx, int tly, int p2_coords[], int is_seeking);

void wait_for_opponent(int tlx, int tly, int brx, int bry, int p2_coords[]);

void take_turn (int is_seeking);

void display_win_lose(void);

void reset_game(void);

int choose_replay(int is_seeking);

void start_game(void);

#endif
