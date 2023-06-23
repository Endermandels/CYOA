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
#include "ui.h"

char display[SCREEN_WIDTH][SCREEN_HEIGHT];
int curX = 0;
int curY = 0;

void initScreen() {
    // set the matrix all to black
    for (int xx = 0; xx < SCREEN_WIDTH; xx++) {
        for (int yy = 0; yy < SCREEN_HEIGHT; yy++) {
            display[xx][yy] = 'X';
        }
    }
}

void drawChar(char cc) {
    display[curX][curY] = cc;
    if (++curX >= SCREEN_WIDTH) {
        curX = 0;
        if (++curY >= SCREEN_HEIGHT) {
            curY = 0;
        }
    }
    render();
}

void drawString(char *string) {
    int ii = 0;
    while (string[ii] != '\0') {
        drawChar(string[ii++]);
    }
}