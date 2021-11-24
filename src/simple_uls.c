#include "uls.h"

char **list_simple_uls(char *direct) {
    DIR *dir;
    struct dirent *ent;
    int flag = 0;

    if (mx_strcmp(".", direct) == 0) {
        flag = 1;
    }
    int counter = count_file(direct);
    int n_counter = 0;
    if (counter == -1) {
        return NULL;
    }
    char **name = (char**)malloc((counter + 1) * sizeof(char*));

    dir = opendir(direct);
    

    while ((ent = readdir(dir)) != NULL) {
        if (flag == 1) {
            if (ent->d_name[0] != '.' &&  mx_strcmp(ent->d_name, "uls") != 0) {

                name[n_counter] = mx_strnew(mx_strlen(ent->d_name));
                mx_strcpy(name[n_counter], ent->d_name);
                n_counter++;
            }
        }
        if (flag == 0) {
            if (ent->d_name[0] != '.') {

                name[n_counter] = mx_strnew(mx_strlen(ent->d_name));
                mx_strcpy(name[n_counter], ent->d_name);
                n_counter++;
            }
        }
        
    }
    name[n_counter] = NULL;

    closedir(dir);

    mx_bubble_sort(name, counter);
    return name;
}
