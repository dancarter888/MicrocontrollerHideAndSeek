ENCE260 ASSIGNMENT | GROUP 444
Member 1: Daniel Siemers
Member 2: Ann Ngo



-------HIDE AND SEEK-------

A Two-player game using the UCFK4 (UC Fun Kit) LED display, navswitch, and IR
communications. Hide and seek. Both players take turns hiding or seeking.
The seeker gets points based on how accurate they were at finding the hider.


------HOW TO COMPILE--------

-Open terminal and navigate to folder that you wish to install game into
-Run command: git clone https://eng-git.canterbury.ac.nz/ence260-2019/group444.git
-Plug UCFK4 into USB slot
-Run command: make
-Run command: make program
-Game will start on UCFK4



------PLAYING THE GAME------

--------INSTRUCTIONS---------

Before the game starts, one player will choose to hide and the other will automatically be the seeker. Blue LED light indicates it is your turn to move your player and select a location
Once the game starts, you must find where to hide on the board by using the navswitch. Pressing the navswitch north, south, east and west will move your player
and pushing down on the navswitch will confirm your player location and halt player movement.

The other player will need to seek where you are hiding by selecting a location on their board using the same technique.
After the positions of both players have been selected, the positions of both players will be revealed on both UCFK screens.

Positions will be in 3 x 3 matrix (9 dots).

Players will take turn to hide and seek for 4 rounds each

After 4 rounds have been taken by each player, a prompt will display on the screen of the most recent seeker asking if they would like to replay.
(Keep fun kit orientated so that text is displayed unrotated) Pressing north on the navswitch will restart the game and display the main menu on both fun kits
Pressing south will exit the game for both players and wipe the screen on both fun kits

---------SCORE---------

When a player is seeking, they have the opportunity to gain points
The seeker gains points based on how many dots of their player overlap with the dots of the hider. This means the seeker is able to gain a maximum of 9 points per round
if they are able to exactly guess where the hider is hiding.
Points will be accumulated for each round and the scores will be displayed on both UCFK's screen after each round, last round showing who won ('W') and who lost ('L') or if both players tied ('T').

----ACKNOWLEDGEMENTS----

The implementation of this game relies on multiple drivers, fonts and
utility programs written by M. Hayes.

