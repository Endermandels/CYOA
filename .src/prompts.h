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
    char *choiceDescription;
    char *goToTitle;
}Option;

typedef struct DESCRIPTION_SEGMENT {
    char *description;
    int delay;  // millisecond delay between prints
}Description_Segment;

typedef struct PROMPT {
    char *title;
    short skipDescription;
    int numDescriptionSegments;
    struct DESCRIPTION_SEGMENT *ds;
    int numOptions;
    struct OPTION *options;
    struct PROMPT *next;
    struct PROMPT *prev;
}Prompt;

int printPT(Prompt*);
void printPTDLL();
Prompt *freePT(Prompt*);
void freePTDLL();
Prompt *getStart();
Prompt *getPrompt(char*);
int storePrompt(char*);
int addDescriptionSegment(char*,char*,int);
int addOption(char*,char*,char*,char*);

#endif