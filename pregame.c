/** @file   pregame.c
    @author DANIEL SIEMERS ( ), ANN NGO (39979230)
    @date   18 Oct 2019
    @brief  Program intro using display module.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "../fonts/font3x5_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

/* Define text update rate (characters per 25 s).  */
#define MESSAGE_RATE 25


int main (void)
{
    system_init ();
    tinygl_init (LOOP_RATE);

    navswitch_init ();

    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);

    tinygl_text ("HIDE AND SEEK : PUSH TO START ");

    pacer_init (LOOP_RATE);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        tinygl_update ();

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_WEST))
            tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

        if (navswitch_push_event_p (NAVSWITCH_EAST))
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);

    }

    return 0;
}
