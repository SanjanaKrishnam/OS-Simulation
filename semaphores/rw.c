#include<stdio.h>
#include<string.h>

#include<pthread.h>

#include<semaphore.h>
#include<unistd.h>

#define N 5

int data = 0, readerCount = 0;
sem_t db, mutex;


void initSemaphores() {
    
    
    sem_init(&mutex, 0, 1);

    sem_init(&db, 0, 1);
} 




void *writer(void *arg) {
    

    sem_wait(&db);
    
    printf("Data writen by the writer(%d) is %d\n",  ((int) arg), ++data);
    sleep(1);
    
    sem_post(&db);    
}




void *reader(void *arg) {
    

    sem_wait(&mutex);

    readerCount++;
   
    if (readerCount == 1) {
      
        sem_wait(&db);    
    }
    

    sem_post(&mutex);

    printf("Data read by the reader(%d) is %d\n",  ((int) arg), data);
    sleep(1);
    

    sem_wait(&mutex);
    

    readerCount--;

    if (readerCount == 0) {

        sem_post(&db);    
    }

    sem_post(&mutex);

}
      
      
      
        
int main() {
            
    initSemaphores();
    

    pthread_t read[N], write[N];
    

    int i;
    for (i = 0; i < N; i++) {
        pthread_create(&write[i], NULL, writer, (void *)i);
        pthread_create(&read[i], NULL, reader, (void *)i);
    }
    

    for (i = 0; i < N; i++) {
        pthread_join(write[i], NULL);
        pthread_join(read[i], NULL);
    }
    
    return 0;
}