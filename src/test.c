#include <stdio.h>
#include <math.h>

#include "common.h"
#include "array.h"

int main() {
    struct {int a;} A = {21};

    struct {int a;} B = {42};

    A = (typeof(A))B;

    printf("%d\n", A.a);

    return 0;
}
