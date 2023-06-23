/*
CYOA
Elijah Delavar

TODO: Files
TODO: Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <curses.h>
#include "ui.h"

char display[SCREEN_WIDTH][SCREEN_HEIGHT];
int curX;
int curY;

void initDisplay() {
    // set the matrix all to black
    for (int xx = 0; xx < SCREEN_WIDTH; xx++) {
        for (int yy = 0; yy < SCREEN_HEIGHT; yy++) {
            display[xx][yy] = 'X';
        }
    }
    curX = 0;
    curY = 0;
}

void drawChar(char cc) {
    if (cc == '\n') {
        curX = 0;
        if (++curY >= SCREEN_HEIGHT) {
            // Bottom of screen
            initDisplay();
            render();
        }
    } else {
        display[curX][curY] = cc;
        mvaddch(curY, curX, cc);

        if (++curX >= SCREEN_WIDTH) {
            curX = 0;
            if (++curY >= SCREEN_HEIGHT) {
                // Bottom of screen
                initDisplay();
                render();
            }
        }
    }
    move(curY, curX);
    refresh();
}

void drawString(char *string) {
    int ii = 0;
    while (string[ii] != '\0') {
        drawChar(string[ii++]);
    }
}