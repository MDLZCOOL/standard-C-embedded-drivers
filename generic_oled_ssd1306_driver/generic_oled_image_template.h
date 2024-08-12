#pragma once
#include "stdint.h"
#include "string.h"

typedef struct Image {
    uint8_t w;                       // 图片宽度
    uint8_t h;                       // 图片高度
    const uint8_t *data;             // 图片数据
} Image;

extern const Image musicImg;
