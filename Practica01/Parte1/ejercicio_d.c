#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void f(uint8_t N);

int main() {

    f(8);

    return 0;
}


void f(uint8_t N) {
    for(int i=0;i<8;i++) {
        printf("%d",(N%2));
        fflush(stdout);
        sleep(1);
        N /= 2;
    }
    printf("\n");
}