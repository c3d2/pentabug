#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

#include "lib/usart.h"
#include "lib/bughal.h"
#include "lib/util.h"
#include "lib/music.h"


typedef struct {
  timer_t timer;
  uint16_t interval;
  void (*callback)();
} alarm_t;
#define NEW_ALARM(name, i, cb) \
  alarm_t name = { .interval = i, .callback = cb }; \
  timer_set(&name.timer, i)

void check_alarm(alarm_t *alarm) {
  if (timer_expired(&alarm->timer)) {
    timer_set(&alarm->timer, alarm->interval);
    alarm->callback();
  }
}


#define HIST_LEN 32
static uint16_t hist[HIST_LEN];
static uint8_t hist_now = 0;

bool noise_motor = 1;
static uint8_t noise_now = 0;
static uint8_t noise_interval = 0;
static uint16_t volume_threshold = 1;

static void do_sample() {
  USART0_puts("Sampled ");
  USART0_put_uint16(hist[hist_now]);
  USART0_puts(" threshold=");
  USART0_put_uint16(volume_threshold);
  USART0_crlf();

  /* Search maxs */
  uint8_t max1 = 0, max2 = 0;
  for(uint8_t i = 0; i < HIST_LEN; i++) {
    if (hist[i] >= hist[max1]) {
      max2 = max1;
      max1 = i;
    } else if (hist[i] >= hist[max2]) {
      max2 = i;
    }
  }
  /* Found new rhythm? */
  if (max1 != max2 &&
      hist[max2] >= volume_threshold &&
      hist[max2] >= hist[max1] / 2) {
    USART0_puts("max1=");
    USART0_put_uint16(max1);
    USART0_putc(':');
    USART0_put_uint16(hist[max1]);
    USART0_puts(" max2=");
    USART0_put_uint16(max2);
    USART0_putc(':');
    USART0_put_uint16(hist[max2]);
    USART0_crlf();
    uint16_t new_threshold = hist[max1] / 2;

    /* hist[] is a ring buffer, fix ranges: */
    if (max1 <= hist_now)
      max1 += HIST_LEN;
    if (max2 <= hist_now)
      max2 += HIST_LEN;
    uint8_t new_interval = max1 > max2 ?
      max1 - max2 :
      max2 - max1;

    if (new_interval > 1 &&
        new_interval != noise_interval) {
      volume_threshold = new_threshold;
      noise_interval = new_interval;
      noise_now = 0;

      USART0_puts("New interval: ");
      USART0_put_uint16(noise_interval);
      USART0_crlf();
    }
  }

  /* Generate noise */
  led_off(LED_L);
  set_motor(MOTOR_OFF);
  if (noise_interval > 1) {
    noise_now++;
    if (noise_now >= noise_interval) {
      noise_now = 0;
      led_on(LED_L);
      if (noise_motor)
        set_motor(MOTOR_ON);
    }
  }

  /* Progress in histogram */
  led_off(LED_R);
  hist_now++;
  if (hist_now >= HIST_LEN) {
    hist_now = 0;
    /* Visually indicate ring buffer length */
    led_on(LED_R);
  }

  /* Clear current volume before recording in do_record */
  hist[hist_now] = 0;
}

static void do_record() {
    uint16_t volume = ADCW;

    if (volume > hist[hist_now])
      hist[hist_now] = volume;
}



void __attribute__((noreturn)) 
main(void)
{
  /* hardware initialisation: */
  init_leds();
  init_buzzr();
  init_switch();
  USART0_Init();
  init_motor();
  /* software initialisation */
  timer_init();
  music_init();
  init_mic();
	
  /* here the show begins:*/
  sei();
    
  NEW_ALARM(sample_alarm, 4, &do_sample);

  for(;;) /* ever */  {
    //main polling loop;
    /*button_poll();
    if (btn_state(BTNST_SUP, BTN_LEFT)) {
      button_clear(BTN_LEFT);
      noise_interval = 0;
    }
    if (btn_state(BTNST_SUP, BTN_RIGHT)) {
      button_clear(BTN_RIGHT);
      noise_motor = !noise_motor;
      USART0_putc('m');
      USART0_put_uint8(noise_motor);
      USART0_crlf();
    }*/

    check_alarm(&sample_alarm);
    do_record();
  }
}



