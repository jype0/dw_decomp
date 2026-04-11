#include <libcd.h>
#include <string.h>
#include "dw/efe_table.h"

#include "common.h"

int8_t* EFEX_PATH_ARRAY[] = {
    "\\ETCDAT\\FEEL_EF.TMD",
    "\\ETCDAT\\EX.TIM",
    "\\ETCDAT\\EX.TMD"
};

// TODO: define once 4 unreferenced bytes after array are understood
// it appears as though only the first 3 entries of this array are used,
// effectively wasting 1KB of RAM
extern EFEX_FileLookup EFEX_TABLE[MAX_EFEX_FILES];

// is called from main() once and never again
// the populated table is never read from
void fillEFEXTable(void)
{
    int32_t i;
    EFEX_FileLookup *itr;
    int8_t path[64];
    CdlFILE searchResult;

    itr = EFEX_TABLE;

    while (CdReadSync(1, 0));

    for (i = 0; i < NUM_EFEX_FILES; i++) {
        path[0] = '\\';
        strcpy(&path[1], EFEX_PATH_ARRAY[i]);
        strcat(path, ";1");
        while ((int32_t)CdSearchFile(&searchResult,path) == -1);
        itr->pos = searchResult.pos;
        itr->size = searchResult.size;
        itr++;
    }

    // strange useless loop that writes to the same entry 
    for (; i < MAX_EFEX_FILES; i++) {
        itr->size = 0;
    }

  return;
}

