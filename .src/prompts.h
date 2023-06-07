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
    short todelete;   // determines whether to delete this description after printing it
}Description_Segment;

typedef struct PROMPT {
    char *title;
    short skipDescription;
    int numDescriptionSegments;
    struct DESCRIPTION_SEGMENT *ds;
    short freeform;
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
int addDescriptionSegment(char*,char*,int,short);
int addOption(char*,char*,char*,char*,short);
int formify(char*);

#endif