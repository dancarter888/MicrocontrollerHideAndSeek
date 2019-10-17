ENCE260 ASSIGNMENT | GROUP 444



-------HIDE AND SEEK-------

A Two-player game using the UCFK4 (UC Fun Kit) LED display, navswitch, and IR 
communications. Hide and seek. Both players take turns hiding or seeking.
The seeker gets points based on how accurate they were at finding the hider.


------HOW TO COMPILE--------

-Open terminal and navigate to folder that you wish to install game into
-Run command: git clone https://eng-git.canterbury.ac.nz/ence260-2019/group444.git
-Plug UCFK4 into USB slot
-Run command: make program
-Game will start on UCFK4



------PLAYING THE GAME------

--------INSTRUCTION---------

Before the game starts, one player will automatically be chosen to hide and the other will be the seeker.
Once the game start, you must find where to hide on the board by using the navswitch. Pressing the navswitch up and down, left and right to find a hiding location, and confirm the hiding location by pressing down on the navswitch.

Other player will need to seek where you are hiding by selecting a location on their board using the same technique.
After the positions of both players have been selected, positions of players will be revealed on both UCFK screen. 

Position will be in 3 x 3 matrix (9 dots).

Players will take turn to hide and seek for 4 rounds each. 

---------SCORE---------

Points will be accumulated for each round and the scores will be displayed on both UCFK's screen after round 4, showing who won and who lost. Game ends.

----ACKNOWLEDGEMENTS----

The implementation of this game relies on multiple drivers, fonts and 
utility programs written by M. Hayes.

