/* gcc -g -Wall -o pth_mat_vect pth_mat_vect.c -lpthread
 * ./pth_mat_vect 4
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int     thread_count;     int     m, n;
double* A;                double* x;
double* y;                double start;

void Generate_matrix(double A[], int m, int n){
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      A[i*n+j] = i*n+j;
}

void Generate_vector(double x[], int n){
  for (int i = 0; i < n; i++)
    x[i] = 1;
}

void Read_matrix(char* prompt, double A[], int m, int n) {
   int i, j;
   printf("%s\n", prompt);
   for (i = 0; i < m; i++) 
      for (j = 0; j < n; j++)
         scanf("%lf", &A[i*n+j]);
}  
void Read_vector(char* prompt, double x[], int n) {
   int i;
   printf("%s\n", prompt);
   for (i = 0; i < n; i++) 
      scanf("%lf", &x[i]);
}  
void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int local_m = m/thread_count; 
   int my_first_row = my_rank*local_m;
   int my_last_row = (my_rank+1)*local_m - 1;

   for (i = my_first_row; i <= my_last_row; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
          y[i] += A[i*n+j]*x[j];
   }
   return NULL;
}  
void Print_matrix( char* title, double A[], int m, int n) {
   int i, j;
   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%4.1f ", A[i*n + j]);
      printf("\n");
   }
}  
void Print_vector(char* title, double y[], double m) {
   int i;
   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
} 
int main(int argc, char* argv[]) {
   long thread;
   pthread_t* thread_handles;
   thread_count = atoi(argv[1]);
   thread_handles = malloc(thread_count*sizeof(pthread_t));
   m = 8;                                                     //   printf("Ingresa m y n \n");  //   scanf("%d%d", &m, &n);
   n = 8000000;

   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   
   Generate_matrix(A, m, n);                  //   Read_matrix("Ingresa la matrix", A, m, n); //   Print_matrix("leer ", A, m, n);
   Generate_vector(x, n);                     //   Read_vector("Ingrese el vector", x, n); //   Print_vector("leer", x, n);

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL, Pth_mat_vect, (void*) thread);
   start = clock();
   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
   printf("Tiempo transcurrido: %f  ", ((double)clock() - start) / CLOCKS_PER_SEC);
  //  double serial = 0.267398; //  Print_vector("El producto total es: ", y, m);
   double serial = (((double)clock() - start) / CLOCKS_PER_SEC)/thread_count; 
   double eficiencia = serial/thread_count * (((double)clock() - start) / CLOCKS_PER_SEC);
   printf("Eficiencia: %f  \n", eficiencia);
   free(A); free(x);  free(y);
   return 0;
}  
