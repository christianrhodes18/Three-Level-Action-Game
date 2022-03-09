//
// Created by thech on 5/07/2021.
//

#ifndef FINAL-PROJECT-CRHODES2-KPOLITZA_H
#define FINAL-PROJECT-CRHODES2-KPOLITZA_H

#include <string>
#include <vector>

class Player
{
public:

    // declare constructor
    Player();

    // draw health bar
    void healthBar(); // draws health bar ; called in graphics draw function > initPlayer > healthBar()

    // set functions
    void setName(std::string); // set name
    void setHealth(int); // set health

    // get functions
    std::string getName(); // get name
    int getHealth(); // get health

private:

    // declare variables
    std::string name;
    int health;
};

#endif //RANDOMIZATION_OPEN_ENDED_PROJECT_CHARACTER_H
