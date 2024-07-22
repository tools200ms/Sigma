
#include <stdio.h>
#include <pthread.h>

#include <wiringPi.h>

#include "buzzer.h"

// static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_destroy(&mutex);

static int buzzing = 0;


static void *_buzzer_do_buzz(void *arg) {
    int top = 1000, iteration = 0, direction = 1;

    while(buzzing) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(0.002 * (iteration));
        digitalWrite(BUZZER_PIN, LOW);
        delay(0.002 * (iteration));

        iteration += direction;

        if( iteration == top ) direction = -1;
        if( iteration == 1 ) direction = 1;
    }

    return NULL;
}


void buzzer_play() {
    pthread_t thread;
    int err;

    if(buzzing == 0) {
        buzzing = 1;
        err = pthread_create(&thread, NULL, _buzzer_do_buzz, NULL);

        if( err ) {
            printf("An error occurred: %d", err);
            return;
        }
    }

    printf("buzzing");
}

void buzzer_stop() {
    buzzing = 0;
}


