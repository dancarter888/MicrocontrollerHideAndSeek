#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 10
#define IR_RATE 2

//allows to encode coordinates into a single char
#define ENCODE_POS(tlx, tly) (tlx << 3 | tly)

//allows to decode char back into coordinates
#define DECODE_p2tlx(encoded_pos) (encoded_pos >> 3)
#define DECODE_p2tly(encoded_pos) (encoded_pos & 0x7)

//keeps track of ticks
static int tick = 0;
static int turn_count = 0;
static int score = 0;


//Display related routines to be run before game loop
static void display_task_init(void)
{
    initialise_display();
    stage_choose(START);
}


//Navswitch related routines to be run before game loop
static void navswitch_task_init(void)
{
    navswitch_init();
}


//Game logic tasks to be run before game loop
static void game_task_init(void)
{
    board_init();
    game_stage = START;
    tick = 0;
    stage_tick = 0;
}

//Button related routines to be run before game loop
static void button_task_init(void)
{
    button_init();
}


//LED related routines to be run before game loop
static void led_task_init(void)
{
    led_init();
    spwm_period_set(&led_flicker, LED_PERIOD);
    spwm_duty_set(&led_flicker, LED_DUTY);
    spwm_reset(&led_flicker);
}


//IR related routines to be run before game loop
static void ir_task_init(void)
{
    ir_uart_init();
}

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

//allows player to take a turn (hide/seek)
//turns are identical for both players except if you are a seeker, your score can increase
//if you are a hider, your score will remain the same until you become a seeker
void take_turn (int is_seeking) {
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
	int p2_coords[2];

	//initializes everything
    system_init ();
    tinygl_init (PACER_RATE * 2);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    navswitch_init ();
    ir_uart_init();

    pacer_init (PACER_RATE);
    
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
		
		//testing by setting the p2 coords manually
		p2_coords[0] = 1;
		p2_coords[1] = 4;
		break;
        
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
        if (tick > 5*PACER_RATE) {
			tick = 0;
			break;
		}
        
        //displays both boxes
        draw_box(p2_coords[0], p2_coords[1], p2_coords[0] + 2, p2_coords[1] + 2);
		draw_box(tlx, tly, brx, bry);
	}
	
	if (is_seeking) {
		//make an overlap func to check how many dots overlap
		//make a score function to add to the score	
	}
}

static void ir_task(void)
{
    states status;
    uint8_t position;

    switch (game_stage) {

        case READY :
            //Await assignment to player 2 from other player pressing button
            if (ir_get_status() == PLAYER_TWO_POS) {
                stage_choose(PAUSE);
            }
            break;

        case HIDEANDSEEK :
            // Await result of overlap
            status = ir_get_status();

            switch (status) {
                case OVERLAP_POS :
                    add_overlap();
                    last_result = OVERLAP;
                    stage_choose(RESULT_DISPLAY);
                    break;

                case MISS_POS :
                    last_result = MISS;
                    stage_choose(RESULT_DISPLAY);
                    break;

                default :
                    break;
            }
            break;

        case PAUSE :
            //Await overlap position, saved calculate result once received
            position = ir_get_position();
            if (position != NO_POSITION) {
                tinygl_point_t shot = ir_decode_overlap(position);
                if (is_overlap(shot)) {
                    ir_send_status(OVERLAP_POS);
                } else {
                    ir_send_status(MISS_POS);
                }
                stage_choose(MOVE);
            }
            break;
            
        case MOVE :
            // Await decision for next game
            status = ir_get_status();
            if (status == LOSER) {
                stage_choose(MESSAGE);
            } else if (status == KEEP_PLAYING) {
                stage_choose(NEXT_TURN);
            }
            break;


        case RESTART :
            //Await new game signal from other player
            if(ir_get_status() == RESTART_STATUS){
                restart_game();
            }
            break;

        default :
            break;
    }
}


void stage_choose(phase_t new_stage)
{
    switch (new_stage) {

        case START :
            tinygl_clear();
            break;
            
        case READY :
            tinygl_clear();
            tinygl_text("  READY?");
            break;

        case RESULT_DISPLAY :
            tinygl_clear();
            break;

        case SCORE :
            tinygl_clear();
            display_result(last_result); //in display_handler
            break;

        case PAUSE :
            tinygl_clear();
            break;

        case MESSAGE :
            tinygl_clear();
            if (game_stage == SCORE) {
                tinygl_text("  WIN!  ");
            } else {
                tinygl_text("  LOSE! ");
            }
            break;

        case RESTART :
            tinygl_clear();
            tinygl_text("  PUSH TO RESTART!");
            break;

        default :
            break;
    }

    stage_tick = 0;
    game_stage = new_stage;
}


//Get navswitch move and return associated direction
dir_t get_navswitch_dir(void)
{
    navswitch_update();
    if (navswitch_push_event_p (NAVSWITCH_WEST)) return DIRECTION_W;
    if (navswitch_push_event_p (NAVSWITCH_EAST)) return DIRECTION_E;
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) return DIRECTION_N;
    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) return DIRECTION_S;
    if (navswitch_push_event_p (NAVSWITCH_PUSH)) return DIRECTION_DOWN;
    return DIR_NONE;
}


//Re-start game
void restart_game(void)
{
    tick = 0;
    stage_tick = 0;
    draw_box();
    stage_choose(SELECT);
}

int main (void)
{
	//your main menu will start (havent combined them yet)
	
	// need to choose who starts
	//We can do this by seeing who pushes start first from the main menu screen you made
	int is_seeking = 1;
	
	//while the less than 6 turns have been played (3 turns each)
	while (turn_count < 6) {
		//if i am seeking
		if (is_seeking) {
			take_turn(is_seeking);
		}
		//if i am not seeking
		if (!is_seeking) {
			take_turn(is_seeking);
		}
		//swap from seeking to hiding (or from hiding to seeking) for the next turn
		is_seeking = !is_seeking;
		turn_count += 1;
	}
	
	//show the end score using tinygl
	//show player score on player funkit and opponents score on opponent funkit
	
    return 0;
}
