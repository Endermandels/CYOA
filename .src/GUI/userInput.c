/*
CYOA
Elijah Delavar

TODO: Files
TODO: Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "userInput.h"
#include "../Util/fileManager.h"

void password(char*,int);
int choose(char*,int,Prompt*);

/*
Receive the user's input given the choices of the given prompt.

@return next title
*/
int choose(char *dest, int n, Prompt *pt) {
    if (!pt) {
        puts("!!! Null Prompt !!!");
        return 1;
    }

    if (pt->numOptions == 0) {
        fgetc(stdin);
        strcpy(dest, "q");      // Quit the game
        return 0;
    }

    short processing = 1;

    while (processing) {
        fgets(dest, n, stdin);

        int nl = strlen(dest) - 1;

        // Lowercase player's free form answer
        if (pt->freeform) {
            for (int ii = 0; ii < nl+1; ii++) {
                dest[ii] = tolower(dest[ii]);
            }
        }
        
        // Delete newline and carriage return
        if (dest[nl] == '\n') {
            dest[nl] = '\0';
            if (dest[nl-1] == '\r') {
                dest[nl-1] = '\0';
            }
        }

        // Quit game
        if (!strcmp(dest, "q")) {
            return 0;
        }

        // Skip Option
        char buffer[DEFAULT_CHAR_ARRAY];
        int lenDest = strlen(dest);
        short skip = 0;
        if (dest[0] == '-') {
            strncpy(buffer, dest+1, lenDest);
            skip = 1;
        } else {
            strncpy(buffer, dest, lenDest+1);
        }
        
        // Check for user's choice and next title
        for (int ii = 0; ii < pt->numOptions; ii++) {
            if (pt->freeform && !strcmp("_", pt->options[ii].choice)) {
                // All other options exhausted
                // Save Keyword
                if (pt->optionKey) {
                    int err = saveKeyword(pt->optionKey, dest);
                    if (err) {
                        return err;
                    }
                }

                // Go To Title
                strncpy(dest, pt->options[ii].goToTitle, strlen(pt->options[ii].goToTitle)+1);
                if (skip) {
                    Prompt *nextPT = getPrompt(pt->options[ii].goToTitle);
                    if (!nextPT) {
                        puts("!!! Null Prompt !!!");
                        return 1;
                    }
                    nextPT->skipDescription = skip;
                }
                processing = 0;
                break;
            }

            // Check for keyword choice
            char keyword[DEFAULT_CHAR_ARRAY];
            if (pt->options[ii].choice[0] == '{') {
                int lenChoice = strlen(pt->options[ii].choice);
                char key[lenChoice];
                strncpy(key, (pt->options[ii].choice)+1, lenChoice-2);
                key[lenChoice-2] = '\0';

                int err = getKeyword(key, keyword);
                if (err) {
                    return err;
                }
            } else {
                strncpy(keyword, pt->options[ii].choice, strlen(pt->options[ii].choice)+1);
            }


            // Check if player's choice matches
            if (!strcmp(buffer, keyword)) {
                strncpy(dest, pt->options[ii].goToTitle, strlen(pt->options[ii].goToTitle)+1);
                if (skip) {
                    Prompt *nextPT = getPrompt(pt->options[ii].goToTitle);
                    if (!nextPT) {
                        puts("!!! Null Prompt !!!");
                        return 1;
                    }
                    nextPT->skipDescription = skip;
                }
                processing = 0;
                break;
            }
        }

        if (processing) {
            puts("- Invalid Answer -");
        }
    }

    puts("");

    return 0;
}