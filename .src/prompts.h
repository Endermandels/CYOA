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
    char *goToTitle;
}Option;

typedef struct PROMPT {
    char *title;
    char *description;
    int numOptions;
    struct OPTION *options;
    struct PROMPT *next;
    struct PROMPT *prev;
}Prompt;

int printPT(Prompt*);
void printPTDLL();
Prompt *freePT(Prompt*);
void freePTDLL();
int storePrompt(char*,char*);
int addOption(char*,char*,char*);
Prompt *getPrompt(char*);
Prompt *getStart();

#endif