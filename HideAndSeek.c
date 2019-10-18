/**
@file       HideAndSeek.c
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Primary game module.
**/

#include "HideAndSeek.h"

//Static variables to keep track of game variables
static int tick = 0;
static int turn_count = 0;
static int score = 0;
static int p2_score = 0;
static char score_buffer[3];
static int replay = 1;

//Displays the score of the player on the funkit
void show_score(void)
{
    tinygl_init (PACER_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

    //Changes score from int to char
    itoa(score, score_buffer, 10);
    tinygl_text(score_buffer);

    //Displays the score on the screen for the duration of PAUSE_TIME
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

//Adds round score to players overall score if they are seeking
//If player is hiding, adds score to the opponents score
void add_to_score(int overlap, int is_seeking)
{
    if (is_seeking) {
        score += overlap;
    } else if (!is_seeking) {
        p2_score += overlap;
    }
}

//Calculates the score based on the overlap of the hider and seeker boxes and shows it
void scoring(int tlx, int tly, int p2_coords[], int is_seeking)
{
    int overlap = 0;

    //Overlap func to check how many dots overlap
    overlap = check_overlap(tlx, tly, p2_coords[0], p2_coords[1]);

    add_to_score(overlap, is_seeking);

    show_score();
}

//Waits for opponent to take their turn
//The player cannot move during this waiting time
void wait_for_opponent(int tlx, int tly, int brx, int bry, int p2_coords[])
{
    while (p2_coords[0] == -1) {
        pacer_wait();
        tinygl_update ();

        //Checks if player has taken their turn yet
        if (ir_uart_read_ready_p()) {
            ir_recv_pos(p2_coords);
        }

        //Displays stationery player box
        draw_box(tlx, tly, brx, bry);
    }
}

//Allows player to take a turn (hide/seek)
//Turns are identical for both players except if you are a seeker, your score can increase
//If you are a hider, your score will remain the same until you become a seeker
void take_turn (int is_seeking)
{
    led_set (LED1, 0);

    //This is the default position that your box shows up at
    int tlx = 1;
    int tly = 4;
    int brx = 3;
    int bry = 6;

    //Creates an array of the players coordinates
    int coords[] = {tlx, tly, brx, bry};

    //Creates an array of opponents coordinates (only stores p2_tlx, p2_tly)
    //p2_tlx = player2 top left x coordinate
    //p2_tly = player2 top left y coordinate
    int p2_coords[2] = {-1};

    //Initializes everything
    tinygl_clear();

    //Seeker waits for hider to hide
    if (is_seeking) {
        wait_for_opponent(tlx, tly, brx, bry, p2_coords);
    }

    //Turns on LED when it is players turn
    led_set (LED1, 1);
    //Lets the player move
    move_player(tlx, tly, brx, bry, coords);
    //Coords of the player were stored in coords array
    //Creates local variables of these coords
    tlx = coords[0];
    tly = coords[1];
    brx = coords[2];
    bry = coords[3];

    //Turns off LED when player turn is over
    led_set (LED1, 0);

    //Hider waits for seeker
    if (!is_seeking) {
        wait_for_opponent(tlx, tly, brx, bry, p2_coords);
    }

    tinygl_clear();

    display_both_boxes(tlx, tly, brx, bry, p2_coords);

    scoring(tlx, tly, p2_coords, is_seeking);
}

//Displays if the player won, lost or tied
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

    //Displays for the duration of PAUSE_TIME
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

//Resets all the game variables for if the game is reset
void reset_game(void)
{
    tick = 0;
    turn_count = 0;
    score = 0;
    p2_score = 0;
}

//Allows the most recent seeker to choose whether they would like to replay
int choose_replay(int is_seeking)
{
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);

    //Most recent seeker is prompted on how to replay or exit
    if (is_seeking) {
        tinygl_text ("UP: REPLAY, DOWN: EXIT");
        while (1) {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();

            //West replays
            if (navswitch_push_event_p (NAVSWITCH_WEST)) {
                ir_uart_putc(1);
                return 1;
            //East exits
            } else if (navswitch_push_event_p (NAVSWITCH_EAST)) {
                ir_uart_putc(0);
                return 0;
            }
        }
    //Most recent hider is told to wait while opponent chooses
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

//Starts the game
void start_game(void)
{
    int is_seeking = 0;

    is_seeking = show_main_menu(is_seeking);
    tinygl_clear();

    while (turn_count < MAX_ROUNDS) {
        take_turn(is_seeking);
        //Swap from seeking to hiding (or from hiding to seeking) for the next turn
        is_seeking = !is_seeking;
        turn_count += 1;
    }

    //Show the end score using tinygl
    //Show player score on player funkit and opponents score on opponent funkit
    display_win_lose();
    replay = choose_replay(is_seeking);
    if (replay) {
        reset_game();
    }
}

//Main inits
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
