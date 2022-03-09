# Three Level Action Game #
## by Christian Rhodes and Kody Politza ##
This project uses graphics API OpenGL and its utility toolkit GLUT to create a top-down bullet hell video game. 
The game environment, as well as characters, boundaries, objects, and projectiles are all modelled with 
GLUT and openGL. The functionality of each of these models will employ the use of inheritance. Each game object 
will inherit qualities from a more basic class. For example, health bars and walls inherit from a quad class, whereas
bullets, players, and enemies will inherit from a circle class.

A demo can be found here: https://youtu.be/X2CsEikTPQk

## Requirements:
freeglut must be installed on the system. To install it, follow the steps found at http://freeglut.sourceforge.net/docs/install.php

## Directions to Run:
- It is recommended to run this application with an IDE. I recommend CLion as it was used during development. After opening in CLion, you will need to select a 'CMakeLists.txt' file. Do this by selecting the 'CMakeLists.txt' file in the root directory, then select 'File' > 'Reload CMake Project' in CLion and a folder called 'cmake-build-debug' will be created. It is essential to move the file named 'freeglut.dll' (in the root directory) into the 'cmake-build-debug' folder for OpenGLL to recognize GLUT. Without doing this, an error such as 'Process finished with exit code -1073741515' will arise. The application will compile, bu the software will not interface with the graphics card as intended. 
- This project assumes a 64-bit system. If you are using 32 bit, edit line 6 of the CMakeLists.txt to accommodate. 
- To run the application and play the game, run the graphics.cpp file

## Screens
1. Title Screen
2. Story Update 1
2. Level 1
3. Story Update 2
4. Level 2
5. Story Update 3
6. Boss Fight
7. Victory Screen
8. Game Over

## Features
- Player Character
    - health gauge that stays centered
    - bullet counter
    - movement and shooting mechanics 
- Damaging Projectiles
- Play area consisting of boundaries.
- Timers to track survival time in each stage.
- Collision detection algorithms on all necessary objects (walls, projectiles, etc).
- Variable level styles.
- Light storyline.

## Level Descriptions
- First Level
    - Classic bullet hell.
    - Different projectiles do different amounts of damage to player.
    - Out last the counter to progress.
- Second Level
    - Stand your ground.
    - The player must hold their ground against an approaching horde.
    - Aim with arrow keys and shoot enemies rushing toward you.
- Boss Fight
    - 2 phase boss fight.
    - Boss has 30 health, player shots do 1.
    - Limited bullets, don't waste shots!

## Play Instructions and Notes
- To move, use the arrow keys. As the narrative shows, movement is only possible on the first and final levels.
- To fire the player's weapon after receiving it, press f.
- The direction in which you fire is dictated by the last direction you moved. Use this to aim accurately.
- The boss will fire a laser of bullets that can hit very hard. Be sure to avoid this attack and shoot him along his pre-determined pathing.
- The boss is most susceptible on his top most path as he will not fire, but he can one shot you!
- The lasers he fires will travel for a short time before becoming invisible. Make sure you pay attention to the trajectory of the bullets to avoid running into the stealth bullets.
- If at any point player health reaches 0, game over screen is passed. 
- If player survives and defeats boss, victory screen is passed.

