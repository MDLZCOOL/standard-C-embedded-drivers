#pragma once
#include "stdint.h"
#include "string.h"

typedef struct ASCIIFont {
    uint8_t h;
    uint8_t w;
    uint8_t *chars;
} ASCIIFont;

extern const ASCIIFont afont8x6;
extern const ASCIIFont afont12x6;
extern const ASCIIFont afont16x8;
extern const ASCIIFont afont24x12;

typedef struct Font {
    uint8_t h;                   // 字高度
    uint8_t w;                   // 字宽度
    const uint8_t *chars;        // 字库 字库前4字节存储utf8编码 剩余字节存储字模数据
    uint8_t len;                 // 字库长度 超过256则请改为uint16_t
    const ASCIIFont *ascii;      // 缺省ASCII字体 当字库中没有对应字符且需要显示ASCII字符时使用
} Font;

extern const Font font16x16;
