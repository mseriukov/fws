#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

#define RAW_DATA_SIZE FRAME_WIDTH * FRAME_HEIGHT
uint8_t raw_data[RAW_DATA_SIZE];

#define FRAME_BUFFER_SIZE RAW_DATA_SIZE * 2
uint8_t frame_buffer[FRAME_BUFFER_SIZE];

#define FRAME_DURATION_MS 1000 / FRAME_RATE

bool frameSending = false;
uint32_t frame_start_ms = 0;
uint32_t offset = 0;

static void update_raw_data(
    uint8_t *buffer, 
    size_t buffer_size, 
    uint32_t offset
) {
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = (offset + i) % 255;
    }
}

static void render_raw_data(
    uint8_t *raw_buffer, 
    size_t raw_buffer_size, 
    uint8_t *frame_buffer, 
    size_t frame_buffer_size
) {
    uint8_t *p = &frame_buffer[0];
    for(size_t i = 0; i < raw_buffer_size; i++) {
        *p++ = raw_buffer[i];
        *p++ = 128;
    }
}

bool usbSendFrame(
    uint8_t *frame_buffer, 
    size_t frame_buffer_size
) {
    if (frameSending) { return false; }
    if (!tud_video_n_streaming(0, 0)) { return false; }

    frameSending = true;
    tud_video_n_frame_xfer(0, 0, (void*)frame_buffer, frame_buffer_size);           
    return true;
}

int main(void) {
    board_init();
    

    while (1) {
        update_raw_data(raw_data, RAW_DATA_SIZE, offset);
        render_raw_data(raw_data, RAW_DATA_SIZE, frame_buffer, FRAME_BUFFER_SIZE);
        
        uint32_t cur_frame_ms = board_millis();  
        if (cur_frame_ms - frame_start_ms >= FRAME_DURATION_MS) {   
            if (usbSendFrame(frame_buffer, FRAME_BUFFER_SIZE)) {
                frame_start_ms += FRAME_DURATION_MS;
                offset += 1;
                if (offset > 256) { offset = 0; }
            }  
        }   
        tud_task(); // tinyusb device task
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {

}

// Invoked when device is unmounted
void tud_umount_cb(void) {

}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void)remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {

}

void tud_video_frame_xfer_complete_cb(
    uint_fast8_t ctl_idx, 
    uint_fast8_t stm_idx
) {
    (void)ctl_idx;
    (void)stm_idx;

    frameSending = false;
}

int tud_video_commit_cb(
    uint_fast8_t ctl_idx, 
    uint_fast8_t stm_idx,
    video_probe_and_commit_control_t const *parameters
) {
    (void)ctl_idx; 
    (void)stm_idx;

    return VIDEO_ERROR_NONE;
}
