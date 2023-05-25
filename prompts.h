/*
CYOA
Elijah Delavar

TODO: Files
TODO: Description
*/

#ifndef PROMPTS
#define PROMPTS

typedef struct OPTION {
    char *choice;
    char *title;            // title associated with choice
}Option;

typedef struct PROMPT {
    char *title;
    char *description;
    // int numOptions;
    // struct OPTION *options;
    struct PROMPT *next;
    struct PROMPT *prev;
}Prompt;

int storePrompt(char*,char*);

#endif