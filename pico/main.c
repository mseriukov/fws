#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/cyw43_arch.h"

static void cdc_task(void);

int main(void) {
    board_init();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    tud_init(BOARD_TUD_RHPORT);

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    while (1) {
        tud_task(); // tinyusb device task
        cdc_task();
    }
}

static void echo_serial_port(uint8_t itf, uint8_t buf[], uint32_t count) {
    for(uint32_t i=0; i<count; i++) {
        tud_cdc_n_write_char(itf, buf[i]);
    }
    tud_cdc_n_write_flush(itf);
}

static void cdc_task(void) {
    static uint8_t buf[256];
    sprintf(buf, "\33[2K\r%d", board_millis());
    echo_serial_port(0, buf, sizeof(buf) / sizeof(buf[0]));
}