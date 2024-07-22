#ifndef _SIGMA_200MS_NET__DRIVERS_BUZZER_H_
#define _SIGMA_200MS_NET__DRIVERS_BUZZER_H_

#define BUZZER_PIN 16

static void *_buzzer_do_buzz(void *arg);

void buzzer_play();
void buzzer_stop();

#endif // _GARDEN_200MS_NET__DRIVERS_BUZZER_H_
