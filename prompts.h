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
    char *title;
}Option;

typedef struct PROMPT {
    char *title;
    char *description;
    int numOptions;
    struct OPTION *options;
    struct PROMPT *next;
    struct PROMPT *prev;
}Prompt;

Prompt *freePT(Prompt*);
void freePTDLL();
int storePrompt(char*,char*);
int addOption(char*,char*,char*);
Prompt *getPrompt(char*);

#endif