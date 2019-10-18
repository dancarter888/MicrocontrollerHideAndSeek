/**
@file       nav_controller.c
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Navigation controller module.
**/

#include "nav_controller.h"

//Checks for navswitch input and moves player based on it
//If navswitch is pushed, player can no longer move and waits for opponent to push navswitch
//Check if the box is at the edge of the LEDMAT and
//Does not let it go past the border
void move_player(int tlx, int tly, int brx, int bry, int coords[])
{
    while (1)
    {
        pacer_wait();
        tinygl_update ();
        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_NORTH) && tly != 0){
            tly--;
            bry--;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && bry != 6){
            tly++;
            bry++;
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST) && brx != 4){
            tlx++;
            brx++;
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST) && brx != 2){
            tlx--;
            brx--;
        }

        //Saves the position that the box was in when played pushed navswitch
        if (navswitch_push_event_p (NAVSWITCH_PUSH)){
            coords[0] = tlx;
            coords[1] = tly;
            coords[2] = brx;
            coords[3] = bry;
            ir_send_pos(coords);
            break;
        }

        tinygl_clear();

        draw_box(tlx, tly, brx, bry);
    }
}
