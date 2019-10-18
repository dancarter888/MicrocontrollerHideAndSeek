/**
@file       score_calc.c
@authors    Daniel Siemers (45944203) & Ann Ngo (39979230)
@date       18 October 2019
@brief      Hide And Seek
            Score calculation module.
**/

#include "score_calc.h"

//Overlap function to check how many dots overlap
//Overlap is either a positive number or zero
int check_overlap(int tlx, int tly, int p2tlx, int p2tly)
{
    int overlap = (3 - abs(tlx - p2tlx)) * (3 - abs(tly - p2tly));

    if (overlap < 0) {
        overlap = 0;
    }
    return overlap;
}
