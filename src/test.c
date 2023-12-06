#include <stdio.h>
#include <math.h>

#include "common.h"
#include "array.h"

int main() {
    Array(int) nums = Array_new(int);

    for (int i = 0; i < 16; i++) {
        Array_push(nums, pow(2, i));
    }

    printf("Size = %llu; Capacity = %llu;\n", nums.size, nums.capacity);
    for (int i = 0; i < nums.size; i++) {
        printf("%d: %d\n", i, nums.at[i]);
    }

    return 0;
}
