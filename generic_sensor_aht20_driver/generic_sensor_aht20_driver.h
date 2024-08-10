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

typedef struct {
    float temperature;
    float humidity;

    system_interface_t *pfsystem_interface;

    generic_err_t (*pfinit)(void *pfdev);

    generic_err_t (*pfmeasure)(void *pfdev);
} generic_sensor_aht20_driver_interface_t;

generic_err_t generic_sensor_aht20_whoami(generic_sensor_aht20_driver_interface_t *pfdev,
                                          generic_err_t (*generic_send_func)(uint8_t *data, size_t len),
                                          generic_err_t (*generic_receive_func)(uint8_t *data, size_t len),
                                          generic_err_t (*delay_ms_func)(uint32_t nms),
                                          generic_err_t (*delay_us_func)(uint32_t nus));

generic_err_t generic_sensor_aht20_del(generic_sensor_aht20_driver_interface_t *pfdev);

#ifdef __cplusplus
}
#endif
