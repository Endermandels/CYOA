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

char *storyFN = "../.devFiles/Guest.txt";

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

    char title[50];
    int descriptionSize = 0;
    char *description = NULL;
    int delay = 50;
    char delayString[16];
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

            if (lenBuffer > 50) {
                puts("!!! Invalid Title Size !!!");
                if (description) {
                    free(description);
                }
                fclose(fp);
                return 1;
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

            // Reset Delay
            delay = 50;

            // Store Prompt
            int err = storePrompt(title);
            if (err) {
                if (description) {
                    free(description);
                }
                fclose(fp);
                return err;
            }
        } else if (ii == 1) {
            // Description
            if (!description) {
                // Init description
                descriptionSize = sizeof(char)*lenBuffer;
                description = (char*)malloc(descriptionSize);
                if (!description) {
                    puts("!!! Memory Allocation Failure !!!");
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
                    free(description);
                    fclose(fp);
                    return 1;
                }
                description = temp;
            }

            // Description Segments
            int jj = 0;
            int kk = 0;
            int lenDescription = strlen(description);
            int ll = lenDescription;
            short readDelay = 0;
            while (buffer[jj] != '\0') {
                if (readDelay) {
                    // Read the delay
                    if (buffer[jj] == ']') {
                        // Stop reading delay
                        readDelay = 0;
                        delayString[kk] = '\0';
                        kk = 0;
                        jj++;

                        // Convert Delay String to Integer
                        delay = atoi(delayString);
                        if (!delay) {
                            puts("!!! Empty Delay !!!");
                            free(description);
                            fclose(fp);
                            return 1;
                        }
                    } else {
                        // Continue reading delay
                        if (kk >= 16) {
                            puts("!!! Too Long of a Delay !!!");
                            free(description);
                            fclose(fp);
                            return 1;
                        }

                        delayString[kk++] = buffer[jj++];
                    }
                } else if (buffer[jj] == '[') {
                    // Change delay (New Description Segment)
                    if (strlen(description) > 0) {
                        description[ll] = '\0';
                        addDescriptionSegment(title, description, delay);
                        description[0] = '\0';
                        ll = 0;
                    }
                    readDelay = 1;
                    jj++;
                } else {
                    // Concatenate buffer to description
                    description[ll++] = buffer[jj++];
                }
            }
            description[ll] = '\0';
        } else if (ii == 2) {
            // Options
            if (buffer[0] == '\r' || buffer[0] == '\n') {
                // End of Options
                ii = 3;
            } else {
                // Choice
                int jj = 0;
                while (buffer[jj] != '.') {
                    if (buffer[jj] == '\0' || jj >= 10) {
                        puts("!!! Improperly Formated Option !!!");
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
                        free(description);
                        fclose(fp);
                        return 1;
                    }
                    goToTitle[kk++] = buffer[jj++];
                }
                goToTitle[kk] = '\0';

                int err = addOption(title, choice, choiceDescription, goToTitle);
                if (err) {
                    free(description);
                    fclose(fp);
                    return err;
                }
            }
        }
        
        if (ii == 3) {
            // Add Description Segment
            int lenDescription = strlen(description);
            if (lenDescription > 0) {
                if (description[lenDescription-1] != '\n') {
                    description[lenDescription] = '\n';
                    description[lenDescription+1] = '\0';
                }
                addDescriptionSegment(title, description, delay);
                description[0] = '\0';
            }
            ii = 0;
        }
    }

    // Add last Description Segment
    int lenDescription = strlen(description);
    if (lenDescription > 0) {
        if (description[lenDescription-1] != '\n') {
            description[lenDescription] = '\n';
            description[lenDescription+1] = '\0';
        }
        addDescriptionSegment(title, description, delay);
    }

    free(description);
    fclose(fp);
    return 0;
}
