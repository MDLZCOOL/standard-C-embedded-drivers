#pragma once
#include <stdint.h>
#include <stddef.h>
#include "struct_typedef.h"

typedef struct {
    generic_err_t (*send)(uint8_t *data, size_t len);

    generic_err_t (*receive)(uint8_t *data, size_t len);

    generic_err_t (*delay_ms)(uint32_t nms);

    generic_err_t (*delay_us)(uint32_t nus);
} system_interface_t;
