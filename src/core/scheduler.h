

#ifndef _SIGMA_200MS_NET__CORE_SCHEDULER_H_
#define _SIGMA_200MS_NET__CORE_SCHEDULER_H_


typedef struct garden_scheduler_settings {
    int interval_ms;

} SchedulerSettings;

int scheduler_init(int interval_ms);
void scheduler_destroy(void* arg);

void scheduler_softexit();
void scheduler_hardexit();

void* scheduler_taskloop(void* arg);

#endif // _SIGMA_200MS_NET__CORE_SCHEDULER_H_
