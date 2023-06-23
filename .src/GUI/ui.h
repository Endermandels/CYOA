/*
CYOA
Elijah Delavar

TODO: Files
TODO: Description
*/

#ifndef UI
#define UI

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 30

// defined in screen.c
extern char display[SCREEN_WIDTH][SCREEN_HEIGHT];
extern int curX;
extern int curY;

void render();
int initUI();
void cleanupUI();

#endif