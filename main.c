/*
CYOA
Elijah Delavar

TODO: Files
TODO: Compilation
TODO: Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileManager.h"

int main(int argc, char const *argv[])
{
    int err = readStory();
    if (err) {
        return err;
    }
    return 0;
}
