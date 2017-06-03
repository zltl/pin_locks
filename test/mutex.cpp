#include <unistd.h>
#include <pthread.h>
#include <iostream>

pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
volatile int i = 0;

void *threadadd(void * arg) {
	for (int j = 0; j < 10; j++) {
		// sleep(1);
		pthread_mutex_lock(&mutex);
		i++;
		// std::cout << "+" << std::endl;
		pthread_mutex_unlock(&mutex);
	}
}

void *threadsub(void *arg) {
	for (int j = 0; j < 10; j++) {
		// sleep(1);
		pthread_mutex_lock(&mutex);
		// std::cout << "-" << std::endl;
		i--;
		pthread_mutex_unlock(&mutex);
	}
}

int main() {
	pthread_t ta, ts;
	pthread_create(&ta, NULL, threadadd, NULL);
	pthread_create(&ts, NULL, threadsub, NULL);
	pthread_join(ta, NULL);
	pthread_join(ts, NULL);
	std::cout << i << std::endl;
}

