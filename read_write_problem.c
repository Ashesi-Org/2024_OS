#include <pthread.h> // where the mutex is coming from
#include <stdio.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 5

//some helper functions
void create_threads(pthread_t threads[], int num_threads, void *(*start_routine)(void *));
void join_threads(pthread_t threads[], int num_threads);

//shared resource
int sharedData = 0;

//initialize mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *writer(void *arg) {

	//acquire
	pthread_mutex_lock(&mutex);

	//critical section
	sharedData++;
	printf("Writer writes: %d\n", sharedData);
	sleep(1);
	//end critical section

	//release
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

// simple reader
void *reader(void *arg) {

	pthread_mutex_lock(&mutex);
	printf("Reader reads: %d\n", sharedData);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main() {
	
	// creating threads and joining them
    	pthread_t readers[NUM_READERS], writers[NUM_WRITERS];

	create_threads(writers, NUM_WRITERS, writer);
    	create_threads(readers, NUM_READERS, reader);

	join_threads(writers, NUM_WRITERS);
    	join_threads(readers, NUM_READERS);

    	//free resources
	pthread_mutex_destroy(&mutex);

    	return 0;
}

void create_threads(pthread_t threads[], int num_threads, void *(*start_routine)(void *)) {
    	for (int i = 0; i < num_threads; ++i) {
        	pthread_create(&threads[i], NULL, start_routine, NULL);
    	}
}

void join_threads(pthread_t threads[], int num_threads) {
    	for (int i = 0; i < num_threads; ++i) {
        	pthread_join(threads[i], NULL);
    	}
}

