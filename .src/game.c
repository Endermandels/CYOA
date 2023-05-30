/*
CYOA
Elijah Delavar

TODO: Files
TODO: Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prompts.h"
#include "userInput.h"

int gameLoop();

/*
Show prompt.
Receive user input.
Go to next prompt.
*/
int gameLoop() {
    short running = 1;
    int err = 0;
    Prompt *cur = getStart();

    while (running) {
        // Show prompt
        err = printPT(cur);
        if (err) {
            return err;
        }
        
        // Receive user input
        char nextTitle[50];
        int err = choose(nextTitle, 50, cur);
        if (err) {
            return err;
        }

        // Quit game
        if (!strcmp(nextTitle, "q")) {
            break;
        }

        // Go to next prompt
        cur = getPrompt(nextTitle);
        if (!cur) {
            puts("Something went wrong");
        }
    }

    return 0;
}