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

Prompt *freePT(Prompt*);
void freePTDLL();
int addOption(char*,char*,char*);   // Prompt's title, choice, choice's title
int storePrompt(char*,char*);       // title, description
Prompt *getPrompt(char*);           // title

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
