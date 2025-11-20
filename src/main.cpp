#include <stdio.h>
#include <unistd.h>
#include <lvgl/lv_conf.h>
#include <lvgl/lvgl.h>
#include <sys/time.h>
#include "ui/ui.h"
#include "ui/vars.h"

#define TARGET_FPS 60
#define LOOP_DELAY_US (1000000 / TARGET_FPS)

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t *hal_init(int32_t w, int32_t h)
{
    lv_group_set_default(lv_group_create());

    lv_display_t *disp = lv_sdl_window_create(w, h);

    lv_indev_t *mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);
    lv_display_set_default(disp);

    lv_indev_t *mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_display(mousewheel, disp);
    lv_indev_set_group(mousewheel, lv_group_get_default());

    lv_indev_t *kb = lv_sdl_keyboard_create();
    lv_indev_set_display(kb, disp);
    lv_indev_set_group(kb, lv_group_get_default());

    return disp;
}

int main(int args, char **argv)
{
    lv_init();
    lv_display_t *display = hal_init(800, 480);

    ui_init();

    struct timeval start;
    gettimeofday(&start, NULL);
    while (1) {
        lv_task_handler();
        ui_tick();

        // calculate loop time
        struct timeval now;
        gettimeofday(&now, NULL);
        int diff = (now.tv_sec - start.tv_sec) * 1000000 + now.tv_usec - start.tv_usec;
        start = now;

        if (diff >= LOOP_DELAY_US) {
            usleep(LOOP_DELAY_US);
        } else {
            usleep(LOOP_DELAY_US - diff);
        }
    }

    return 0;
}
