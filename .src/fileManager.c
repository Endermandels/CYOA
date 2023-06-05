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
#include "userInput.h"

char *storyFN = "../.devFiles/Princess.txt";

int chooseStory();
int readStory();
int readGF();
int writeGF();

/*
Choose the story based off game files and user's password.
*/
int chooseStory() {
    char username[50];
    password(username, 50);

    puts(username);

    return 0;
}

/*
Parse the story text file into prompts.

Format:
--Title--
Description (may contain newlines)
1.  Desc 1  [--Title 1--]
2.  Desc 2  [--Title 2--]
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
    char choice[10];
    char choiceDescription[452];
    char goToTitle[50];

    int MAXLEN = 512;
    char buffer[MAXLEN];
    int ii = 0;

    // Read whole file
    while (fgets(buffer, MAXLEN, fp)) {
        int lenBuffer = strlen(buffer) + 1;

        if (description && description[0] != '\0') {
            if (description[strlen(description)-1] == '\n') {
                if (buffer[0] == '1') {
                    // Begin Options
                    ii = 2;
                } else if (buffer[0] == '!') {
                    // End
                    ii = 3;
                }
            }
        }

        if (ii == 0) {
            // Title
            if (buffer[0] == '\r' || buffer[0] == '\n') {
                // Skip ahead to next title
                continue;
            }

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
                char *temp = NULL;
                temp = (char*)realloc(title, titleSize);
                if (!temp) {
                    puts("!!! Memory Allocation Failure !!!");
                    free(title);
                    free(description);
                    fclose(fp);
                    return 1;
                }
                title = temp;
            }
            strncpy(title, buffer, lenBuffer);
            if (title[lenBuffer-2] == '\n') {
                title[lenBuffer-2] = '\0';
                if (title[lenBuffer-3] == '\r') {
                    title[lenBuffer-3] = '\0';
                }
            }
            ii++;

            // Empty Description
            if (description) {
                description[0] = '\0';
            }
        } else if (ii == 1) {
            // Description
            if (!description) {
                // Init description
                descriptionSize = sizeof(char)*lenBuffer;
                description = (char*)malloc(descriptionSize);
                if (!description) {
                    puts("!!! Memory Allocation Failure !!!");
                    free(title);
                    fclose(fp);
                    return 1;
                }
                description[0] = '\0';
            } else if (lenBuffer + strlen(description) + 1 > descriptionSize) {
                // Resize description
                descriptionSize += sizeof(char)*lenBuffer;
                char *temp = NULL;
                temp = (char*)realloc(description, descriptionSize);
                if (!temp) {
                    puts("!!! Memory Allocation Failure !!!");
                    free(title);
                    free(description);
                    fclose(fp);
                    return 1;
                }
                description = temp;
            }
            strncat(description, buffer, lenBuffer);
        } else if (ii == 2) {
            // Options
            if (buffer[0] == '1') {
                int err = storePrompt(title, description);
                if (err) {
                    free(title);
                    free(description);
                    fclose(fp);
                    return err;
                }
            }

            // End of Options
            if (buffer[0] == '\r' || buffer[0] == '\n') {
                ii = 0;
                continue;
            }

            // Choice
            int jj = 0;
            while (buffer[jj] != '.') {
                if (buffer[jj] == '\0' || jj >= 10) {
                    puts("!!! Improperly Formated Option !!!");
                    free(title);
                    free(description);
                    fclose(fp);
                    return 1;
                }
                choice[jj] = buffer[jj];
                jj++;
            }
            choice[jj] = '\0';
            jj++;


            // Clear whitespace
            while (buffer[jj] == ' ' || buffer[jj] == '\t') {
                jj++;
            }

            // Choice Description
            int kk = 0;
            while (buffer[jj] != '[') {
                if (buffer[jj] == '\0') {
                    puts("!!! Improperly Formated Option !!!");
                    free(title);
                    free(description);
                    fclose(fp);
                    return 1;
                }
                choiceDescription[kk++] = buffer[jj++];
            }
            choiceDescription[kk] = '\0';
            jj++;

            // GoToTitle
            kk = 0;
            while (buffer[jj] != ']') {
                if (kk >= 50) {
                    puts("!!! Improperly Formated Option !!!");
                    free(title);
                    free(description);
                    fclose(fp);
                    return 1;
                }
                goToTitle[kk++] = buffer[jj++];
            }
            goToTitle[kk] = '\0';

            int err = addOption(title, choice, choiceDescription, goToTitle);
            if (err) {
                free(title);
                free(description);
                fclose(fp);
                return err;
            }
        } else if (ii == 3) {
            // End
            int err = storePrompt(title, description);
            if (err) {
                free(title);
                free(description);
                fclose(fp);
                return err;
            }
            ii = 0;
        }
    }

    // printPTDLL();

    free(title);
    free(description);
    fclose(fp);
    return 0;
}
