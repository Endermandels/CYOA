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
#include "Game/game.h"
#include "Util/fileManager.h"
#include "Util/exitManager.h"

int main(int argc, char const *argv[])
{
    int err = 0;
    err = readStory();
    if (err) {
        quit();
        return err;
    }

    // err = initUI();
    // if (err) {
    //     quit();
    //     return err;
    // }

    err = gameLoop();
    if (err) {
        quit();
        return err;
    }
    quit();
    return 0;
}
