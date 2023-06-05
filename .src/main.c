/*
CYOA
Elijah Delavar

TODO: Files
TODO: Compilation
TODO: Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "fileManager.h"
#include "exitManager.h"

int main(int argc, char const *argv[])
{
    int err = 0;
    // err = chooseStory();
    // if (err) {
    //     quit();
    //     return err;
    // }
    err = readStory();
    if (err) {
        quit();
        return err;
    }
    err = gameLoop();
    if (err) {
        quit();
        return err;
    }
    quit();
    return 0;
}
