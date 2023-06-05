/*
CYOA
Elijah Delavar

TODO: Files

Everything related to prompts.
Store prompt.
Get prompt.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "prompts.h"

Prompt *start;
Prompt *end;

int printPT(Prompt*);
void printPTDLL();
Prompt *freePT(Prompt*);
void freePTDLL();
Prompt *getStart();
Prompt *getPrompt(char*);
int storePrompt(char*);
int addDescriptionSegment(char*,char*,int);
int addOption(char*,char*,char*,char*);

/*
Print the prompt for the user to answer.
Prints one character at a time.
*/
int printPT(Prompt *pt) {
    if (!pt) {
        puts("!!! Null Prompt !!!");
        return 1;
    }

    for (int ii = 0; ii < pt->numDescriptionSegments; ii++) {
        // Milliseconds between prints
        long msec = pt->ds[ii].delay;

        struct timespec ts;
        ts.tv_sec = msec / 1000;
        ts.tv_nsec = (msec % 1000) * 1000000;

        int jj = 0;
        while (pt->ds[ii].description[jj] != '\0') {
            printf("%c", pt->ds[ii].description[jj++]);
            fflush(stdout);
            nanosleep(&ts, &ts);
        }
    }
    puts("");

    for (int ii = 0; ii < pt->numOptions; ii++) {
        printf("%s.  %s\n", pt->options[ii].choice, pt->options[ii].choiceDescription);
    }
    return 0;
}

/*
Print all Prompts' attributes.
*/
void printPTDLL() {
    Prompt *cur = start;
    while (cur) {
        puts(cur->title);
        for (int ii = 0; ii < cur->numDescriptionSegments; ii++) {
            puts(cur->ds[ii].description);
            printf("%d\n", cur->ds[ii].delay);
        }
        for (int ii = 0; ii < cur->numOptions; ii++) {
            puts(cur->options[ii].choice);
            puts(cur->options[ii].choiceDescription);
            puts(cur->options[ii].goToTitle);
        }
        cur = cur->next;
    }
}

/*
Free prompt and return the next prompt.
*/
Prompt *freePT(Prompt *pt) {
    if (!pt) {
        puts("!!! Cannot Free Null Prompt !!!");
        return NULL;
    }

    if (pt->title) {
        free(pt->title);
    }
    if (pt->ds) {
        for (int ii = 0; ii < pt->numDescriptionSegments; ii++) {
            if (pt->ds[ii].description) {
                free(pt->ds[ii].description);
            }
        }
        free(pt->ds);
    }
    if (pt->options) {
        for (int ii = 0; ii < pt->numOptions; ii++) {
            if (pt->options[ii].choice) {
                free(pt->options[ii].choice);
            }
            if (pt->options[ii].choiceDescription) {
                free(pt->options[ii].choiceDescription);
            }
            if (pt->options[ii].goToTitle) {
                free(pt->options[ii].goToTitle);
            }
        }
        free(pt->options);
    }

    Prompt *next = pt->next;
    free(pt);
    return next;
}

/*
Free Prompt Doubly Linked List.
*/
void freePTDLL() {
    Prompt *cur = start;
    while(cur) {
        cur = freePT(cur);
    }
}

Prompt *getStart() {
    return start;
}

/*
Search through Prompt DLL to find Prompt with given title.
*/
Prompt *getPrompt(char *title) {
    Prompt *cur = start;
    while (cur) {
        if (!strcmp(cur->title, title)) {
            return cur;
        }
        cur = cur->next;
    }
    return cur;
}

/*
Add new prompt to DLL.
*/
int storePrompt(char *title) {
    Prompt *new = NULL;
    new = (Prompt*)malloc(sizeof(Prompt));
    if (!new) {
        puts("!!! Memory Allocation Failure !!!");
        return 1;
    }

    new->title = NULL;
    new->numDescriptionSegments = 0;
    new->ds = NULL;
    new->numOptions = 0;
    new->options = NULL;
    new->next = NULL;
    new->prev = NULL;

    // Title
    int lenCopy = strlen(title) + 1;
    new->title = (char*)malloc(sizeof(char)*lenCopy);
    if (!new->title) {
        puts("!!! Memory Allocation Failure !!!");
        freePT(new);
        return 1;
    }
    strncpy(new->title, title, lenCopy);

    // Link to DLL
    if (!start) {
        start = new;
    } else {
        end->next = new;
        new->prev = end;
    }
    end = new;
    return 0;
}

/*
Add a new description segment to an existing Prompt with given title.
*/
int addDescriptionSegment(char *title, char *description, int delay) {
    Prompt *pt = getPrompt(title);
    if (!pt) {
        puts("!!! Prompt Not Found !!!");
        return 1;
    }

    // Allocate Memory
    if (!pt->ds) {
        // Malloc
        pt->ds = (Description_Segment*)malloc(sizeof(Description_Segment));
        if (!pt->ds) {
            puts("!!! Memory Allocation Failure !!!");
            return 1;
        }
        pt->numDescriptionSegments = 1;
    } else {
        // Realloc
        pt->numDescriptionSegments += 1;
        Description_Segment *temp = NULL;
        temp = (Description_Segment*)realloc(pt->ds, sizeof(Description_Segment)*pt->numDescriptionSegments);
        if (!temp) {
            puts("!!! Memory Allocation Failure !!!");
            pt->numDescriptionSegments -= 1;
            return 1;
        }
        pt->ds = temp;
    }

    // Add description and delay
    int ii = pt->numDescriptionSegments - 1;
    pt->ds[ii].description = NULL;
    pt->ds[ii].delay = delay;

    // Description
    int lenCopy = strlen(description) + 1;
    pt->ds[ii].description = (char*)malloc(sizeof(char)*lenCopy);
    if (!pt->ds[ii].description) {
        puts("!!! Memory Allocation Failure !!!");
        return 1;
    }
    strncpy(pt->ds[ii].description, description, lenCopy);

    return 0;
}

/*
Add a new option to an existing Prompt with given title.
*/
int addOption(char *title, char *choice, char *choiceDescription, char *goToTitle) {
    Prompt *pt = getPrompt(title);
    if (!pt) {
        puts("!!! Prompt Not Found !!!");
        return 1;
    }

    // Allocate Memory
    if (!pt->options) {
        // Malloc
        pt->options = (Option*)malloc(sizeof(Option));
        if (!pt->options) {
            puts("!!! Memory Allocation Failure !!!");
            return 1;
        }
        pt->numOptions = 1;
    } else {
        // Realloc
        pt->numOptions += 1;
        Option *temp = NULL;
        temp = (Option*)realloc(pt->options, sizeof(Option)*pt->numOptions);
        if (!temp) {
            puts("!!! Memory Allocation Failure !!!");
            pt->numOptions -= 1;
            return 1;
        }
        pt->options = temp;
    }

    // Add choice, choice description, and go to title
    int ii = pt->numOptions - 1;
    pt->options[ii].choice = NULL;
    pt->options[ii].choiceDescription = NULL;
    pt->options[ii].goToTitle = NULL;

    // Choice
    int lenCopy = strlen(choice) + 1;
    pt->options[ii].choice = (char*)malloc(sizeof(char)*lenCopy);
    if (!pt->options[ii].choice) {
        puts("!!! Memory Allocation Failure !!!");
        return 1;
    }
    strncpy(pt->options[ii].choice, choice, lenCopy);

    // Choice Description
    lenCopy = strlen(choiceDescription) + 1;
    pt->options[ii].choiceDescription = (char*)malloc(sizeof(char)*lenCopy);
    if (!pt->options[ii].choiceDescription) {
        puts("!!! Memory Allocation Failure !!!");
        return 1;
    }
    strncpy(pt->options[ii].choiceDescription, choiceDescription, lenCopy);

    // GoToTitle
    lenCopy = strlen(goToTitle) + 1;
    pt->options[ii].goToTitle = (char*)malloc(sizeof(char)*lenCopy);
    if (!pt->options[ii].goToTitle) {
        puts("!!! Memory Allocation Failure !!!");
        return 1;
    }
    strncpy(pt->options[ii].goToTitle, goToTitle, lenCopy);

    return 0;
}
