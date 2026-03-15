#include <stdio.h>
#include <stdint.h>

void f(uint8_t N, char *r);

int main() {

    uint8_t N = 10;

    printf("Insertar num: ");
    scanf("%hhu", &N);

    char r[4];
    f(N,r);

    return 0;
}


void f(uint8_t N, char *r) {
    
    for(int i=2;i>=0;i--) {
        r[i] = (N%10) + '0';
        N /= 10;
    }

    r[3]='\0';
}