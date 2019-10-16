void display_win_lose(int win)
{
/*display either win or lose message based on 'win' parameter
 * which is set to 1 for a win and 0 for a loss */
    int tick = 0;
    int flashes = 0;
    if (win == 1) {
        while (flashes < NUM_FLASHES_WIN) {
            flashes++;
            tick = 0;
            while (tick < FLASH_TOTAL_DURATION) {
                if (tick < FLASH_ON_DURATION) {
                    //flash screen by drawing boxes over all pixels
                    tinygl_draw_box (tinygl_point (0, 0), tinygl_point (4, 6), 1);
                    tinygl_draw_box (tinygl_point (1, 1), tinygl_point (3, 5), 1);
                    tinygl_draw_box (tinygl_point (2, 4), tinygl_point (2, 2), 1);
                } else {
                    tinygl_clear ();
                }
                pacer_wait ();
                tick++;
                tinygl_update ();
            }
        }
        tinygl_clear ();
        tinygl_update ();
        
        //display "YOU WIN" message
        tinygl_font_set (&font3x5_1);
        tinygl_text_speed_set (WIN_LOSE_TEXT_SPEED);
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
        tinygl_text (" WIN");
        while (1) {
            pacer_wait ();
            tick++;
            tinygl_update ();
        }

    } else {
        while (flashes < NUM_FLASHES) {
            flashes++;
            tick = 0;
            while (tick < FLASH_TOTAL_DURATION) {
                if (tick < FLASH_ON_DURATION) {
                    //flash screen by drawing boxes over all pixels
                    tinygl_draw_box (tinygl_point (0, 0), tinygl_point (4, 6), 1);
                    tinygl_draw_box (tinygl_point (1, 1), tinygl_point (3, 5), 1);
                    tinygl_draw_box (tinygl_point (2, 4), tinygl_point (2, 2), 1);
                } else {
                    tinygl_clear ();
                }
                pacer_wait ();
                tick++;
                tinygl_update ();
            }
        }
        tinygl_clear ();
        tinygl_update ();

        //display "YOU LOSE" message
        tinygl_font_set (&font3x5_1);
        tinygl_text_speed_set (WIN_LOSE_TEXT_SPEED);
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
        tinygl_text (" LOSE");
        while (1) {
            pacer_wait ();
            tick++;
            tinygl_update ();
        }

    }
}