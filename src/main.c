#include "uls.h"

int main(int argc, char *argv[]) {
    
    if (argc == 1) {
        if (!(isatty(1))) {
            char **name = list_simple_uls(".");
            mx_print_strarr(name, "\n");
            mx_del_strarr(&name);
        }
        else {
            char **name = list_simple_uls(".");
            supp_output_base(name);
            mx_del_strarr(&name);
        }
    }
    if (argc > 1) {
        int flag_l = 0;
        int flag_basic = 1;
        if (argv[1][0] == '-') {
                if (argv[1][1] == 'l') {
                    flag_l = 1;
                    flag_basic = 0;
                }
                else {
                    mx_printerr("uls: illegal option -- ");
                    mx_printerr(&argv[1][1]);
                    mx_printchar('\n');
                    mx_printerr("usage: uls [-l] [file ...]\n");
                    exit(1);
                }
            }
        
        if (flag_basic == 1) {
            
            output_basic(argc, argv);
        }
        if (flag_l == 1) {
            if (argc == 2) {
               output_L("./"); 
            }
            else {
                char **temp_sort_dir = (char**)malloc(argc * sizeof(char*));
                int temp_counter_dir = 0;

                char **temp_sort_file = (char**)malloc(argc * sizeof(char*));
                int temp_counter_file = 0;

                for (int i = 2; i < argc; i++) {
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

                output_L_file(temp_sort_file);
                if (temp_counter_file != 0 && temp_counter_dir != 0) {
                    mx_printchar('\n');
                }
                for (int i = 0; i < temp_counter_dir; i++) {
                    char *path = mx_strjoin(mx_strjoin("./", temp_sort_dir[i]), "/");
                    if (argc > 3 && list_simple_uls(path) != NULL) {
                        mx_printstr(temp_sort_dir[i]);
                        mx_printstr(":\n");
                    }
                    
                    output_L(path);

                    if (i != temp_counter_dir - 1) {
                        mx_printchar('\n');
                    }
                    //mx_strdel(&path);
                }
                //mx_del_strarr(&temp_sort_dir);
                //mx_del_strarr(&temp_sort_file);
            }            
        }
        
    }
    return 0;
}
