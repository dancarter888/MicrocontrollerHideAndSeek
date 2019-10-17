/**
@file       HideAndSeek.c
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Primary game module.
**/

#include "HideAndSeek.h"

//keeps track of ticks
static int tick = 0;
static int turn_count = 0;
static int score = 0;
static int p2_score = 0;
static char score_buffer[3];
static int replay = 1;

void show_score(void)
{
    tinygl_init (PACER_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);


    itoa(score, score_buffer, 10);
    tinygl_text(score_buffer);

    tick = 0;

    while (1) {
        pacer_wait();
        tinygl_update ();

        tick += 1;
        if (tick > PAUSE_TIME * PACER_RATE) {
            tick = 0;
            break;
        }
    }
}

void add_to_score(int overlap, int is_seeking)
{
    if (is_seeking) {
        score += overlap;
    } else if (!is_seeking) {
        p2_score += overlap;
    }
}

void scoring(int tlx, int tly, int p2_coords[], int is_seeking)
{
    int overlap = 0;

    //make an overlap func to check how many dots overlap
    overlap = check_overlap(tlx, tly, p2_coords[0], p2_coords[1]);

    if (is_seeking) {
        //make a score function to add to the score
        add_to_score(overlap, is_seeking);
    } else if (!is_seeking) {
        add_to_score(overlap, is_seeking);
    }

    show_score();
}


void wait_for_opponent(int tlx, int tly, int brx, int bry, int p2_coords[])
{
    while (p2_coords[0] == -1) {
        pacer_wait();
        tinygl_update ();

        if (ir_uart_read_ready_p()) {
            ir_recv_pos(p2_coords);
        }

        draw_box(tlx, tly, brx, bry);
    }
}


//allows player to take a turn (hide/seek)
//turns are identical for both players except if you are a seeker, your score can increase
//if you are a hider, your score will remain the same until you become a seeker
void take_turn (int is_seeking)
{
    led_set (LED1, 0);

    //This is the default position that your box shows up at
    int tlx = 1;
    int tly = 4;
    int brx = 3;
    int bry = 6;

    //creates an array of the players coordinates
    int coords[] = {tlx, tly, brx, bry};

    //creates an array of opponents coordinates (only stores p2_tlx, p2_tly)
    //p2_tlx = player2 top left x coordinate
    //p2_tly = player2 top left y coordinate
    int p2_coords[2] = {-1};

    //initializes everything
    tinygl_clear();

    //seeker waits for hider to hide
    if (is_seeking) {
        wait_for_opponent(tlx, tly, brx, bry, p2_coords);
    }

    //lets the player move
    led_set (LED1, 1);
    move_player(tlx, tly, brx, bry, coords);
    //puts the coordinates of the player when they pushed the navswitch, into the coords array
    tlx = coords[0];
    tly = coords[1];
    brx = coords[2];
    bry = coords[3];

    //hider waits for seeker
    led_set (LED1, 0);
    if (!is_seeking) {
        wait_for_opponent(tlx, tly, brx, bry, p2_coords);
    }

    //clears the board
    tinygl_clear();

    display_both_boxes(tlx, tly, brx, bry, p2_coords);

    scoring(tlx, tly, p2_coords, is_seeking);
}

void display_win_lose(void)
{
    tinygl_init (PACER_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

    if (score > p2_score) {
        tinygl_text ("W");
    } else if (score < p2_score) {
        tinygl_text ("L");
    } else {
        tinygl_text ("T");
    }

    while (1) {
        pacer_wait();
        tinygl_update ();

        tick += 1;
        if (tick > PAUSE_TIME*2 * PACER_RATE) {
            tick = 0;
            break;
        }
    }
}

void reset_game(void)
{
    tick = 0;
    turn_count = 0;
    score = 0;
    p2_score = 0;
}

int choose_replay(int is_seeking)
{
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);

    if (is_seeking) {
        tinygl_text ("UP: REPLAY, DOWN: EXIT");
        while (1) {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();

            if (navswitch_push_event_p (NAVSWITCH_WEST)) {
                ir_uart_putc(1);
                return 1;
            } else if (navswitch_push_event_p (NAVSWITCH_EAST)) {
                ir_uart_putc(0);
                return 0;
            }
        }

    } else {
        tinygl_text ("OPPONENT CHOOSING");
        while (1) {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();

            if (ir_uart_read_ready_p ()) {
                replay = ir_uart_getc();
                return replay;
            }
        }
    }
}

void start_game(void)
{
    int is_seeking = 0;

    is_seeking = show_main_menu(is_seeking);
    tinygl_clear();

    //while the less than 8 turns have been played (4 turns each)
    while (turn_count < MAX_ROUNDS) {
        take_turn(is_seeking);
        //swap from seeking to hiding (or from hiding to seeking) for the next turn
        is_seeking = !is_seeking;
        turn_count += 1;
    }

    //show the end score using tinygl
    //show player score on player funkit and opponents score on opponent funkit
    display_win_lose();
    replay = choose_replay(is_seeking);
    if (replay) {
        reset_game();
    }
}

int main (void)
{
    system_init();
    tinygl_init(PACER_RATE * 2);
    navswitch_init();
    ir_uart_init();
    pacer_init(PACER_RATE);
    led_init ();
    led_set (LED1, 0);

    while (replay == 1) {
        start_game();
    }

    tinygl_clear();
    tinygl_update ();
    return 0;
}
