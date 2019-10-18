/**
@file       ir_controller.h
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Header file for the IR controller.
**/

#ifndef IR_CONTROLLER_H
#define IR_CONTROLLER_H

/** Library Modules */
#include "ir_uart.h"

#define IR_RATE 2

//allows to encode coordinates into a single char
#define ENCODE_POS(tlx, tly) (tlx << 3 | tly)
//allows to decode char back into coordinates
#define DECODE_p2tlx(encoded_pos) (encoded_pos >> 3)
#define DECODE_p2tly(encoded_pos) (encoded_pos & 0x7)

void ir_send_pos(int coords[]);

void ir_recv_pos(int p2_coords[]);

#endif
