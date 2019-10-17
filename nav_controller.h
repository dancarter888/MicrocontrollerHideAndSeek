/**
@file       nav_controller.h
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Header file for the navigation controller.
**/

#ifndef NAV_CONTROLLER_H
#define NAV_CONTROLLER_H

#include "pacer.h"
#include "tinygl.h"
#include "navswitch.h"

#include "ir_controller.h"
#include "board_display.h"

void move_player(int tlx, int tly, int brx, int bry, int coords[]);

#endif
