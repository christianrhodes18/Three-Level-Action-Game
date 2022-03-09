#include "graphics.h"
#include "circle.h"
#include "player.h"
#include "Quad.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <time.h>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;
int health = 300;
int bossHealth = 60;
int bulletsRemaining = 60;
double surviveTime = 45;
double bulletdelay = 0;

Circle player({1, 1, 1}, {300, 250}, 12);
Circle playerStationary({1, 1, 1}, {300, 250}, 12);
Circle boss({1,1,1},{100,75},30);
vector<Circle> bulletsFromTop;
vector<Circle> bulletsFromLeft;
vector<Circle> bulletsFromRight;
vector<Circle> bulletsLeftWall;
vector<Circle> pistolBulletsL;
vector<Circle> pistolBulletsD;
vector<Circle> pistolBulletsU;
vector<Circle> pistolBulletsR;
vector<Circle> enemiesFromTop;
vector<Circle> enemiesFromLeft;
vector<Circle> enemiesFromRight;
vector<Circle> enemiesFromBottom;
vector<Circle> bossBulletsUR;
vector<Circle> bossBulletsUL;
vector<Circle> bossBulletsLR;
vector<Circle> bossBulletsLL;


Quad hpBar({0, 1, 0}, {175, 550}, 300, 10);
vector<Quad> walls;
int rotation = 1;

const color white(1, 1, 1);
const color black(0, 0, 0);
const color red(1, 0, 0);
const color maroon(.5, 0, 0);
const color purple(.6, .2, .79);
const color pink(.9, .4, .5);
const color blue(0, 0, 1);

GLenum screen;

void init() {
    width = 600;
    height = 600;
    srand(time(0));
    screen = 1; // title screen
}

void initBossBullets(int l){
    //upper left
    if(l == 1){
        bossBulletsUL.push_back(Circle({red}, boss.getCenterX(), boss.getCenterY(), 10));
    }
    //upper right
    if(l == 2){
        bossBulletsUR.push_back(Circle({red}, boss.getCenterX(), boss.getCenterY(), 10));
    }
    //lower right
    if(l == 3){
        bossBulletsLR.push_back(Circle({red}, boss.getCenterX(), boss.getCenterY(), 10));
    }
    //lower left
    if(l == 4){
        bossBulletsLL.push_back(Circle({red}, boss.getCenterX(), boss.getCenterY(), 10));
    }
}

void initBullets() {
    // from top of screen - large sized
    bulletsFromTop.push_back(Circle({purple}, 50, -250, 25));
    bulletsFromTop.push_back(Circle({purple}, 175, -300, 25));
    bulletsFromTop.push_back(Circle({purple}, 375, -325, 25));
    bulletsFromTop.push_back(Circle({purple}, 500, -180, 25));
    bulletsFromTop.push_back(Circle({purple}, 250, -100, 25));
    bulletsFromTop.push_back(Circle({purple}, 300, -200, 25));

    // from left of screen - medium sized
    bulletsFromLeft.push_back(Circle({red}, -100, 50, 12));
    bulletsFromLeft.push_back(Circle({red}, -100, 125, 12));
    bulletsFromLeft.push_back(Circle({red}, -140, 175, 12));
    bulletsFromLeft.push_back(Circle({red}, -380, 250, 12));
    bulletsFromLeft.push_back(Circle({red}, -180, 370, 12));
    bulletsFromLeft.push_back(Circle({red}, -135, 450, 12));

    // from right of screen - small and fast
    bulletsFromRight.push_back(Circle({pink}, 600, 50, 8));
    bulletsFromRight.push_back(Circle({pink}, 685, 175, 8));
    bulletsFromRight.push_back(Circle({pink}, 640, 300, 8));
    bulletsFromRight.push_back(Circle({pink}, 675, 450, 8));

    // left wall
    bulletsLeftWall.push_back(Circle({white}, -15, 0, 12));
}

void initEnemies() {
    // top
        enemiesFromTop.push_back(Circle({maroon}, 300, 0, 8));
    // left
        enemiesFromLeft.push_back(Circle({maroon}, 0, 250, 8));
    // right
        enemiesFromRight.push_back(Circle({maroon}, 600, 250, 8));
    // bottom
        enemiesFromBottom.push_back(Circle({maroon}, 300, 600, 8));
}

void initPistol(int r) {
    // pistol bullets
    //down
    if(r == 1) {
        pistolBulletsD.push_back(Circle({blue}, playerStationary.getCenterX(), playerStationary.getCenterY(), 8));
    }
    //left
    else if(r == 2) {
        pistolBulletsL.push_back(Circle({blue}, playerStationary.getCenterX(), playerStationary.getCenterY(), 8));
    }
    //right
    else if(r == 3) {
        pistolBulletsR.push_back(Circle({blue}, playerStationary.getCenterX(), playerStationary.getCenterY(), 8));
    }
    //up
    else if(r == 4) {
        pistolBulletsU.push_back(Circle({blue}, playerStationary.getCenterX(), playerStationary.getCenterY(), 8));
    }
}

void initPistol2(int r) {
    // pistol bullets
    //down
    if(r == 1) {
        pistolBulletsD.push_back(Circle({blue}, player.getCenterX(), player.getCenterY(), 8));
    }
    //left
    else if(r == 2) {
        pistolBulletsL.push_back(Circle({blue}, player.getCenterX(), player.getCenterY(), 8));
    }
    //right
    else if(r == 3) {
        pistolBulletsR.push_back(Circle({blue}, player.getCenterX(), player.getCenterY(), 8));
    }
    //up
    else if(r == 4) {
        pistolBulletsU.push_back(Circle({blue}, player.getCenterX(), player.getCenterY(), 8));
    }
}

void initWalls() {
    walls.push_back(Quad({1, 1, 1}, {300, 0}, 600, 20)); // top wall
    walls.push_back(Quad({1, 1, 1}, {0, 300}, 20, 600)); // left wall
    walls.push_back(Quad({1, 1, 1}, {300, 600}, 600, 200)); // bottom wall (extra thick)
    walls.push_back(Quad({1, 1, 1}, {600, 300}, 20, 600)); // right wall
}

// Function to check if any point overlaps the given circle and rectangle
bool checkOverlap(int R, int Xc, int Yc,
                  int X1, int Y1,
                  int X2, int Y2)
{

    // Find the nearest point on the
    // rectangle to the center of
    // the circle
    int Xn = max(X1, min(Xc, X2));
    int Yn = max(Y1, min(Yc, Y2));

    // Find the distance between the nearest point and the center of the circle distance between 2 points,
    // (x1, y1) & (x2, y2) in 2D Euclidean space is ((x1-x2)**2 + (y1-y2)**2)**0.5
    int Dx = Xn - Xc;
    int Dy = Yn - Yc;
    return (Dx * Dx + Dy * Dy) <= R * R;
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black and opaque
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /*
     * Draw here
     */
    // screen switch statements here
    switch (screen) {
        case 1: // intro screen
            glRasterPos2i(240, 250);
            glColor3f(1, 1, 1);
            for (const char &m : "Welcome to the game") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(0, 1, 0);
            glRasterPos2i(250, 300);
            for (const char &m : "Press s to start") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(1, 1, 1);
            break;
        case 2: // story update 1
            glRasterPos2i(20, 200);
            glColor3f(1, 1, 1);
            for (const char &m : "You don't know where you are or how you got here.") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 230);
            for (const char &m : "Gunfire erupts all around you. Your heart is racing..") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 260);
            for (const char &m : "All you can think to do is run.") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(0, 1, 0);
            glRasterPos2i(20, 300);
            for (const char &m : "Press spacebar to survive") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(1, 1, 1);
            break;
        case 3: // avoid bullets screen
            if (surviveTime <= 0) {
                screen = 4;
                surviveTime = 45;
            }
            for (const Quad &walls : walls) {
                walls.draw();
            }
            player.draw();
            hpBar.draw();
            glColor3f(0,0,0);
            glRasterPos2i(25, 530);
            for (const char &letter : "Health:") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }

            glColor3f(0,0,0);
            glRasterPos2i(25, 580);
            for (const char &letter : "Remaining Time: " + to_string(int(surviveTime))) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }

            for (const Circle &c : bulletsFromTop) {
                c.draw();
            }
            for (const Circle &c : bulletsFromLeft) {
                c.draw();
            }
            for (const Circle &c : bulletsFromRight) {
                c.draw();
            }

            for (const Circle &c : bulletsLeftWall) {
                c.draw();
            }

            // check for collision between player and bulletsFromTop
            for (int i = 0; i < bulletsFromTop.size(); i++) {
                if (bulletsFromTop[i].isOverlapping(player)) {
                    cout << "collision detected!" << endl;
                }
            }
            // check for collision between player and bulletsFromLeft
            for (int i = 0; i < bulletsFromLeft.size(); i++) {
                if (bulletsFromLeft[i].isOverlapping(player)) {
                    cout << "collision detected!" << endl;
                }
            }
            // check for collision between player and bulletsFromRight
            for (int i = 0; i < bulletsFromRight.size(); i++) {
                if (bulletsFromRight[i].isOverlapping(player)) {
                    cout << "collision detected!" << endl;
                }
            }

            break;
        case 4: // story update 2
            //glColor3f(1, 1, 1);
            glRasterPos2i(20, 200);
            for (const char &m : "The bullets have stopped but while you were running for your life...") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 230);
            for (const char &m : "You suddenly trip over something hard!") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 260);
            for (const char &m : "You pick up the hard object that happens to be a strange handgun.") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 290);
            for (const char &m : "You hear a noise and are frozen in fear. Stand your ground!") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(0, 0, 1);
            glRasterPos2i(20, 320);
            for (const char &m : "Press f to shoot") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(0, 1, 0);
            glRasterPos2i(20, 360);
            for (const char &m : "Press spacebar to survive") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(1, 1, 1);
            break;
        case 5: // stationary shoot to survive screen
            if (surviveTime <= 0) {
                screen = 6;
                surviveTime = 45;
                bulletsRemaining = 60;
            }
            for (const Quad &walls : walls) {
                walls.draw();
            }
            playerStationary.draw();
            hpBar.draw();

            glColor3f(0,0,0);
            glRasterPos2i(400, 530);
            for (const char &letter : "Bullets: " + to_string(bulletsRemaining)) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            if (bulletsRemaining == 0) {
                glColor3f(1,0,0);
                glRasterPos2i(400, 550);
                for (const char &letter : "You are out of bullets") {
                    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
                }
            }
            glColor3f(0,0,0);
            glRasterPos2i(25, 530);
            for (const char &letter : "Health:") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }

            glColor3f(0,0,0);
            glRasterPos2i(25, 580);
            for (const char &letter : "Remaining Time: " + to_string(int(surviveTime))) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }

            for(const Circle &shot : pistolBulletsL){
                shot.draw();
            }
            for(const Circle &shot : pistolBulletsD){
                shot.draw();
            }
            for(const Circle &shot : pistolBulletsU){
                shot.draw();
            }
            for(const Circle &shot : pistolBulletsR){
                shot.draw();
            }

            for (const Circle &c : enemiesFromTop) {
                c.draw();
            }
            for (const Circle &c : enemiesFromLeft) {
                c.draw();
            }
            for (const Circle &c : enemiesFromRight) {
                c.draw();
            }
            for (const Circle &c : enemiesFromBottom) {
                c.draw();
            }

            // check collision between pistol bullets and enemies
            for (int i = 0; i < pistolBulletsU.size(); i++) {
                if (pistolBulletsU[i].isOverlapping(enemiesFromTop[i])) {
                    enemiesFromTop[i].setCenterY(-50);
                    pistolBulletsU.erase(pistolBulletsU.begin());
                }
            }
            for (int i = 0; i < pistolBulletsL.size(); i++) {
                if (pistolBulletsL[i].isOverlapping(enemiesFromLeft[i])) {
                    enemiesFromLeft[i].setCenterX(-70);
                    pistolBulletsL.erase(pistolBulletsL.begin());
                }
            }
            for (int i = 0; i < pistolBulletsR.size(); i++) {
                if (pistolBulletsR[i].isOverlapping(enemiesFromRight[i])) {
                    enemiesFromRight[i].setCenterX(690);
                    pistolBulletsR.erase(pistolBulletsR.begin());
                }
            }
            for (int i = 0; i < pistolBulletsD.size(); i++) {
                if (pistolBulletsD[i].isOverlapping(enemiesFromBottom[i])) {
                    enemiesFromBottom[i].setCenterY(690);
                    pistolBulletsD.erase(pistolBulletsD.begin());
                }
            }

            break;
        case 6: // story update 3
            glRasterPos2i(20, 200);
            for (const char &m : "Eventually, the assault ceases. You heave a sigh of relief...") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 230);
            for (const char &m : "Seconds later, a massive entity smashes into the room.") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 260);
            for (const char &m : "You find ammunition scattered about and reload your weapon.") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 290);
            for (const char &m : "The being roars and begins firing at you. Prepare for a big fight!") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(0, 1, 0);
            glRasterPos2i(20, 320);
            for (const char &m : "Press spacebar to survive") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(1, 1, 1);

            pistolBulletsD.clear();
            pistolBulletsL.clear();
            pistolBulletsR.clear();
            pistolBulletsU.clear();

            break;

        case 7: // boss fight
            for (const Quad &walls : walls) {
                walls.draw();
            }
            player.draw();
            boss.draw();
            hpBar.draw();

            if(bossHealth <= 0){
                screen = 9;
            }

            glColor3f(0,0,0);
            glRasterPos2i(400, 530);
            for (const char &letter : "Bullets: " + to_string(bulletsRemaining)) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            if (bulletsRemaining == 0) {
                glColor3f(1,0,0);
                glRasterPos2i(400, 550);
                for (const char &letter : "You are out of bullets") {
                    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
                }
            }
            glColor3f(0,0,0);
            glRasterPos2i(25, 530);
            for (const char &letter : "Health:") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }

            glRasterPos2i(25, 580);
            for (const char &letter : "Boss Health: " + to_string(int(bossHealth))) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }

            for(const Circle &shot : pistolBulletsL){
                shot.draw();
            }
            for(const Circle &shot : pistolBulletsD){
                shot.draw();
            }
            for(const Circle &shot : pistolBulletsU){
                shot.draw();
            }
            for(const Circle &shot : pistolBulletsR){
                shot.draw();
            }

            for(const Circle &shot : bossBulletsUR){
                if(boss.getCenterX() < 500 && boss.getCenterY() == 75){
                    shot.draw();
                }
            }

            for(const Circle &shot : bossBulletsUL){
                if(boss.getCenterX() == 500 && boss.getCenterY() < 450){
                    shot.draw();
                }
            }

            for(const Circle &shot : bossBulletsLR){
                if(boss.getCenterY() == 450 && boss.getCenterX() > 100){
                    shot.draw();
                }
            }

            for(const Circle &shot : bossBulletsLL){
                if(boss.getCenterX() == 100 && boss.getCenterY() > 75){
                    shot.draw();
                }
            }
            break;
        case 8: // game over
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // dark blue window
            glRasterPos2i(250,250);
            glColor3f(1,0,0);
            for (const char &m : "Game Over!") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            break;
        case 9: // victory
            glRasterPos2i(20, 200);
            for (const char &m : "You have vanquished the massive monster.") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 230);
            for (const char &m : "Everything is silent. You can rest for now it seems.") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glRasterPos2i(20, 260);
            for (const char &m : "However, you feel you are not out of the woods just yet...") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            glColor3f(0,1,0);
            glRasterPos2i(20, 290);
            for (const char &m : "Congratulations! You beat the game!") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m);
            }
            break;
    }

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    // intro screen pass
    if (screen == 1) {
        if (key == 115 || key == 83) { // uppercase or lowercase S
            screen = 2;
        }
    }
    // screen 2 pass
    if (screen == 2) {
        if (key == 32) { // spacebar
            screen = 3;
        }
    }

    // screen 4 pass
    if (screen == 4) {
        if (key == 32) { // spacebar
            screen = 5;
        }
    }

    //screen 6 pass
    if (screen == 6) {
        if(key == 32){ // f
            screen = 7;
        }
    }

    // fire player bullet
    if(screen == 5){
        if (bulletsRemaining > 0) {
            if (key == 102) {
                for (int i = 0; i < 5; i++) {
                    initPistol(rotation);
                }
                bulletsRemaining -= 1;
            }
        }
    }

    //fire player bullets boss fight -- f
    if(screen == 7){
        if (bulletsRemaining > 0) {
            if (key == 102) {
                initPistol2(rotation);
                bulletsRemaining -= 1;
            }
        }
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            player.move(0, 15);
            rotation = 1;
            break;
        case GLUT_KEY_LEFT:
            player.move(-15, 0);
            rotation = 2;
            break;
        case GLUT_KEY_RIGHT:
            player.move(15, 0);
            rotation = 3;
            break;
        case GLUT_KEY_UP:
            player.move(0, -15);
            rotation = 4;
            break;
    }
    glutPostRedisplay();
}


void cursor(int x, int y) {
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    glutPostRedisplay();
}

void timer(int dummy) {
    glutPostRedisplay();
    glutTimerFunc(0, timer, dummy);
}

void gameEventTimer(int dummy) {
    // iterate survive time
    if (screen == 3) {
        surviveTime -= .1;
    }

    // iterate enemy spawn times
    if (screen == 5) {
        surviveTime -=.1;
    }

    if(screen == 7){
        bulletdelay += .1;
    }

    // bullet motion for screen 3
    if (screen == 3) {
        // BULLETS FROM TOP
        for (int i = 0; i < (bulletsFromTop.size()); i++) {
            bulletsFromTop[i].moveY(2);

            // replace bullets to top of screen if they go off
            if (bulletsFromTop[i].getCenterY() > 475) {
                bulletsFromTop[i].setCenterY(-15);
            }

            if(bulletsFromTop[i].isOverlapping(player)){
                bulletsFromTop[i].setCenterY(-15);
                health = health - 60;
                hpBar.resize(health,10);
                hpBar.move(-30,0);
                if(health <= 0){
                    screen = 8;
                }
                if(health <= 150){
                    hpBar.setColor(1,1,0);
                }
                if(health <= 60){
                    hpBar.setColor(1,0,0);
                }
            }
        }

        // BULLETS FROM LEFT
        for (int i = 0; i < bulletsFromLeft.size(); i++) {
            bulletsFromLeft[i].moveX(4);

            // replace bullets to top of screen if they go off
            if (bulletsFromLeft[i].getCenterX() > 550) {
                bulletsFromLeft[i].setCenterX(-15);
            }

            if(bulletsFromLeft[i].isOverlapping(player)){
                bulletsFromLeft[i].setCenterX(-15);
                health = health - 30;
                hpBar.resize(health,10);
                hpBar.move(-15,0);
                if(health <= 0){
                    screen = 8;
                }
                if(health <= 150){
                    hpBar.setColor(1,1,0);
                }
                if(health <= 60){
                    hpBar.setColor(1,0,0);
                }
            }
        }

        // BULLETS FROM RIGHT
        for (int i = 0; i < bulletsFromRight.size(); i++) {
            bulletsFromRight[i].moveX((6) * -1);

            // replace bullets to top of screen if they go off
            if (bulletsFromRight[i].getCenterX() < -50) {
                bulletsFromRight[i].setCenterX(600);
            }

            if (bulletsFromRight[i].isOverlapping(player)) {
                bulletsFromRight[i].setCenterX(600);
                health = health - 14;
                hpBar.resize(health, 10);
                hpBar.move(-7, 0);
                if (health <= 0) {
                    screen = 8;
                }
                if (health <= 150) {
                    hpBar.setColor(1, 1, 0);
                }
                if (health <= 60) {
                    hpBar.setColor(1, 0, 0);
                }
            }
        }
    }

    // bullet movement for screen 5
    if (screen == 5) {
        //DOWN
        for (int i = 0; i < (pistolBulletsD.size()); i++) {
            //shoot down
            pistolBulletsD[i].move(0, 5);

            // if pistol hits wall or collides with enemies, erase pistol bullets
            if(pistolBulletsD[i].getCenterY() > 490 || pistolBulletsD[i].isOverlapping(enemiesFromBottom[i])){
                pistolBulletsD.erase(pistolBulletsD.begin());
            }
            if (pistolBulletsD[i].isOverlapping(enemiesFromBottom[i])) {
                enemiesFromBottom[i].setCenterY(-20);
            }
        }

        for (int i = 0; i < (enemiesFromTop.size()); i++) {
            enemiesFromTop[i].move(0, 6);
            if (enemiesFromTop[i].isOverlapping(playerStationary)) {
                enemiesFromTop[i].setCenterY(-75);
                health = health - 30;
                hpBar.resize(health,10);
                hpBar.move(-15,0);
                if(health <= 0){
                    screen = 8;
                }
                if(health <= 150){
                    hpBar.setColor(1,1,0);
                }
                if(health <= 60){
                    hpBar.setColor(1,0,0);
                }
            }
        }

        //LEFT
        for (int i = 0; i < (pistolBulletsL.size()); i++) {
            //shoot left
            pistolBulletsL[i].move(-5, 0);

            // if pistol hits wall or collides with enemies, erase pistol bullets
            if(pistolBulletsL[i].getCenterX() < 25 || pistolBulletsL[i].isOverlapping(enemiesFromLeft[i])){
                pistolBulletsL.erase(pistolBulletsL.begin());
            }
        }

        for (int i = 0; i < (enemiesFromLeft.size()); i++) {
            enemiesFromLeft[i].move(6, 0);
            if (enemiesFromLeft[i].isOverlapping(playerStationary)) {
                enemiesFromLeft[i].setCenterX(-50);
                health = health - 30;
                hpBar.resize(health,10);
                hpBar.move(-15,0);
                if(health <= 0){
                    screen = 8;
                }
                if(health <= 150){
                    hpBar.setColor(1,1,0);
                }
                if(health <= 60){
                    hpBar.setColor(1,0,0);
                }
            }
        }

        //RIGHT
        for (int i = 0; i < (pistolBulletsR.size()); i++) {
            //shoot right
            pistolBulletsR[i].move(5, 0);

            // if pistol hits wall or collides with enemies, erase pistol bullets
            if(pistolBulletsR[i].getCenterX() > 575 || pistolBulletsR[i].isOverlapping(enemiesFromRight[i])){
                pistolBulletsR.erase(pistolBulletsR.begin());
            }
        }

        for (int i = 0; i < (enemiesFromRight.size()); i++) {
            enemiesFromRight[i].move(-6, 0);
            if (enemiesFromRight[i].isOverlapping(playerStationary)) {
                enemiesFromRight[i].setCenterX(720);
                health = health - 30;
                hpBar.resize(health,10);
                hpBar.move(-15,0);
                if(health <= 0){
                    screen = 8;
                }
                if(health <= 150){
                    hpBar.setColor(1,1,0);
                }
                if(health <= 60){
                    hpBar.setColor(1,0,0);
                }
            }
        }

        //UP
        for (int i = 0; i < (pistolBulletsU.size()); i++) {
            //shoot up
            pistolBulletsU[i].move(0, -5);

            // if pistol hits wall or collides with enemies, erase pistol bullets
            if(pistolBulletsU[i].getCenterY() < 25 || pistolBulletsU[i].isOverlapping(enemiesFromTop[i])){
                pistolBulletsU.erase(pistolBulletsU.begin());
            }
        }

        for (int i = 0; i < (enemiesFromBottom.size()); i++) {
            enemiesFromBottom[i].move(0, -6);
            if (enemiesFromBottom[i].isOverlapping(playerStationary)) {
                enemiesFromBottom[i].setCenterY(800);
                health = health - 30;
                hpBar.resize(health,10);
                hpBar.move(-15,0);
                if(health <= 0){
                    screen = 8;
                }
                if(health <= 150){
                    hpBar.setColor(1,1,0);
                }
                if(health <= 60){
                    hpBar.setColor(1,0,0);
                }
            }
        }
    }

    if(screen == 7){
        //BOSS FIGHT ---- movement
        if(boss.getCenterX() < 500 && boss.getCenterY() == 75){
            boss.move(5,0);
            initBossBullets(1);
        }

        if(boss.getCenterX() == 500 && boss.getCenterY() < 450){
            boss.move(0,5);
            initBossBullets(2);
        }

        if(boss.getCenterY() == 450 && boss.getCenterX() > 100){
            boss.move(-5,0);
            initBossBullets(3);
        }

        if(boss.getCenterX() == 100 && boss.getCenterY() > 75){
            boss.move(0,-5);
            initBossBullets(4);
        }

        //boss fight bullet movement
        //player mechanics ----
        //DOWN
        for (int i = 0; i < (pistolBulletsD.size()); i++) {
            //shoot down
            pistolBulletsD[i].move(0, 5);

            if(pistolBulletsD[i].getCenterY() > 490){
                pistolBulletsD.erase(pistolBulletsD.begin());
            }

            if(pistolBulletsD[i].isOverlapping(boss)){
                bossHealth = bossHealth - 2;
                pistolBulletsD.erase(pistolBulletsD.begin());
            }
        }

        //LEFT
        for (int i = 0; i < (pistolBulletsL.size()); i++) {
            //shoot left
            pistolBulletsL[i].move(-5, 0);

            //FROM RIGHT
            if(pistolBulletsL[i].getCenterX() < 25){
                pistolBulletsL.erase(pistolBulletsL.begin());
            }

            if(pistolBulletsL[i].isOverlapping(boss)){
                bossHealth = bossHealth - 2;
                pistolBulletsL.erase(pistolBulletsL.begin());
            }
        }

        //RIGHT
        for (int i = 0; i < (pistolBulletsR.size()); i++) {
            //shoot right
            pistolBulletsR[i].move(5, 0);

            //FROM LEFT
            if(pistolBulletsR[i].getCenterX() > 575){
                pistolBulletsR.erase(pistolBulletsR.begin());
            }

            if(pistolBulletsR[i].isOverlapping(boss)){
                bossHealth = bossHealth - 2;
                pistolBulletsR.erase(pistolBulletsR.begin());
            }
        }

        //UP
        for (int i = 0; i < (pistolBulletsU.size()); i++) {
            //shoot up
            pistolBulletsU[i].move(0, -5);

            //FROM BOTTOM
            if(pistolBulletsU[i].getCenterY() < 25){
                pistolBulletsU.erase(pistolBulletsU.begin());
            }

            if(pistolBulletsU[i].isOverlapping(boss)){
                bossHealth = bossHealth - 2;
                pistolBulletsU.erase(pistolBulletsU.begin());
            }
        }

        //boss mechanics -----
        //UPPER RIGHT
        for(int i = 0; i < bossBulletsUR.size(); i++) {
            //shoot diagonally right and down
            bossBulletsUR[i].move(2, 2);

            if (bossBulletsUR[i].getCenterY() > 250 || bossBulletsUR[i].getCenterX() < 250) {
                bossBulletsUR.erase(bossBulletsUR.begin());
            }

            if (bossBulletsUR[i].isOverlapping(player)) {
                bossBulletsUR.erase(bossBulletsUR.begin());
                health = health - 2;
                hpBar.resize(health, 10);
                hpBar.move(-1, 0);
                if (health <= 0) {
                    screen = 8;
                }
                if (health <= 150) {
                    hpBar.setColor(1, 1, 0);
                }
                if (health <= 60) {
                    hpBar.setColor(1, 0, 0);
                }
            }
        }
        //UPPER LEFT
        for(int i = 0; i < bossBulletsUL.size(); i++){
            bossBulletsUL[i].move(-2,2);

            //left and down
            if(bossBulletsUL[i].getCenterX() < 25 || bossBulletsUL[i].getCenterY() > 490){
                bossBulletsUL.erase(bossBulletsUL.begin());
            }

            if (bossBulletsUL[i].isOverlapping(player)) {
                bossBulletsUL.erase(bossBulletsUL.begin());
                health = health - 2;
                hpBar.resize(health, 10);
                hpBar.move(-1, 0);
                if (health <= 0) {
                    screen = 8;
                }
                if (health <= 150) {
                    hpBar.setColor(1, 1, 0);
                }
                if (health <= 60) {
                    hpBar.setColor(1, 0, 0);
                }
            }
        }
        //LOWER RIGHT
        for(int i = 0; i < bossBulletsLR.size(); i++){
            bossBulletsLR[i].move(2,-2);

            //up and right
            if(bossBulletsLR[i].getCenterX() > 575 || bossBulletsLR[i].getCenterY() < 25){
                bossBulletsLR.erase(bossBulletsLR.begin());
            }

            if (bossBulletsLR[i].isOverlapping(player)) {
                bossBulletsLR.erase(bossBulletsLR.begin());
                health = health - 2;
                hpBar.resize(health, 10);
                hpBar.move(-1, 0);
                if (health <= 0) {
                    screen = 8;
                }
                if (health <= 150) {
                    hpBar.setColor(1, 1, 0);
                }
                if (health <= 60) {
                    hpBar.setColor(1, 0, 0);
                }
            }
        }
        //LOWER LEFT
        for(int i = 0; i < bossBulletsLL.size(); i++){
            bossBulletsLL[i].move(2,2);

            //right and down
            if(bossBulletsLL[i].getCenterX() > 575 || bossBulletsLL[i].getCenterY() > 490){
                bossBulletsLL.erase(bossBulletsLL.begin());
            }

            if (bossBulletsLL[i].isOverlapping(player)) {
                bossBulletsLL.erase(bossBulletsLL.begin());
                health = health - 2;
                hpBar.resize(health, 10);
                hpBar.move(-1, 0);
                if (health <= 0) {
                    screen = 8;
                }
                if (health <= 150) {
                    hpBar.setColor(1, 1, 0);
                }
                if (health <= 60) {
                    hpBar.setColor(1, 0, 0);
                }
            }
        }
    }

    // create left wall animation
    for (int i = 0; i < bulletsLeftWall.size(); i++) {
        bulletsFromLeft[i].moveY(-5);
    }

    // check collision between player and wall
    for (int i = 0; i < walls.size(); i++) {
        if (checkOverlap(player.getRadius(), player.getCenterX(), player.getCenterY(),
                                            walls[i].getLeftX(), walls[i].getBottomY(),
                                            walls[i].getRightX(), walls[i].getTopY())) {
            int x = player.getCenterX();
            int y = player.getCenterY();
            player.setCenter(x, y + 15);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, gameEventTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();
    initWalls();
    initBullets();
    initEnemies();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Game On" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(0, gameEventTimer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}