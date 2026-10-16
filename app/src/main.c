/**
 * @file main.c
 */



#include <zephyr/kernel.h>
#include <stdint.h>
#include "LED.h"
#include "BTN.h"

#define SLEEP_MS 100


int main(void) {
  int count = 0;
  if(LED_init() != 0){
    return 0;
  }
  if(BTN_init() != 0) {

    return 0;
  }

  while (1) {
    if (BTN_check_clear_pressed(BTN0)) {
      if (count < 16){
        count++;
      }
      else{
        count = 0;
      }
    }
    LED_set(LED0, count & 0x0001);
    LED_set(LED1, count & 0x0002);
    LED_set(LED2, count & 0x0004);
    LED_set(LED3, count & 0x0008);

    k_msleep(SLEEP_MS);
    

  }
  return 0;
}
