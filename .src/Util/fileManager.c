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
#include "../GUI/userInput.h"

char *keywordsFN = "../.devFiles/keywords.txt";

int readStory();
int saveKeyword(char*,char*);
int getKeyword(char*, char*);

/*
Get keyword correlating to key, it key exists.
*/
int getKeyword(char *key, char *dest) {
    FILE *fp = NULL;
    fp = fopen(keywordsFN, "r");
    if (!fp) {
        puts("!!! File Not Found !!!");
        return 1;
    }

    char buffer[128];
    char *result = NULL;

    while (fgets(buffer, 128, fp)) {
        result = strstr(buffer, key);
        if (result == buffer) {
            // Found key
            result = strchr(buffer, ':');
            int resultLen = strlen(result)-2;
            strncpy(dest, result+2, resultLen);

            // Remove newline
            if (dest[resultLen-1] == '\n') {
                dest[resultLen-1] = '\0';
                if (resultLen-2 >= 0 && dest[resultLen-2] == '\r') {
                    dest[resultLen-2] = '\0';
                }
            }
            fclose(fp);

            return 0;
        }
    }

    fclose(fp);

    strcpy(dest, "[Key Not Found]");
    return 0;
}

/*
Check if key exists, then update it.
If it does not exist, then create it.
*/
int saveKeyword(char *key, char *word) {
    FILE *fp = NULL;
    fp = fopen(keywordsFN, "r");
    if (!fp) {
        puts("!!! File Not Found !!!");
        return 1;
    }

    FILE *tempFP = NULL;
    char *tempFN = "../.devFiles/temp.tmp";
    tempFP = fopen(tempFN, "w");
    if (!tempFP) {
        fclose(fp);
        puts("!!! Unable to Open New File !!!");
        return 1;
    }


    char buffer[128];

    short foundKey = 0;
    short lookForKey = 1;
    short createKey = 1;
    while (fgets(buffer, 128, fp)) {
        int ii = 0;
        int lenKey = strlen(key) + 1;
        if (lookForKey) {
            foundKey = 1;
        }

        // Look for key in text file
        while (lookForKey && buffer[ii] != ':') {
            if (ii >= lenKey || buffer[ii] == '\0' || buffer[ii] != key[ii]) {
                foundKey = 0;
                break;
            }
            ii++;
        }

        if (foundKey) {
            // If it exists, update it
            fprintf(tempFP, "%s: %s\n", key, word);
            lookForKey = 0;
            foundKey = 0;
            createKey = 0;
        } else {
            fprintf(tempFP, "%s", buffer);
        }
    }

    if (createKey) {
        // Otherwise, create it
        fprintf(tempFP, "%s: %s\n", key, word);
    }

    fclose(fp);
    fclose(tempFP);

    remove(keywordsFN); // Delete source file
    rename(tempFN, keywordsFN); // Rename saved file

    return 0;
}

/*
Parse the story text file into prompts.

Format:
--Title--
Description
1.  Desc 1  [--Title 1--]
2.  Desc 2  [--Title 2--]
etc.
*/
int readStory() {
    FILE *fp = NULL;
    char *storyFN = "../.devFiles/Princess.txt";
    fp = fopen(storyFN, "r");
    if (!fp) {
        puts("!!! File Not Found !!!");
        return 1;
    }

    int lineNumber = 0;

    char title[DEFAULT_CHAR_ARRAY];
    int descriptionSize = 0;
    char *description = NULL;
    int delay = 50;
    short todelete = 0;
    short freeform = 0;
    char delayString[16];
    char choiceDescription[412];
    char choice[DEFAULT_CHAR_ARRAY];
    char goToTitle[DEFAULT_CHAR_ARRAY];

    int MAXLEN = 512;
    char buffer[MAXLEN];
    int ii = 0;

    // Read whole file
    while (fgets(buffer, MAXLEN, fp)) {
        lineNumber++;
        int lenBuffer = strlen(buffer) + 1;

        if (description && description[0] != '\0') {
            if (description[strlen(description)-1] == '\n') {
                if (buffer[0] == '1') {
                    // Begin Options
                    ii = 2;
                } else if (buffer[0] == '*') {
                    // Begin Free Form Options
                    ii = 2;
                    int err = formify(title);
                    if (err) {
                        free(description);
                        fclose(fp);
                        return err;
                    }
                    freeform = 1;
                    if (lenBuffer - 1 > 2) {
                        // Add key
                        buffer[lenBuffer-2] = '\0'; // Remove newline
                        if (buffer[lenBuffer-3] == '\r') {
                            buffer[lenBuffer-3] = '\0';
                        }
                        err = addKey(title,buffer+1);
                        if (err) {
                            free(description);
                            fclose(fp);
                            return err;
                        }
                    }
                    continue;
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

            if (lenBuffer > DEFAULT_CHAR_ARRAY) {
                printf("!!! Invalid Title Size (Line %d) !!!\n", lineNumber);
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

            // Empty Description
            if (description) {
                description[0] = '\0';
            }

            // Reset variables
            delay = 50;
            todelete = 0;
            freeform = 0;

            // Store Prompt
            int err = storePrompt(title);
            if (err) {
                if (description) {
                    free(description);
                }
                fclose(fp);
                return err;
            }

            // Go to description
            ii = 1;
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
                        if (strlen(delayString) > 0) {
                            delay = atoi(delayString);
                            if (!delay) {
                                printf("!!! Invalid Delay (Line %d) !!!\n", lineNumber);
                                free(description);
                                fclose(fp);
                                return 1;
                            }
                        }
                    } else {
                        // Continue reading delay
                        if (kk >= 16) {
                            printf("!!! Too Long of a Delay (Line %d) !!!\n", lineNumber);
                            free(description);
                            fclose(fp);
                            return 1;
                        }

                        if (buffer[jj] == 'd') {
                            // Delete
                            todelete = 1;
                            jj++;
                        } else if (buffer[jj] == 'e') {
                            // End Delete
                            todelete = 0;
                            jj++;
                        } else {
                            // Read Delay
                            delayString[kk++] = buffer[jj++];
                        }
                    }
                } else if (buffer[jj] == '[') {
                    // Change delay (New Description Segment)
                    if (strlen(description) > 0) {
                        description[ll] = '\0';
                        addDescriptionSegment(title, description, delay, todelete);
                        description[0] = '\0';
                        delay = 50;
                        ll = 0;
                    }
                    readDelay = 1;
                    jj++;
                } else {
                    // Concatenate buffer to description
                    description[ll++] = buffer[jj++];
                }
            }

            if (readDelay) {
                printf("!!! Still Reading Delay (Line %d) !!!\n", lineNumber);
                free(description);
                fclose(fp);
                return 1;
            }

            description[ll] = '\0';
        } else if (ii == 2) {
            // Options
            if ((!freeform && !strchr("123456789", buffer[0])) || strchr(" \t\r\n", buffer[0])) {
                // End of Options
                ii = 3;
            } else {
                // Choice
                int jj = 0;
                while (buffer[jj] != '.') {
                    if (buffer[jj] == '\0' || jj >= DEFAULT_CHAR_ARRAY) {
                        printf("!!! Improperly Formated Option (Choice @ %d) !!!\n", lineNumber);
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
                if (!freeform) {
                    int kk = 0;
                    while (buffer[jj] != '[') {
                        if (buffer[jj] == '\0') {
                            printf("!!! Improperly Formated Option (Choice Description @ %d) !!!\n", lineNumber);
                            free(description);
                            fclose(fp);
                            return 1;
                        }
                        choiceDescription[kk++] = buffer[jj++];
                    }
                    choiceDescription[kk] = '\0';
                }
                jj++; // Skip '['

                // GoToTitle
                int kk = 0;
                while (buffer[jj] != ']') {
                    if (kk >= DEFAULT_CHAR_ARRAY) {
                        printf("!!! Improperly Formated Option (Go To Title @ %d) !!!\n", lineNumber);
                        free(description);
                        fclose(fp);
                        return 1;
                    }
                    goToTitle[kk++] = buffer[jj++];
                }
                goToTitle[kk] = '\0';

                int err = addOption(title, choice, choiceDescription, goToTitle, freeform);
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
                addDescriptionSegment(title, description, delay, todelete);
                description[0] = '\0';
                delay = 50;
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
        addDescriptionSegment(title, description, delay, todelete);
    }

    free(description);
    fclose(fp);
    return 0;
}
