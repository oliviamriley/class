#include <stdio.h>

long fib(int n) {
    if(n == 0) { return 0; }
    if(n == 1) { return 1; }
    return fib(n-1) + fib(n-2);
}

void main(void) {
    long f = 0;
    int n = 0;

    printf("n: ");
    scanf("%d", &n);

    f = fib(n);

    printf("%ld\n", f);
}
