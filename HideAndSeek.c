#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 10


void draw_box(int tlx, int tly, int brx, int bry)
{
    tinygl_draw_box(tinygl_point(tlx, tly), tinygl_point(brx, bry), 1);
    tinygl_pixel_set(tinygl_point(tlx + 1, tly + 1), 1);
}

int* move_player(int tlx, int tly, int brx, int bry){
	
	int coords[] = {tlx, tly, brx, bry};
	
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
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH)){
			coords = {tlx, tly, brx, bry};
			return coords;
        }
        
        tinygl_clear();
        draw_box(tlx, tly, brx, bry);     
    }
}

int main (void)
{
	int tlx = 1;
	int tly = 4;
	int brx = 3;
	int bry = 6;
	int coords[] = {tlx, tly, brx, bry};

    system_init ();
    tinygl_init (PACER_RATE * 2);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    navswitch_init ();

    /* TODO: Initialise IR driver.  */

    pacer_init (PACER_RATE);
    
    coords = move_player(tlx, tly, brx, bry);
    
    while (1) {
		pacer_wait();
        tinygl_update ();
		draw_box(tlx, tly, brx, bry);
	}
    

    return 0;
}
