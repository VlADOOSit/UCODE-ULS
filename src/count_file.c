#include "uls.h"

int count_file(char *direct) {
    DIR *dir;
    struct dirent *ent;
    int counter = 0;

    dir = opendir(direct);
    
    if (!dir) {
        return -1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_name[0] != '.' &&  mx_strcmp(ent->d_name, "uls") != 0) {
            counter++;
        }
    }
    
    closedir(dir);
    return counter;
}
