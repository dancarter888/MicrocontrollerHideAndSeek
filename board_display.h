/**
@file       board_display.h
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Board display header file.
**/

#include "tinygl.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "../fonts/font3x5_1.h"

#ifndef BOARD_DISPLAY_H
#define BOARD_DISPLAY_H

#define PACER_RATE 500
#define MESSAGE_RATE 25
#define LOOP_RATE 300
#define PAUSE_TIME 2

void draw_box(int tlx, int tly, int brx, int bry);

void display_both_boxes(int tlx, int tly, int brx, int bry, int p2_coords[]);

int show_main_menu(int is_seeking);

#endif
