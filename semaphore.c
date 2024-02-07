#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

// Global semaphore variable
sem_t semaphore;

// Function executed by each thread
void* thread_function(void* thread_id) {
    int id = *(int*)thread_id;
    
    // Wait on semaphore
    sem_wait(&semaphore);
    
    printf("Thread %d is in critical section\n", id);
    sleep(1);
    // Signal semaphore
    sem_post(&semaphore);
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Initialize semaphore with initial value 1
    sem_init(&semaphore, 0, 1); // binary semaphore
    // sem_init(&semaphore, 0, 3); // counting semaphore
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }
    
    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    // Destroy semaphore
    sem_destroy(&semaphore);
    
    return 0;
}