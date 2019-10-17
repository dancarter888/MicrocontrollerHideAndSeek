/**
@file       board_display.c
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Board display module.
**/

#include "board_display.h"

//draws the boxes using the top left coordinate (tlx, tly) and the bottom right coordinate (brx, bry)
//also sets the middle dot inside the box
void draw_box(int tlx, int tly, int brx, int bry)
{
    tinygl_draw_box(tinygl_point(tlx, tly), tinygl_point(brx, bry), 1);
    tinygl_pixel_set(tinygl_point(tlx + 1, tly + 1), 1);
}

void display_both_boxes(int tlx, int tly, int brx, int bry, int p2_coords[])
{
    int tick = 0;

    while (1) {
        pacer_wait();
        tinygl_update ();

        //makes sure both boxes are displayed for 2 seconds
        tick += 1;
        if (tick > PAUSE_TIME * PACER_RATE) {
            tick = 0;
            break;
        }

        //displays both boxes
        draw_box(p2_coords[0], p2_coords[1], p2_coords[0] + 2, p2_coords[1] + 2);
        draw_box(tlx, tly, brx, bry);
    }
}

int show_main_menu(int is_seeking)
{
    system_init ();
    tinygl_init (LOOP_RATE);

    navswitch_init ();

    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);

    tinygl_text ("HIDE AND SEEK : PUSH TO START ");

    while (1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            ir_uart_putc(is_seeking);
            return is_seeking;
        } else if (ir_uart_read_ready_p()) {
            is_seeking = ir_uart_getc();
            return !is_seeking;
        }
    }
}
