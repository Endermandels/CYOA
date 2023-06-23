/*
CYOA
Elijah Delavar

TODO: Files
TODO: Description
*/

#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <curses.h>
#include "ui.h"
#include "../Util/exitManager.h"

WINDOW *uiwindow = NULL;

void render() {
    for (int xx = 0; xx < SCREEN_WIDTH; xx++) {
        for (int yy = 0; yy < SCREEN_HEIGHT; yy++) {
            mvaddch(yy, xx, display[xx][yy]); // Draw character to x and y position
        }
    }
    refresh();
}

int initUI() {
    // init curses
    uiwindow = initscr();
    if (!uiwindow) {
        setErrorMessage("!!! Window Init Failure !!!");
        return 1;
    }
    return 0;
}

void cleanupUI() {
    delwin(uiwindow);
    endwin();
    refresh();
}

