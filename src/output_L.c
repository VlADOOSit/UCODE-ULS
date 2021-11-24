#include "uls.h"

char first_char_per(struct stat *sb) {
    if (S_ISDIR(sb->st_mode))
        return 'd';
    if (S_ISLNK(sb->st_mode))
        return 'l';
    if (S_ISBLK(sb->st_mode))
        return 'b';
    if (S_ISCHR(sb->st_mode))
        return 'c';
    if (S_ISFIFO(sb->st_mode))
        return 'p';
    if (S_ISSOCK(sb->st_mode))
        return 's';
    if (S_ISWHT(sb->st_mode))
        return 'w';
    return '-';
}

char get_atr(char *file) {
    acl_t temp;

    if (listxattr(file, NULL, 0, XATTR_NOFOLLOW) > 0) {
        return '@';
    }
    if ((temp = acl_get_file(file, ACL_TYPE_EXTENDED))) {
        acl_free(temp);
        return ('+');
    }
    return (' ');
}

char check_chmode1(char chmod) {
    if (chmod == '-')
        return chmod = 'S';
    else
        return chmod = 's';
}

char check_chmode2(char *chmod) {
    if (chmod[9] == '-')
        return chmod[9] = 'T';
    else
        return chmod[9] = 't';
}


char *get_per(struct stat *sb, char *file) {
    char chmod[12];
    chmod[0] = first_char_per(sb);
    chmod[1] = (S_IRUSR & sb->st_mode) ? 'r' : '-';
    chmod[2] = (S_IWUSR & sb->st_mode) ? 'w' : '-';
    chmod[3] = (S_IXUSR & sb->st_mode) ? 'x' : '-';
    chmod[4] = (S_IRGRP & sb->st_mode) ? 'r' : '-';
    chmod[5] = (S_IWGRP & sb->st_mode) ? 'w' : '-';
    chmod[6] = (S_IXGRP & sb->st_mode) ? 'x' : '-';
    chmod[7] = (S_IROTH & sb->st_mode) ? 'r' : '-';
    chmod[8] = (S_IWOTH & sb->st_mode) ? 'w' : '-';
    chmod[9] = (S_IXOTH & sb->st_mode) ? 'x' : '-';
    chmod[10] = get_atr(file);
    chmod[11] = '\0';

    S_ISUID & sb->st_mode? chmod[3] = check_chmode1(chmod[3]) : 0;
    S_ISGID & sb->st_mode ? chmod[6] = check_chmode1(chmod[6]) : 0;
    01000 & sb->st_mode ? chmod[9] = check_chmode2(chmod) : 0;
   
    char *tmp = mx_strdup(chmod);
    
    return tmp;
}

int size_len(int size) {
    int counter = 0;
    int temp_size = size;
    if (size == 0) {
        counter = 1;
    }
    while (temp_size > 0) {
        temp_size /= 10;  
        counter++; 
    }
    return counter;
}

int get_total(char **name, char *dir) {
    int total = 0;
    for (int i = 0; name[i]; i++) {
        char *path = mx_strjoin(dir, name[i]);
        struct stat sb;
        stat(path, &sb);
        total += sb.st_blocks;
        mx_strdel(&path);
    }
    return total;
}

int get_max_len_size(char **name, int flag, char *dir) {
    int max = 0;
    for (int i = 0; name[i]; i++) {
        char *path = mx_strjoin(dir, name[i]);
        struct stat sb;
        stat(path, &sb);
        int temp;
        if (flag == 1) {
            temp = size_len(sb.st_size);
        }
        if (flag == 0) {
            temp = size_len(sb.st_nlink);
        }
        if (temp > max) {
            max = temp;
        }
        mx_strdel(&path);
    }
    return max;
}

void print_date(char *str) {
    char **arr = mx_strsplit(mx_strdup(str), ' ');
    mx_printstr(arr[1]);
    mx_printstr(" ");
    mx_printstr(arr[2]);
    mx_printstr("  ");
    for (int i = 0; arr[4][i] != '\n'; i++) {
        mx_printchar(arr[4][i]);
    }
    mx_del_strarr(&arr);
}

void output_L_file(char **file) {

    int max_len_s = get_max_len_size(file, 1, NULL);
    int max_len_l = get_max_len_size(file, 0, NULL);

    for (int i = 0; file[i]; i++) {
        struct stat sb;
        stat(file[i], &sb);
        mx_printstr(mx_strdup(get_per(&sb, file[i])));
        
        mx_printstr(" ");
        for (int j = 0; j < ((max_len_l + 1) - size_len(sb.st_nlink)) - 1; j++) {
            mx_printchar(' ');
        }
        mx_printint(sb.st_nlink);

        mx_printstr(" ");
        struct passwd *pw = getpwuid(sb.st_uid);
        if (pw) {
            mx_printstr(pw->pw_name);
        }
        mx_printstr("  ");
        struct group *gr = getgrgid(sb.st_gid);
        if (gr) {
            mx_printstr(gr->gr_name);
        }
        else {
            mx_printstr(mx_itoa(sb.st_gid));
        }
        mx_printchar(' ');
        for (int j = 0; j < (max_len_s + 1) - size_len(sb.st_size); j++) {
            mx_printchar(' ');
        }
        mx_printint(sb.st_size);

        mx_printstr(" ");
        print_date(ctime(&sb.st_mtime));
        mx_printstr(" ");
        mx_printstr(file[i]);
        mx_printstr("\n");
            
    }
    
}



void output_L(char* dir) {
    
    char **name;

    if (mx_strcmp(dir, "./") == 0) {
        name = list_simple_uls(".");
    }
    else {
        name = list_simple_uls(dir);
    }

    if (name == NULL) {
        dir+=2;
        mx_printerr("uls: ");
        mx_printerr(mx_strndup(dir, mx_strlen(dir) - 1));
        mx_printerr(": Permission denied\n");
        return;
    }

    int max_len_s = get_max_len_size(name, 1, dir);
    int max_len_l = get_max_len_size(name, 0, dir);

    int total = get_total(name, dir);

    if (size_strarr(name) != 0) {
        mx_printstr("total ");
        mx_printint(total);
        mx_printchar('\n');
    }
    
    char *path;
    struct stat sb;
    for (int i = 0; name[i]; i++) {
        path = mx_strjoin(dir, name[i]);
        
        stat(path, &sb);
        mx_printstr(mx_strdup(get_per(&sb, path)));

        mx_printstr(" ");
        for (int j = 0; j < ((max_len_l + 1) - size_len(sb.st_nlink)) - 1; j++) {
            mx_printchar(' ');
        }
        mx_printint(sb.st_nlink);

        mx_printstr(" ");
        struct passwd *pw = getpwuid(sb.st_uid);
        if (pw) {
            mx_printstr(pw->pw_name);
        }
        mx_printstr("  ");
        struct group *gr = getgrgid(sb.st_gid);
        if (gr) {
            mx_printstr(gr->gr_name);
        }
        else {
            mx_printstr(mx_itoa(sb.st_gid));
        }
        mx_printchar(' ');
        for (int j = 0; j < (max_len_s + 1) - size_len(sb.st_size); j++) {
            mx_printchar(' ');
        }
        mx_printint(sb.st_size);

        mx_printstr(" ");
        print_date(ctime(&sb.st_mtime));
        mx_printstr(" ");
        if (S_ISLNK(sb.st_mode)) {
            char link_buf[1024];
            ssize_t len;
            if ((len = readlink(name[i], link_buf, sizeof(link_buf)-1)) != -1) {
                char **temp = mx_strsplit(name[i], '/');
                name[i] = mx_strdup(temp[size_strarr(temp) - 1]);
                
                link_buf[len] = '\0';
                char *t_nam = mx_strjoin(name[i], " -> ");
                t_nam = mx_strjoin(t_nam, link_buf);
                mx_printstr(t_nam);
                
            }
        }
        mx_printstr(name[i]);
        mx_printstr("\n");
        
        mx_strdel(&path);
    }
}
