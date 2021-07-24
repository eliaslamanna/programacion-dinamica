#include <stdio.h>
unsigned long long maximo ( unsigned long long i, unsigned long long j){
         return (i > j) ? i : j;
}

int main (unsigned int argc, char** argv){
    if (argc != 2) return 1;
    
    unsigned int n = atoi(argv[1]);
    unsigned long long i,j;
    unsigned long long producto_maximo[n+1];
    producto_maximo[0] = 0;
    producto_maximo[1] = 0; //para que den los primeros casos 
    producto_maximo[2] = 1;
    for (i = 3; i <= n; i++){
        producto_maximo[i] = 0;
            for (j = 2; j < i; j++){
                producto_maximo[i] = maximo (maximo (j, producto_maximo[j]) * maximo (i-j, producto_maximo[i-j]), producto_maximo[i]);
            }
    }
    printf("El producto maximo es %llu\n", producto_maximo[n]);
    return 0;
}
