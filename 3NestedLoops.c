#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
    int i, j, k, a, b, c;
    scanf("%d %d %d", &a, &b, &c);
	int **matriz1 = malloc(sizeof *matriz1 * a);
   	int **matriz2 = malloc(sizeof *matriz2 * b);
	int **result = malloc(sizeof *result * a);
	for(i = 0; i < a; i++)
		matriz1[i] = malloc(sizeof * matriz1[i]*b);	
    for(i = 0; i < b; i++)
		matriz2[i] = malloc(sizeof * matriz2[i]*c);
	for(i = 0; i < a; i++)
		result[i] = malloc(sizeof * result[i]*c);
    for(i = 0; i < a; i++)
        for(j = 0; j < b; j++)
            matriz1[i][j] = 1;
    for(i = 0; i < b; i++)
        for(j = 0; j < c; j++)
            matriz2[i][j] = 1;
	for(i = 0; i < a; i++)
        for(j = 0; j < c; j++)
            result[i][j] = 0;
    clock_t inicio = clock();
    for(i = 0; i < a; i++)
        for(j = 0; j < c; j++)
            for(k = 0; k < b; k++)
                result[i][j] += (matriz1[i][k] * matriz2[k][j]);
    clock_t fin = clock();
    double tiempoFinal;
    tiempoFinal = (double)(fin-inicio) /CLOCKS_PER_SEC;
    printf("%f\n", tiempoFinal);
    return 0;
}