#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <time.h>

#include "scheduler.h"

pthread_mutex_t event_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t event_cond = PTHREAD_COND_INITIALIZER;
int __loop_event = 0;  // event

static short __scheduler_running = 1;
static pthread_t __scheduler_thread = 1;


int scheduler_init(int interval_ms) {
    pthread_t thread;

    SchedulerSettings *settings;

    if( (settings = malloc( sizeof (SchedulerSettings) )) == NULL )
		return 1;

    settings->interval_ms = interval_ms;

    // Create the thread
    if (pthread_create(&thread, NULL, scheduler_taskloop, settings) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    __scheduler_thread = thread;

    return 0;
}

void scheduler_softexit() {
    __scheduler_running = 0;

    pthread_join(__scheduler_thread, NULL);
    printf("Scheduler exited softly\n");
}

void scheduler_hardexit() {

    // interup nanosleep
}

void scheduler_destroy(void* arg) {

    // TODO: can be called twice?
    // SchedulerSettings *settings = (SchedulerSettings *)arg;
    free( arg );

    printf("Cleanup handler called. Cleaning up resources...\n");
}

void* scheduler_taskloop(void* arg) {

    struct timespec corr_interval;
    struct timespec early_exit_time;

    // desired (target) interval time
    SchedulerSettings *settings = (SchedulerSettings *)arg;
    int interv_ms = settings->interval_ms;

    pthread_cleanup_push(scheduler_destroy, arg);

    // Set the thread to be cancellable at a cancellation point
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);


    if(interv_ms < 10 && interv_ms > 2000) {
        perror("Illegal interval time");
        pthread_exit((void*)-1);
    }

    struct timespec ctime;
    corr_interval.tv_sec = 0;             // 0 seconds
    corr_interval.tv_nsec = (long)(settings->interval_ms) * 1000000L;

    printf("POINT0");
    while( __scheduler_running ) {

        // Lock the mutex
        pthread_mutex_lock(&event_mutex);

        printf("POINT1");
        // Signal the condition
        __loop_event = 1;
        pthread_cond_signal(&event_cond);
        printf("POINT2");
        // Unlock the mutex
        pthread_mutex_unlock(&event_mutex);

        // schedule next events

        // Sleep for the specified interval
        if( nanosleep(&corr_interval, &early_exit_time) == -1 ) {
            perror("nanosleep");
            pthread_exit((void*)1);
        }
    }

    pthread_cleanup_pop(0);

    // function 'scheduler_destroy' can be called while hanging on 'nanosleep(...)',
    // but if it has not happen call it now to free resources
    scheduler_destroy(arg);

    return NULL;
}
