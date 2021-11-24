#include "uls.h"

int size_strarr(char **strarr) {
    int size = 0;
    for (int i = 0; strarr[i]; i++) {
        size++;
    }
    return size;
}
