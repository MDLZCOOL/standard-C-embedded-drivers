#pragma once
#include <stddef.h>
#include <string.h>
#include "generic_err.h"
#include "stdlib.h"
#include <stdio.h>
#include "generic_driver_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OLED_COLOR_NORMAL = 0, // 正常模式 黑底白字
    OLED_COLOR_REVERSED    // 反色模式 白底黑字
} generic_oled_ssd1306_color_mode;

#define OLED_PAGE (8)
#define OLED_ROW (8 * OLED_PAGE)
#define OLED_COLUMN (128)

extern uint8_t OLED_GRAM[OLED_PAGE][OLED_COLUMN];

typedef struct {
    system_interface_t *pfsystem_interface;

    generic_err_t (*pfinit)(void *pfdev);

    generic_err_t (*pfdisplay_on)(void *pfdev);

    generic_err_t (*pfdisplay_off)(void *pfdev);

    generic_err_t (*pfnewframe)(void *pfdev);

    generic_err_t (*pfshowframe)(void *pfdev);

    generic_err_t (*pfdraw_point)(uint8_t chXpos, uint8_t chYpos,
                                  generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdraw_line)(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2,
                                 generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdraw_rectangle)(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2,
                                      generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdraw_filled_rectangle)(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2,
                                             uint8_t chYpos2,
                                             generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdraw_circle)(uint8_t chXpos, uint8_t chYpos, uint8_t chRadius,
                                   generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdraw_filled_circle)(uint8_t chXpos, uint8_t chYpos, uint8_t chRadius,
                                          generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdraw_triangle)(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2,
                                     uint8_t chXpos3,
                                     uint8_t chYpos3, generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdraw_filled_triangle)(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2,
                                            uint8_t chYpos2,
                                            uint8_t chXpos3, uint8_t chYpos3,
                                            generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdraw_eclipse)(uint8_t chXpos, uint8_t chYpos, uint8_t a, uint8_t b,
                                    generic_oled_ssd1306_color_mode color_mode);

    generic_err_t (*pfdirect_set_color_mode)(void *pfdev, generic_oled_ssd1306_color_mode color_mode);
} generic_oled_ssd1306_driver_interface_t;

generic_err_t generic_oled_ssd1306_whoami(generic_oled_ssd1306_driver_interface_t *pfdev,
                                          generic_err_t (*generic_send_func)(uint8_t *data, size_t len),
                                          generic_err_t (*generic_receive_func)(uint8_t *data, size_t len),
                                          generic_err_t (*delay_ms_func)(uint32_t nms),
                                          generic_err_t (*delay_us_func)(uint32_t nus));

generic_err_t generic_oled_ssd1306_del(generic_oled_ssd1306_driver_interface_t *pfdev);

#ifdef __cplusplus
}
#endif
