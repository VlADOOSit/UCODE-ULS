#include "uls.h"

char *check_file(char *file) {
    int fl = open(file, O_RDONLY);
    if (fl == -1) {
        close(fl);
        return NULL;
    }
    
    close(fl);
    return "OK";
}
