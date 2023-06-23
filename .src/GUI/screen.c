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

#define BLANK 'X'

char display[SCREEN_WIDTH][SCREEN_HEIGHT];
int curX, curY;

void initDisplay() {
    // set the matrix all to black
    for (int xx = 0; xx < SCREEN_WIDTH; xx++) {
        for (int yy = 0; yy < SCREEN_HEIGHT; yy++) {
            display[xx][yy] = BLANK;
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

/*
Delete character at cursor position.

@return Whether the top left corner was reached
*/
int deleteChar() {
    display[curX][curY] = BLANK;
    mvaddch(curY, curX, BLANK);
    if (--curX < 0) {
        if(--curY < 0) {
            // Top Left Corner
            curX = 0;
            curY = 0;
            move(curY, curX);
            refresh();
            return 1;
        }
        curX = SCREEN_WIDTH-1;
    }
    move(curY, curX);
    refresh();
    return 0;
}