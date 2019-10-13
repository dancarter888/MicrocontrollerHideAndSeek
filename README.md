ENCE260 ASSIGNMENT | GROUP 444



-------HIDE AND SEEK-------

A Two-player game using the UCFK4 (UC Fun Kit) LED display, navswitch, and IR 
communications. Hide and seek. Find where the opponent hide and smash them.


------HOW TO COMPILE--------

-Open terminal and navigate to folder that you wish to install game into
-Run command: git clone https://eng-git.canterbury.ac.nz/ence260-2019/group444.git
-Plug UCFK4 into USB slot
-Run command: make program
-Game will start on UCFK4



------PLAYING THE GAME------

--------INSTRUCTION---------

Before the game start, one player will automatically be chosen to hide and the other will be the seeker.
Once the game start, you must find where to hide on the board by using the navswitch. Pressing the navswitch up and down, left and right to find a hiding location, and confirm the hiding location by using the button located on the other side of the LED array. 

Other player will need to seek where you are hiding by selecting a location on their board using the same technique. Each player has 5 seconds to select a position to hide and to seek.
After 5 second, positions of players will be revealed on both UCFK screen. 

Flashing lights indicate the right position. Solid lights indicate the position that the seeker miss.
Position will be in 2 x 2 matrix (4 dots).

Players will take turn to hide and seek for 10 rounds or until one player can find exactly your 4 dot hiding position. 

---------SCORE---------

Points will be accumulate for each round and the scores will be display on both UCFK's screen after round 10, showing who win and who lose. When a player manages to find total 4 dot of your hiding position, then that player will win automatically. Game ends.

----ACKNOWLEDGEMENTS----

The implementation of this game relies on multiple drivers, fonts and 
utility programs written by M. Hayes.

