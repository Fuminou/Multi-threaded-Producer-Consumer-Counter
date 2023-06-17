#include <stdio.h>
#include <pthread.h>

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t myCond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t myCond2 = PTHREAD_COND_INITIALIZER;
int myCount = 0;
#define SIZE 10
void* producerThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&myMutex);
        printf("PRODUCER: myMutex locked\n");
        
        if (myCount >= SIZE) {
            pthread_mutex_unlock(&myMutex);
            printf("PRODUCER: waiting on myCond1\n");
            break;
        }
        
        while (myCount % 2 == 1) {
            pthread_cond_wait(&myCond1, &myMutex);
            printf("PRODUCER: waiting on myCond1\n");
        }

        myCount++;
        printf("myCount: %d -> %d\n", myCount - 1, myCount);

        pthread_cond_signal(&myCond2);
        printf("PRODUCER: signaling myCond2\n");
        pthread_mutex_unlock(&myMutex);
        printf("PRODUCER: myMutex unlocked\n");
    }

    pthread_exit(NULL);
}

void* consumerThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&myMutex);
        printf("CONSUMER: myMutex locked\n");
        
        if (myCount >= SIZE) {
            pthread_mutex_unlock(&myMutex);
            printf("CONSUMER: myMutex unlocked\n");
            break;
        }
        
        while (myCount % 2 == 0) {
            pthread_cond_wait(&myCond2, &myMutex);
            printf("CONSUMER: waiting on myCond2\n");
        }

        myCount++;
        printf("myCount: %d -> %d\n", myCount - 1, myCount);

        pthread_cond_signal(&myCond1);
        printf("CONSUMER: signaling myCond1\n");
        pthread_mutex_unlock(&myMutex);
        printf("CONSUMER: myMutex unlocked\n");
    }

    pthread_exit(NULL);
}

int main() {
    printf("PROGRAM START\n");

    pthread_t producerThreadId, consumerThreadId;

    pthread_create(&producerThreadId, NULL, producerThread, NULL);
    pthread_create(&consumerThreadId, NULL, consumerThread, NULL);

    pthread_join(producerThreadId, NULL);
    pthread_join(consumerThreadId, NULL);

    printf("PROGRAM END\n");
    return 0;
}