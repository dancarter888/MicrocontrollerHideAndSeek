/**
@file       HideAndSeek.h
@authors    Daniel Siemers () & Ann Ngo ()
@date       18 October 2019
@brief      Hide And Seek
            Header file for the primary game module.
**/

#ifndef HIDE_AND_SEEK_H
#define HIDE_AND_SEEK_H


/** Library Modules */
#include <stdlib.h>
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "../fonts/font3x5_1.h"

/** Application Modules */
//#include "pregame.h"

#define PACER_RATE 500
#define IR_RATE 2
#define MESSAGE_RATE 25
#define LOOP_RATE 300

#define MAX_ROUNDS 8
#define PAUSE_TIME 5

//allows to encode coordinates into a single char
#define ENCODE_POS(tlx, tly) (tlx << 3 | tly)

//allows to decode char back into coordinates
#define DECODE_p2tlx(encoded_pos) (encoded_pos >> 3)
#define DECODE_p2tly(encoded_pos) (encoded_pos & 0x7)

void draw_box(int tlx, int tly, int brx, int bry);

#endif
