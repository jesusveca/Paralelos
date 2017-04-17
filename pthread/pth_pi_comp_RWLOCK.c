#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_rand.h"
#include "timer.h"

const int MAX_KEY = 100000000;

struct list_node_s {
   int    data;
   struct list_node_s* next;
};

struct      list_node_s* head = NULL;  
int thread_count;             int total_ops;
double insert_percent;        double search_percent;        double delete_percent;
pthread_rwlock_t rwlock;
pthread_mutex_t count_mutex;
int member_count = 0, insert_count = 0, delete_count = 0;
void Get_input(int* inserts_in_main_p) {
   printf("Cuantas inserciones desea hacer ? \n");
   scanf("%d", inserts_in_main_p);
   printf("Cuantas operacones en total desea hacer? \n");
   scanf("%d", &total_ops);
   printf("Porcetaje para busquedas: \n");
   scanf("%lf", &search_percent);
   printf("Porcentajes de inserciones: \n");
   scanf("%lf", &insert_percent);
   delete_percent = 1.0 - (search_percent + insert_percent);
} 

int Insert(int value) {
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;
   struct list_node_s* temp;
   int rv = 1;
   
   while (curr != NULL && curr->data < value) {
      pred = curr;
      curr = curr->next;
   }

   if (curr == NULL || curr->data > value) {
      temp = malloc(sizeof(struct list_node_s));
      temp->data = value;
      temp->next = curr;
      if (pred == NULL)
         head = temp;
      else
         pred->next = temp;
   } else { /* value in list */
      rv = 0;
   }

   return rv;
}  

int  Member(int value) {
   struct list_node_s* temp;
   temp = head;
   while (temp != NULL && temp->data < value)
      temp = temp->next;
   if (temp == NULL || temp->data > value) {
      return 0;
   } else {
      return 1;
   }
}  

int Delete(int value) {
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;

   while (curr != NULL && curr->data < value) {
      pred = curr;
      curr = curr->next;
   }
   if (curr != NULL && curr->data == value) {
      if (pred == NULL) { 
         head = curr->next;
         free(curr);
      } else { 
         pred->next = curr->next;
         free(curr);
      }
      return 1;
   } else { 
      return 0;
   }
}  

void* Thread_work(void* rank) {
   long my_rank = (long) rank;
   int i, val;
   double which_op;
   unsigned seed = my_rank + 1;
   int my_member_count = 0, my_insert_count=0, my_delete_count=0;
   int ops_per_thread = total_ops/thread_count;

   for (i = 0; i < ops_per_thread; i++) {
      which_op = my_drand(&seed);
      val = my_rand(&seed) % MAX_KEY;
      if (which_op < search_percent) {
         pthread_rwlock_rdlock(&rwlock);
         Member(val);
         pthread_rwlock_unlock(&rwlock);
         my_member_count++;
      } else if (which_op < search_percent + insert_percent) {
         pthread_rwlock_wrlock(&rwlock);
         Insert(val);
         pthread_rwlock_unlock(&rwlock);
         my_insert_count++;
      } else { /* delete */
         pthread_rwlock_wrlock(&rwlock);
         Delete(val);
         pthread_rwlock_unlock(&rwlock);
         my_delete_count++;
      }
   }  

   pthread_mutex_lock(&count_mutex);
   member_count += my_member_count;
   insert_count += my_insert_count;
   delete_count += my_delete_count;
   pthread_mutex_unlock(&count_mutex);

   return NULL;
}  

int main(int argc, char* argv[]) {
   long i; 
   int key, success, attempts;
   pthread_t* thread_handles;
   int inserts_in_main;
   unsigned seed = 1;
   double start, finish;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1],NULL,10);

   Get_input(&inserts_in_main);

   i = attempts = 0;
   while ( i < inserts_in_main && attempts < 2*inserts_in_main ) {
      key = my_rand(&seed) % MAX_KEY;
      success = Insert(key);
      attempts++;
      if (success) i++;
   }
   printf("Inserto %ld nodos en lista\n", i);

   thread_handles = malloc(thread_count*sizeof(pthread_t));
   pthread_mutex_init(&count_mutex, NULL);
   pthread_rwlock_init(&rwlock, NULL);

   GET_TIME(start);
   for (i = 0; i < thread_count; i++)
      pthread_create(&thread_handles[i], NULL, Thread_work, (void*) i);

   for (i = 0; i < thread_count; i++)
      pthread_join(thread_handles[i], NULL);
   GET_TIME(finish);
   printf("Tiempo Transcurrido = %e seconds\n", finish - start);
   printf("Operaciones Totales = %d\n", total_ops);
   printf("Ops Busqueda = %d\n", member_total);
   printf("Ops Inserciones = %d\n", insert_total);
   printf("Ops Borrado= %d\n", delete_total);
   
   pthread_rwlock_destroy(&rwlock);
   pthread_mutex_destroy(&count_mutex);
   free(thread_handles);

   return 0;
}  