/*
CYOA
Elijah Delavar

TODO: Files
TODO: Description
*/
#include <stdio.h>
#include <stdlib.h>
#include "../Game/prompts.h"
#include "../GUI/ui.h"

const char *errmsg = NULL;

void quit() {
    cleanupUI();
    if (errmsg) {
        puts(errmsg);
    }
    puts("\nExiting...");
    freePTDLL();
}

void setErrorMessage(const char *msg) {
    errmsg = msg;
}