#include <FreeRTOS.h>

int main(void) {

  while (1) {
    nrf_gpio_pin_toggle(22);
    nrf_gpio_pin_toggle(23);
    nrf_gpio_pin_toggle(14);
    vTaskDelay(1000);
  }
  
}
