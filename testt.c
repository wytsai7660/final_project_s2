#include <stdio.h>

int main() {
    printf("hello");
    printf("\e[B");
    printf("hello");
    printf("\e[A");
    printf("hello");
}
