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
#include "prompts.h"

Prompt *start;
Prompt *end;

void printPTDLL();
Prompt *freePT(Prompt*);
void freePTDLL();
Prompt *getStart();
Prompt *getPrompt(char*);
int storePrompt(char*,char*);
int addOption(char*,char*,char*);

/*
Print the prompt for the user to answer.
*/
int printPT(Prompt *pt) {
    if (!pt) {
        puts("!!! Null Prompt !!!");
        return 1;
    }
    puts(pt->description);
    for (int ii = 0; ii < pt->numOptions; ii++) {
        printf("%s.  %s\n", pt->options[ii].choice, pt->options[ii].goToTitle);
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
        puts(cur->description);
        for (int ii = 0; ii < cur->numOptions; ii++) {
            puts(cur->options[ii].choice);
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
    if (pt->description) {
        free(pt->description);
    }
    if (pt->options) {
        for (int ii = 0; ii < pt->numOptions; ii++) {
            if (pt->options[ii].choice) {
                free(pt->options[ii].choice);
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
int storePrompt(char *title, char *description) {
    Prompt *new = NULL;
    new = (Prompt*)malloc(sizeof(Prompt));
    if (!new) {
        puts("!!! Memory Allocation Failure !!!");
        return 1;
    }

    new->title = NULL;
    new->description = NULL;
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

    // Description
    lenCopy = strlen(description) + 1;
    new->description = (char*)malloc(sizeof(char)*lenCopy);
    if (!new->description) {
        puts("!!! Memory Allocation Failure !!!");
        freePT(new);
        return 1;
    }
    strncpy(new->description, description, lenCopy);

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
Add a new option to an existing Prompt with given title.
*/
int addOption(char *title, char *choice, char *goToTitle) {
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

    // Add choice and goToTitle
    int ii = pt->numOptions - 1;
    pt->options[ii].choice = NULL;
    pt->options[ii].goToTitle = NULL;

    // Choice
    int lenCopy = strlen(choice) + 1;
    pt->options[ii].choice = (char*)malloc(sizeof(char)*lenCopy);
    if (!pt->options[ii].choice) {
        puts("!!! Memory Allocation Failure !!!");
        return 1;
    }
    strncpy(pt->options[ii].choice, choice, lenCopy);

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
