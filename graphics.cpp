/*
 * Project name: Food collection
 * Version 3
 * Student : Albert Eduard Merino Pulido
 */
#include "graphics.h"

using namespace std;

const char * Graphics::windowTitle = "Food collection - Merino";

// Constructors
Graphics::Graphics(){ }

// Getters
Graphics& Graphics::getInstance(){
    static Graphics instance;

    return instance;
}

// Getters
int Graphics::getHeight(){ return heigth; }

int Graphics::getWidth(){ return width; }

int Graphics::getMaxHeigth(){ return glutGet(GLUT_SCREEN_HEIGHT) / Drawer::cellSize; }

int Graphics::getMaxWidth(){ return glutGet(GLUT_SCREEN_WIDTH) / Drawer::cellSize; }

// Setters
void Graphics::setGame(Game g){ game = g; }

// Methods
void Graphics::init(int argc, char * argv[]){
    glutInit(&argc, argv);
}

void Graphics::start(){
    heigth     = game.getHeight() * Drawer::cellSize;
    width      = game.getWidth() * Drawer::cellSize;
    last_t     = 0;
    anglealpha = 90;
    anglebeta  = -45; // -45

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, heigth);
    glutCreateWindow(Graphics::windowTitle);

    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecial);
    glutIdleFunc(myIdle);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
}

void Graphics::display(){
    glClearColor(Color::background.red1, Color::background.green1, Color::background.blue1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    positionObserver(anglealpha, anglebeta, 450);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width * 0.5, width * 0.5, -heigth * 0.5, heigth * 0.5, 50, 2000);

    glMatrixMode(GL_MODELVIEW);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    game.draw();

    glutSwapBuffers();
} // display

void Graphics::positionObserver(float alpha, float beta, int radi){
    float x, y, z;
    float upx, upy, upz;
    float modul;

    x = (float) radi * cos(alpha * 2 * M_PI / 360.0) * cos(beta * 2 * M_PI / 360.0);
    y = (float) radi * sin(beta * 2 * M_PI / 360.0);
    z = (float) radi * sin(alpha * 2 * M_PI / 360.0) * cos(beta * 2 * M_PI / 360.0);

    if (beta > 0) {
        upx = -x;
        upz = -z;
        upy = (x * x + z * z) / y;
    } else if (beta == 0) {
        upx = 0;
        upy = 1;
        upz = 0;
    } else {
        upx = x;
        upz = z;
        upy = -(x * x + z * z) / y;
    }


    modul = sqrt(upx * upx + upy * upy + upz * upz);

    upx = upx / modul;
    upy = upy / modul;
    upz = upz / modul;

    gluLookAt(x, y, z, 0.0, 0.0, 0.0, upx, upy, upz);
}

void Graphics::keyboard(unsigned char c, int x, int y){
    CellType cellType = PLAYER;

    if (c == 'r') game.resetGame();
    else if (c == 'w') game.moveAgent(cellType, UP);
    else if (c == 's') game.moveAgent(cellType, DOWN);
    else if (c == 'a') game.moveAgent(cellType, LEFT);
    else if (c == 'd') game.moveAgent(cellType, RIGHT);

    else if (c == 'i' && anglebeta <= (90 - 4)) anglebeta = (anglebeta + 3);
    else if (c == 'k' && anglebeta >= (-90 + 4)) anglebeta = anglebeta - 3;
    else if (c == 'j') anglealpha = (anglealpha + 3) % 360;
    else if (c == 'l') anglealpha = (anglealpha - 3 + 360) % 360;

    else if (c == '+' && Agent::duration > 100) Agent::duration -= 50;
    else if (c == '-' && Agent::duration < 500) Agent::duration += 50;

    glutPostRedisplay();
}

void Graphics::special(int key, int x, int y){
    CellType cellType = PLAYER;

    switch (key) {
        case GLUT_KEY_UP:
            game.moveAgent(cellType, UP);
            break;
        case GLUT_KEY_DOWN:
            game.moveAgent(cellType, DOWN);
            break;
        case GLUT_KEY_LEFT:
            game.moveAgent(cellType, LEFT);
            break;
        case GLUT_KEY_RIGHT:
            game.moveAgent(cellType, RIGHT);
            break;
    }
    glutPostRedisplay();
}

void Graphics::idle(){
    long t = glutGet(GLUT_ELAPSED_TIME);

    if (last_t == 0) {
        last_t = t;
    } else {
        game.integrate(t - last_t);
        last_t = t;
    }

    glutPostRedisplay();
}

void myDisplay(){ Graphics::getInstance().display(); }

void myKeyboard(unsigned char c, int x, int y){ Graphics::getInstance().keyboard(c, x, y); }

void mySpecial(int key, int x, int y){ Graphics::getInstance().special(key, x, y); }

void myIdle(){ Graphics::getInstance().idle(); }
