#ifndef ULS_H
#define ULS_H

#include "libmx.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include <sys/ioctl.h>
#include <string.h>

//typdef struct s_per {
    struct stat sb;
    char *per;
    char *nlink;
    char *usr;
    char *group;
    char *size; 
    char *name;
//} t_per;

int count_file(char *direct);
char **list_simple_uls(char *direc);
int size_strarr(char **strarr);
void output_basic(int argc, char **argv);
char *check_file(char *file);
void output_L(char* dir);
void output_L_file(char **file);
void supp_output_base(char **names);

#endif 
