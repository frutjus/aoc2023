#include <stdio.h>
#include <ctype.h>

#include "common.h"

int main() {
    String inp = read_file("inp/day1.txt");

    char buffer[100];
    char c;
    int first = -1, last = -1, ans = 0;
    char *cl = "[0m";

    while (fread(&c, 1, 1, inp) != 0) {
        

        
        
    }

    printf("Answer = %d\n", ans);

    return 0;
}