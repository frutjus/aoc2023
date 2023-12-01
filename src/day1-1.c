#include <stdio.h>
#include <ctype.h>

int main() {
    FILE *inp = fopen("inp/day1.txt", "r");
    if (!inp) {
        printf("Error; could not open file: %s\n", inp);
        return 1;
    }

    char c;
    int first = -1, last = -1, ans = 0;
    char *cl = "[0m";

    while (fread(&c, 1, 1, inp) != 0) {

        if (isdigit(c)) {
            if (first == -1) {
                first = (int)c - 48;
            }
            last = (int)c - 48;
            cl = "[32m";
        } else {
            cl = "[0m";
            if (c == '\n') {
                int value = first * 10 + last;
                ans += value;
                printf(" \tvalue = %d", value);
                first = -1;
            }
        }
        
        printf("%s%c[0m", cl, c);
    }

    printf("Answer = %d\n", ans);

    return 0;
}