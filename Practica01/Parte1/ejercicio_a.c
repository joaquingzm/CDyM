#include <stdio.h>
#include <stdint.h>

uint16_t f(uint8_t N);

int main(){
    
    uint8_t N = 10;
    uint16_t result = f(N);
    printf("Resultado %d \n",result);
    
    return 0;
}


// Es una sumatoria de i=1 hasta N, eso es: N(N+1)/2
uint16_t f(uint8_t N) {
    return (uint16_t)(N*(N+1)/2);
}




