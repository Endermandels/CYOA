/*
CYOA
Elijah Delavar

TODO: Files

Read in story prompts from associated text file.
Read game files.
Write to game files.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prompts.h"

char *storyFN = "story.txt";

int readStory();
int readGF();
int writeGF();

/*
Parse the story text file into prompts.

Format:
Title
Description (may contain newlines)
1.
2.
3.
4.
etc.
*/
int readStory() {
    FILE *fp = NULL;
    fp = fopen(storyFN, "r");
    if (!fp) {
        puts("!!! File Not Found !!!");
        return 1;
    }

    int titleSize = 0;
    char *title = NULL;
    int descriptionSize = 0;
    char *description = NULL;

    int MAXLEN = 512;
    char buffer[MAXLEN];
    int ii = 0;

    // Read whole file
    while (fgets(buffer, MAXLEN, fp)) {
        int lenBuffer = strlen(buffer) + 1;

        if (ii == 0) {
            // Title
            if (!title) {
                // Init title
                titleSize = sizeof(char)*lenBuffer;
                title = (char*)malloc(titleSize);
                if (!title) {
                    puts("!!! Memory Allocation Failure !!!");
                    fclose(fp);
                    return 1;
                }
            } else if (lenBuffer > titleSize) {
                // Resize title
                titleSize = sizeof(char)*lenBuffer;
                char *temp = (char*)realloc(title, titleSize);
                if (!temp) {
                    puts("!!! Memory Allocation Failure !!!");
                    free(title);
                    fclose(fp);
                    return 1;
                }
            }
            strncpy(title, buffer, lenBuffer);
            ii++;
        } else if (ii == 1) {
            // Description

        }

        // storePrompt(title, description);
    }

    puts(title);

    free(title);

    fclose(fp);
    return 0;
}
