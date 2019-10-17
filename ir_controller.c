/**
@file       ir_controller.c
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            IR controller module.
**/

#include "ir_controller.h"

//encodes the coordinates of the box into one character (since you can only send one character over IR)
//sends the encoded coordinates
void ir_send_pos(int coords[])
{
    char msg = ENCODE_POS(coords[0], coords[1]);
    ir_uart_putc(msg);
}

//decodes the coordinates of the player's box that was received
void ir_recv_pos(int p2_coords[])
{
    char msg = ir_uart_getc();
    int p2tlx = DECODE_p2tlx(msg);
    int p2tly = DECODE_p2tly(msg);
    p2_coords[0] = p2tlx;
    p2_coords[1] = p2tly;
}
