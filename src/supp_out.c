#include "uls.h"

int get_max_len(char **name) {
    int max = 0;
    int temp = 0;
    for (int i = 0; name[i]; i++) {
        temp = mx_strlen(name[i]);
        if (temp > max)
            max = temp;
    }
    if (max % 8 == 0)
        max += 8;
    else
        max = 8 - (max % 8) + max;
    return max;
}

void print_tab(int len, int max) {
    int counter = 0;
    int p;

    p = max - len;
    if (p % 8 != 0) {
        counter = (p / 8) + 1;
    }
        
    else
        counter = p / 8;
    for (int i = 0; i < counter; i++) {
        mx_printchar('\t');
    }
        
}

void print_cols(char **name, int rows, int num, int max) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; i + j < num; j += rows) {
            mx_printstr(name[i + j]);
            if (name[i + j + 1] && (i + j + rows < num))
                print_tab(mx_strlen(name[i + j]), max);
        }
        if (i != rows - 1)
            mx_printchar('\n');
    }
}

void print_name(char **name, int max, int max_win) {
    int rows;
    int cols = (max_win / max) != 0 ? max_win / max : 1;
    int num = 0;

    for(;name[num]; num++) {
    }
    if (max * cols > max_win && cols != 1)
        cols--;
    if (num * max > max_win) {
        rows = num / cols;
        if (rows == 0 || num % cols != 0)
            rows += 1;
        print_cols(name, rows, num, max);
    } 
    else {
        for (int i = 0; name[i]; i++) {
            mx_printstr(name[i]);
            if (name[i + 1]) {
                print_tab(mx_strlen(name[i]), max);
            } 
                
        }
        
    }
    mx_printchar('\n');
        
}

void supp_output_base(char **name) {
    int max;
    struct winsize win;

    if (!name) {
        return;
    }
    max = get_max_len(name);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    if (isatty(1)) {
        print_name(name, max, win.ws_col);
    }
        
    else {
        print_name(name, max, 80);
    }
        
}
