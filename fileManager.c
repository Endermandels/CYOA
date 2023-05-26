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
1.  Title 1
2.  Title 2
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
    char goToTitle[50];

    int MAXLEN = 512;
    char buffer[MAXLEN];
    int ii = 0;

    // Read whole file
    while (fgets(buffer, MAXLEN, fp)) {
        int lenBuffer = strlen(buffer) + 1;

        if (description) {
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
                    free(description);
                    fclose(fp);
                    return 1;
                }
                title = temp;
            }
            strncpy(title, buffer, lenBuffer);
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
                char *temp = (char*)realloc(description, descriptionSize);
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

            if (buffer[0] == '\r' || buffer[0] == '\n') {
                // End of Options
                ii = 0;
            } else {
                // Choice
                int jj = 0;
                while (buffer[jj] != '.') {
                    if (buffer[jj] == '\0') {
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

                // GoToTitle
                int kk = 0;
                while (buffer[jj] != '\n' && buffer[jj] != '\r' && buffer[jj] != '\0') {
                    goToTitle[kk++] = buffer[jj++];
                }
                goToTitle[kk] = '\0';

                int err = addOption(title, choice, goToTitle);
                if (err) {
                    free(title);
                    free(description);
                    fclose(fp);
                    return err;
                }
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

    printPTDLL();

    free(title);
    free(description);
    fclose(fp);
    return 0;
}
