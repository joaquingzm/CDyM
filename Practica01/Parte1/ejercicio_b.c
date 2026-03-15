#include <stdio.h>
#include <stdint.h>

#define N 9

void f(int a[], int size);

int main() {

    int size = N;
    int a[N];

    printf("array: ");
    for (int i=0;i<size;i++) {
        a[i] = i;
        printf("%d , ",a[i]);
    }
    printf("\n");

    f(a,size);

    printf("array: ");
    for (int i=0;i<size;i++) {
        printf("%d , ",a[i]);
    }
    printf("\n");
    return 0;
}

void f(int a[], int size) {
    int temp;
    for (int i=0;i<size/2;i++) {
        temp = a[i];
        a[i] = a[size-1-i];
        a[size-1-i] = temp;

    }
}

