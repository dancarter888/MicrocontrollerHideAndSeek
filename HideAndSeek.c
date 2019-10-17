/**
@file       HideAndSeek.c
@authors    Daniel Siemers () & Ann Ngo ()
@date       18 October 2019
@brief      Hide And Seek
            Primary game module.
**/

#include "HideAndSeek.h"


//keeps track of ticks
static int tick = 0;
static int turn_count = 0;
static int score = 0;

//draws the boxes using the top left coordinate (tlx, tly) and the bottom right coordinate (brx, bry)
//also sets the middle dot inside the box
void draw_box(int tlx, int tly, int brx, int bry)
{
    tinygl_draw_box(tinygl_point(tlx, tly), tinygl_point(brx, bry), 1);
    tinygl_pixel_set(tinygl_point(tlx + 1, tly + 1), 1);
}

//checks for navswitch input and moves based on it
//if navswitch is pushed, player can no longer move and waits for opponent to push navswitch
void move_player(int tlx, int tly, int brx, int bry, int coords[]){
    while (1)
    {
        pacer_wait();
        tinygl_update ();
        navswitch_update ();

        //all the '&&'s check if the box is at the edge of the LEDMAT and
        //does not let it go past the border
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

        //saves the position that the box was in when played pushed navswitch
        if (navswitch_push_event_p (NAVSWITCH_PUSH)){
            coords[0] = tlx;
            coords[1] = tly;
            coords[2] = brx;
            coords[3] = bry;
            //no longer moving the player
            break;
        }

        tinygl_clear();
        draw_box(tlx, tly, brx, bry);
    }
}

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

static int check_overlap(int tlx, int tly, int p2tlx, int p2tly) {
    int overlap = (3 - abs(tlx - p2tlx)) * (3 - abs(tly - p2tly));

    if (overlap < 0) {
        overlap = 0;
    }
    return overlap;
}

static void add_to_score(int overlap) {
    score += overlap;
}

//allows player to take a turn (hide/seek)
//turns are identical for both players except if you are a seeker, your score can increase
//if you are a hider, your score will remain the same until you become a seeker
static void take_turn (int is_seeking) {
    //This is the default position that your box shows up at
    int tlx = 1;
    int tly = 4;
    int brx = 3;
    int bry = 6;
    int overlap = 0;

    //creates an array of the players coordinates
    int coords[] = {tlx, tly, brx, bry};

    //creates an array of opponents coordinates (only stores p2_tlx, p2_tly)
    //p2_tlx = player2 top left x coordinate
    //p2_tly = player2 top left y coordinate
    int p2_coords[2];

    //initializes everything

    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);

    //lets the player move
    move_player(tlx, tly, brx, bry, coords);
    //puts the coordinates of the player when they pushed the navswitch, into the coords array
    tlx = coords[0];
    tly = coords[1];
    brx = coords[2];
    bry = coords[3];

    while (1) {
        pacer_wait();
        tinygl_update ();

        //slows down the rate a t which the IR sends info as otherwise the LEDMAT messes up
        tick += 1;
        if (tick > PACER_RATE/IR_RATE) {
            //sends the players coordinates to the opponent
            ir_send_pos(coords);
            tick = 0;
        }

        //check if the oppoenent has sent their coords to the player
        //will store those coordinates in the p2_coords array
        if (ir_uart_read_ready_p()) {
            ir_recv_pos(p2_coords);
            break;
        }

        /**
        //testing by setting the p2 coords manually
        p2_coords[0] = 1;
        p2_coords[1] = 4;
        break;
        */

        draw_box(tlx, tly, brx, bry);
    }

    //clears the board
    tinygl_clear();

    tick = 0;

    while (1) {
        pacer_wait();
        tinygl_update ();

        //makes sure both boxes are displayed for 5 seconds
        tick += 1;
        if (tick > PAUSE_TIME * PACER_RATE) {
            tick = 0;
            break;
        }

        //displays both boxes
        draw_box(p2_coords[0], p2_coords[1], p2_coords[0] + 2, p2_coords[1] + 2);
        draw_box(tlx, tly, brx, bry);
    }

    if (is_seeking) {
        //make an overlap func to check how many dots overlap
        overlap = check_overlap(tlx, tly, p2_coords[0], p2_coords[1]);

        //make a score function to add to the score
        add_to_score(overlap);
    }
}

static int show_main_menu(int is_seeking)
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

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            return 1;
        } else if (ir_uart_read_ready_p()) {
            is_seeking = ir_uart_getc();
            return !is_seeking;
        }
    }
}

int main (void)
{
    system_init ();
    tinygl_init (PACER_RATE * 2);
    navswitch_init ();
    ir_uart_init();
    pacer_init (PACER_RATE);

    int is_seeking = 0;
    //your main menu will start (havent combined them yet)
    /**
    is_seeking = show_main_menu(is_seeking);
    tinygl_clear();

    while (is_seeking == 1) {
        pacer_wait();

        tick += 1;
        if (tick > 20) {
            tick = 0;
            break;
        }

        ir_uart_putc(is_seeking);
    }
    */


    // need to choose who starts
    //We can do this by seeing who pushes start first from the main menu screen you made


    //while the less than 8 turns have been played (3 turns each)
    while (turn_count < MAX_ROUNDS) {
        take_turn(is_seeking);
        //swap from seeking to hiding (or from hiding to seeking) for the next turn
        is_seeking = !is_seeking;
        turn_count += 1;
    }

    //show the end score using tinygl
    //show player score on player funkit and opponents score on opponent funkit

    return 0;
}
