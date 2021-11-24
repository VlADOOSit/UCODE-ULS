#include "uls.h"

void output_basic(int argc, char **argv) {
    char **temp_sort_dir = (char**)malloc(argc * sizeof(char*));
    int temp_counter_dir = 0;

    char **temp_sort_file = (char**)malloc(argc * sizeof(char*));
    int temp_counter_file = 0;

    for (int i = 1; i < argc; i++) {
        struct stat sb;
        stat(argv[i], &sb);
        if (S_ISDIR(sb.st_mode)) {
            if (stat(argv[i], &sb) == -1) {
                mx_printerr("uls: ");
                mx_printerr(argv[i]);
                mx_printerr(": No such file or directory\n");
            }
            else {
                temp_sort_dir[temp_counter_dir] = mx_strnew(mx_strlen(argv[i]));
                mx_strcpy(temp_sort_dir[temp_counter_dir], argv[i]);
                temp_counter_dir++;
            }
            
        }
        else {
            if (check_file(argv[i]) != NULL) {
                temp_sort_file[temp_counter_file] = mx_strnew(mx_strlen(argv[i]));
                mx_strcpy(temp_sort_file[temp_counter_file], argv[i]);
                temp_counter_file++;
            }
            else {
                mx_printerr("uls: ");
                mx_printerr(argv[i]);
                mx_printerr(": No such file or directory\n");
            }
        
        }
    }
            
    mx_bubble_sort(temp_sort_dir, temp_counter_dir);
    mx_bubble_sort(temp_sort_file, temp_counter_file);

    temp_sort_dir[temp_counter_dir] = NULL;
    temp_sort_file[temp_counter_file] = NULL;
    
    

    if (size_strarr(temp_sort_file) != 0) {
        if (!(isatty(1))) {
            mx_print_strarr(temp_sort_file, "\n");
            
            if ((argc > 2 && temp_counter_file + 1 != 2) || temp_counter_dir > 0) {
                mx_printstr("\n");
            }
        }
        else {
            //mx_print_strarr(temp_sort_file, "  ");
            supp_output_base(temp_sort_file);
            if ((argc > 2 && temp_counter_file + 1 != 2) || temp_counter_dir > 0) {
                mx_printstr("\n");
            }
        }
        
    }
    mx_del_strarr(&temp_sort_file);
    for (int i = 0; i < temp_counter_dir; i++) {
        char **name = list_simple_uls(temp_sort_dir[i]);
        if (name == NULL) {
            mx_printerr("uls: ");
            mx_printerr(temp_sort_dir[i]);
            mx_printerr(": Permission denied\n");
            continue;
        }
        if (argc != 2) {
            mx_printstr(temp_sort_dir[i]);
            mx_printstr(":\n");
        }
        if (size_strarr(name) != 0) {
            if (!(isatty(1))) {
                mx_print_strarr(name, "\n");
            }
            else {
                //mx_print_strarr(name, "  ");
                supp_output_base(name);
            }
           
        }
        if (i != temp_counter_dir - 1 && argc != 2) {
            mx_printstr("\n");
        }
        mx_del_strarr(&name);
    }
    mx_del_strarr(&temp_sort_dir);   
}
