#include <stdio.h>
#include <time.h>

#define MAX 100
double A[MAX][MAX], x[MAX], y[MAX];

int main()
{
    clock_t inicio1, inicio2, fin1, fin2;
    double tiempoTotal1, tiempoTotal2;

    inicio1 = clock();
    for(int i=0; i < MAX ; i++){
        for( int j=0; j < MAX ; j++){
            y[i] += A[i][j] * x[j];
        }
    }
    fin1 = clock();
    tiempoTotal1 = ((double) (fin1 - inicio1)) * 1000 / CLOCKS_PER_SEC;

    
    inicio2 = clock();

    for(int j=0; j < MAX ; j++){
        for( int i=0; i < MAX ; i++){
            y[i] += A[i][j] * x[j];
        }
    }

    fin2 = clock();
    tiempoTotal2 = ((double) (fin2 - inicio2)) * 1000 / CLOCKS_PER_SEC;

    printf("%f\n", tiempoTotal1);
    printf("%f\n", tiempoTotal2);
    return 0;
}