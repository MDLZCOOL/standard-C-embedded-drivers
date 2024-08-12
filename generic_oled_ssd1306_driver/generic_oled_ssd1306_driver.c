#include "generic_oled_ssd1306_driver.h"

uint8_t OLED_GRAM[OLED_PAGE][OLED_COLUMN];

generic_err_t generic_oled_ssd1306_send_cmd(generic_oled_ssd1306_driver_interface_t *pfdev, uint8_t cmd) {
    uint8_t sendBuffer[2] = {0};
    sendBuffer[1] = cmd;
    pfdev->pfsystem_interface->send(sendBuffer, 2);
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_newframe(generic_oled_ssd1306_driver_interface_t *pfdev) {
    memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_showframe(generic_oled_ssd1306_driver_interface_t *pfdev) {
    uint8_t sendBuffer[OLED_COLUMN + 1];
    sendBuffer[0] = 0x40;
    for (uint8_t i = 0; i < OLED_PAGE; i++) {
        generic_oled_ssd1306_send_cmd(pfdev, 0xB0 + i);
        generic_oled_ssd1306_send_cmd(pfdev, 0x02);
        generic_oled_ssd1306_send_cmd(pfdev, 0x10);
        memcpy(sendBuffer + 1, OLED_GRAM[i], OLED_COLUMN);
        pfdev->pfsystem_interface->send(sendBuffer, OLED_COLUMN + 1);
    }
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_init(generic_oled_ssd1306_driver_interface_t *pfdev) {
    generic_oled_ssd1306_send_cmd(pfdev, 0xAE);
    generic_oled_ssd1306_send_cmd(pfdev, 0x20);
    generic_oled_ssd1306_send_cmd(pfdev, 0x10);
    generic_oled_ssd1306_send_cmd(pfdev, 0xB0);
    generic_oled_ssd1306_send_cmd(pfdev, 0xC8);
    generic_oled_ssd1306_send_cmd(pfdev, 0x00);
    generic_oled_ssd1306_send_cmd(pfdev, 0x10);
    generic_oled_ssd1306_send_cmd(pfdev, 0x40);
    generic_oled_ssd1306_send_cmd(pfdev, 0x81);
    generic_oled_ssd1306_send_cmd(pfdev, 0xDF);
    generic_oled_ssd1306_send_cmd(pfdev, 0xA1);
    generic_oled_ssd1306_send_cmd(pfdev, 0xA6);
    generic_oled_ssd1306_send_cmd(pfdev, 0xA8);
    generic_oled_ssd1306_send_cmd(pfdev, 0x3F);
    generic_oled_ssd1306_send_cmd(pfdev, 0xA4);
    generic_oled_ssd1306_send_cmd(pfdev, 0xD3);
    generic_oled_ssd1306_send_cmd(pfdev, 0x00);
    generic_oled_ssd1306_send_cmd(pfdev, 0xD5);
    generic_oled_ssd1306_send_cmd(pfdev, 0xF0);
    generic_oled_ssd1306_send_cmd(pfdev, 0xD9);
    generic_oled_ssd1306_send_cmd(pfdev, 0x22);
    generic_oled_ssd1306_send_cmd(pfdev, 0xDA);
    generic_oled_ssd1306_send_cmd(pfdev, 0x12);
    generic_oled_ssd1306_send_cmd(pfdev, 0xDB);
    generic_oled_ssd1306_send_cmd(pfdev, 0x20);
    generic_oled_ssd1306_send_cmd(pfdev, 0x8D);
    generic_oled_ssd1306_send_cmd(pfdev, 0x14);
    generic_oled_ssd1306_newframe(pfdev);
    generic_oled_ssd1306_showframe(pfdev);
    generic_oled_ssd1306_send_cmd(pfdev, 0xAF);
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_display_on(generic_oled_ssd1306_driver_interface_t *pfdev) {
    generic_oled_ssd1306_send_cmd(pfdev, 0x8D);
    generic_oled_ssd1306_send_cmd(pfdev, 0x14);
    generic_oled_ssd1306_send_cmd(pfdev, 0xAF);
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_display_off(generic_oled_ssd1306_driver_interface_t *pfdev) {
    generic_oled_ssd1306_send_cmd(pfdev, 0x8D);
    generic_oled_ssd1306_send_cmd(pfdev, 0x10);
    generic_oled_ssd1306_send_cmd(pfdev, 0xAE);
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_direct_set_color_mode(generic_oled_ssd1306_driver_interface_t *pfdev,
                                                         generic_oled_ssd1306_color_mode color_mode) {
    if (color_mode == OLED_COLOR_NORMAL) {
        generic_oled_ssd1306_send_cmd(pfdev, 0xA6);
    }
    if (color_mode == OLED_COLOR_REVERSED) {
        generic_oled_ssd1306_send_cmd(pfdev, 0xA7);
    }
    return GENERIC_OK;
}

generic_err_t
generic_oled_ssd1306_draw_point(uint8_t chXpos, uint8_t chYpos, generic_oled_ssd1306_color_mode color_mode) {
    if (chXpos > OLED_COLUMN || chYpos > OLED_ROW) {
        return GENERIC_FAIL;
    }
    if (!color_mode) {
        OLED_GRAM[chYpos / 8][chXpos] |= 1 << (chYpos % 8);
    } else {
        OLED_GRAM[chYpos / 8][chXpos] &= ~(1 << (chYpos % 8));
    }
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_draw_line(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2,
                                             generic_oled_ssd1306_color_mode color_mode) {
    uint8_t temp = 0;
    if (chXpos1 == chXpos2) {
        if (chYpos1 > chYpos2) {
            temp = chYpos1;
            chYpos1 = chYpos2;
            chYpos2 = temp;
        }
        for (uint8_t y = chYpos1; y <= chYpos2; y++) {
            generic_oled_ssd1306_draw_point(chXpos1, y, color_mode);
        }
    } else if (chYpos1 == chYpos2) {
        if (chXpos1 > chXpos2) {
            temp = chXpos1;
            chXpos1 = chXpos2;
            chXpos2 = temp;
        }
        for (uint8_t x = chXpos1; x <= chXpos2; x++) {
            generic_oled_ssd1306_draw_point(x, chYpos1, color_mode);
        }
    } else {
        // Bresenham直线算法
        int16_t dx = chXpos2 - chXpos1;
        int16_t dy = chYpos2 - chYpos1;
        int16_t ux = ((dx > 0) << 1) - 1;
        int16_t uy = ((dy > 0) << 1) - 1;
        int16_t x = chXpos1, y = chYpos1, eps = 0;
        dx = abs(dx);
        dy = abs(dy);
        if (dx > dy) {
            for (x = chXpos1; x != chXpos2; x += ux) {
                generic_oled_ssd1306_draw_point(x, y, color_mode);
                eps += dy;
                if ((eps << 1) >= dx) {
                    y += uy;
                    eps -= dx;
                }
            }
        } else {
            for (y = chYpos1; y != chYpos2; y += uy) {
                generic_oled_ssd1306_draw_point(x, y, color_mode);
                eps += dx;
                if ((eps << 1) >= dy) {
                    x += ux;
                    eps -= dy;
                }
            }
        }
    }
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_draw_rectangle(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2,
                                                  generic_oled_ssd1306_color_mode color_mode) {
    generic_oled_ssd1306_draw_line(chXpos1, chYpos1, chXpos2, chYpos1, color_mode);
    generic_oled_ssd1306_draw_line(chXpos2, chYpos1, chXpos2, chYpos2, color_mode);
    generic_oled_ssd1306_draw_line(chXpos2, chYpos2, chXpos1, chYpos2, color_mode);
    generic_oled_ssd1306_draw_line(chXpos1, chYpos2, chXpos1, chYpos1, color_mode);
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_draw_filled_rectangle(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2,
                                                         uint8_t chYpos2,
                                                         generic_oled_ssd1306_color_mode color_mode) {
    // // 这里chXpos1, chYpos1, chXpos2, chYpos2分别代表左上和右下
    // if (chXpos2 - chXpos1 <= 0 || chYpos2 - chYpos1 <= 0) {
    //     return GENERIC_FAIL;
    // }
    // for (int i = 0; chXpos1 + i <= chXpos2 - i; i++) {
    //     generic_oled_ssd1306_draw_rectangle(chXpos1 + i, chYpos1 + i, chXpos2 - i, chYpos2 - i, color_mode);
    // }
    // return GENERIC_OK;
    uint8_t chXpos, chYpos;
    for (chXpos = chXpos1; chXpos <= chXpos2; chXpos++) {
        for (chYpos = chYpos1; chYpos <= chYpos2; chYpos++) {
            generic_oled_ssd1306_draw_point(chXpos, chYpos, color_mode);
        }
    }
}

generic_err_t generic_oled_ssd1306_draw_circle(uint8_t chXpos, uint8_t chYpos, uint8_t chRadius,
                                               generic_oled_ssd1306_color_mode color_mode) {
    int16_t a = 0, b = chRadius, di = 3 - (chRadius << 1);
    while (a <= b) {
        generic_oled_ssd1306_draw_point(chXpos - b, chYpos - a, color_mode);
        generic_oled_ssd1306_draw_point(chXpos + b, chYpos - a, color_mode);
        generic_oled_ssd1306_draw_point(chXpos - a, chYpos + b, color_mode);
        generic_oled_ssd1306_draw_point(chXpos - b, chYpos - a, color_mode);
        generic_oled_ssd1306_draw_point(chXpos - a, chYpos - b, color_mode);
        generic_oled_ssd1306_draw_point(chXpos + b, chYpos + a, color_mode);
        generic_oled_ssd1306_draw_point(chXpos + a, chYpos - b, color_mode);
        generic_oled_ssd1306_draw_point(chXpos + a, chYpos + b, color_mode);
        generic_oled_ssd1306_draw_point(chXpos - b, chYpos + a, color_mode);
        a++;
        if (di < 0) {
            di += 4 * a + 6;
        } else {
            di += 10 + 4 * (a - b);
            b--;
        }
        generic_oled_ssd1306_draw_point(chXpos + a, chYpos + b, color_mode);
    }
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_draw_filled_circle(uint8_t chXpos, uint8_t chYpos, uint8_t chRadius,
                                                      generic_oled_ssd1306_color_mode color_mode) {
    int16_t a = 0, b = chRadius, di = 3 - (chRadius << 1);
    while (a <= b) {
        for (int16_t i = chXpos - b; i <= chXpos + b; i++) {
            generic_oled_ssd1306_draw_point(i, chYpos + a, color_mode);
            generic_oled_ssd1306_draw_point(i, chYpos - a, color_mode);
        }
        for (int16_t i = chXpos - a; i <= chXpos + a; i++) {
            generic_oled_ssd1306_draw_point(i, chYpos + b, color_mode);
            generic_oled_ssd1306_draw_point(i, chYpos - b, color_mode);
        }
        a++;
        if (di < 0) {
            di += 4 * a + 6;
        } else {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_draw_triangle(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2,
                                                 uint8_t chXpos3, uint8_t chYpos3,
                                                 generic_oled_ssd1306_color_mode color_mode) {
    generic_oled_ssd1306_draw_line(chXpos1, chYpos1, chXpos2, chYpos2, color_mode);
    generic_oled_ssd1306_draw_line(chXpos2, chYpos2, chXpos3, chYpos3, color_mode);
    generic_oled_ssd1306_draw_line(chXpos3, chYpos3, chXpos1, chYpos1, color_mode);
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_draw_filled_triangle(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2,
                                                        uint8_t chYpos2, uint8_t chXpos3, uint8_t chYpos3,
                                                        generic_oled_ssd1306_color_mode color_mode) {
    uint8_t a = 0, b = 0, y = 0, last = 0;
    if (chYpos1 > chYpos2) {
        a = chYpos2;
        b = chYpos1;
    } else {
        a = chYpos1;
        b = chYpos2;
    }
    y = a;
    for (; y <= b; y++) {
        if (y <= chYpos3) {
            generic_oled_ssd1306_draw_line(chXpos1 + (y - chYpos1) * (chXpos2 - chXpos1) / (chYpos2 - chYpos1), y,
                                           chXpos1 + (y - chYpos1) * (chXpos3 - chXpos1) / (chYpos3 - chYpos1), y,
                                           color_mode);
        } else {
            last = y - 1;
            break;
        }
    }
    for (; y <= b; y++) {
        generic_oled_ssd1306_draw_line(chXpos2 + (y - chYpos2) * (chXpos3 - chXpos2) / (chYpos3 - chYpos2), y,
                                       chXpos1 + (y - last) * (chXpos3 - chXpos1) / (chYpos3 - last), y, color_mode);
    }
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_draw_eclipse(uint8_t chXpos, uint8_t chYpos, uint8_t a, uint8_t b,
                                                generic_oled_ssd1306_color_mode color_mode) {
    int xpos = 0, ypos = b;
    int a2 = a * a, b2 = b * b;
    int d = b2 + a2 * (0.25 - b);
    while (a2 * ypos > b2 * xpos) {
        generic_oled_ssd1306_draw_point(chXpos + xpos, chYpos + ypos, color_mode);
        generic_oled_ssd1306_draw_point(chXpos - xpos, chYpos + ypos, color_mode);
        generic_oled_ssd1306_draw_point(chXpos + xpos, chYpos - ypos, color_mode);
        generic_oled_ssd1306_draw_point(chXpos - xpos, chYpos - ypos, color_mode);
        if (d < 0) {
            d = d + b2 * ((xpos << 1) + 3);
            xpos += 1;
        } else {
            d = d + b2 * ((xpos << 1) + 3) + a2 * (-(ypos << 1) + 2);
            xpos += 1, ypos -= 1;
        }
    }
    d = b2 * (xpos + 0.5) * (xpos + 0.5) + a2 * (ypos - 1) * (ypos - 1) - a2 * b2;
    while (ypos > 0) {
        generic_oled_ssd1306_draw_point(chXpos + xpos, chYpos + ypos, color_mode);
        generic_oled_ssd1306_draw_point(chXpos - xpos, chYpos + ypos, color_mode);
        generic_oled_ssd1306_draw_point(chXpos + xpos, chYpos - ypos, color_mode);
        generic_oled_ssd1306_draw_point(chXpos - xpos, chYpos - ypos, color_mode);
        if (d < 0) {
            d = d + b2 * ((xpos << 1) + 2) + a2 * (-(ypos << 1) + 3);
            xpos += 1, ypos -= 1;
        } else {
            d = d + a2 * (-(ypos << 1) + 3);
            ypos -= 1;
        }
    }
    return GENERIC_OK;
}

void generic_oled_ssd1306_set_byte_fine(uint8_t page, uint8_t column, uint8_t data, uint8_t start, uint8_t end,
                                        generic_oled_ssd1306_color_mode color_mode) {
    static uint8_t temp;
    if (page >= OLED_PAGE || column >= OLED_COLUMN)
        return;
    if (color_mode)
        data = ~data;

    temp = data | (0xff << (end + 1)) | (0xff >> (8 - start));
    OLED_GRAM[page][column] &= temp;
    temp = data & ~(0xff << (end + 1)) & ~(0xff >> (8 - start));
    OLED_GRAM[page][column] |= temp;
}

void generic_oled_ssd1306_set_byte(uint8_t page, uint8_t column, uint8_t data,
                                   generic_oled_ssd1306_color_mode color_mode) {
    if (page >= OLED_PAGE || column >= OLED_COLUMN)
        return;
    if (color_mode)
        data = ~data;
    OLED_GRAM[page][column] = data;
}

void generic_oled_ssd1306_set_bits_fine(uint8_t x, uint8_t y, uint8_t data, uint8_t len,
                                        generic_oled_ssd1306_color_mode color_mode) {
    uint8_t page = y / 8;
    uint8_t bit = y % 8;
    if (bit + len > 8) {
        generic_oled_ssd1306_set_byte_fine(page, x, data << bit, bit, 7, color_mode);
        generic_oled_ssd1306_set_byte_fine(page + 1, x, data >> (8 - bit), 0, len + bit - 1 - 8, color_mode);
    } else {
        generic_oled_ssd1306_set_byte_fine(page, x, data << bit, bit, bit + len - 1, color_mode);
    }
}

void generic_oled_ssd1306_set_bits(uint8_t x, uint8_t y, uint8_t data, generic_oled_ssd1306_color_mode color_mode) {
    uint8_t page = y / 8;
    uint8_t bit = y % 8;
    generic_oled_ssd1306_set_byte_fine(page, x, data << bit, bit, 7, color_mode);
    if (bit) {
        generic_oled_ssd1306_set_byte_fine(page + 1, x, data >> (8 - bit), 0, bit - 1, color_mode);
    }
}

void generic_oled_ssd1306_set_block(uint8_t x, uint8_t y, const uint8_t *data, uint8_t w, uint8_t h,
                                    generic_oled_ssd1306_color_mode color_mode) {
    uint8_t fullRow = h / 8; // 完整的行数
    uint8_t partBit = h % 8; // 不完整的字节中的有效位数
    for (uint8_t i = 0; i < w; i++) {
        for (uint8_t j = 0; j < fullRow; j++) {
            generic_oled_ssd1306_set_bits(x + i, y + j * 8, data[i + j * w], color_mode);
        }
    }
    if (partBit) {
        uint16_t fullNum = w * fullRow; // 完整的字节数
        for (uint8_t i = 0; i < w; i++) {
            generic_oled_ssd1306_set_bits_fine(x + i, y + (fullRow * 8), data[fullNum + i], partBit, color_mode);
        }
    }
}

void generic_oled_ssd1306_draw_ascii_char(uint8_t x, uint8_t y, char ch, const ASCIIFont *font,
                                          generic_oled_ssd1306_color_mode color_mode) {
    generic_oled_ssd1306_set_block(x, y, font->chars + (ch - ' ') * (((font->h + 7) / 8) * font->w), font->w, font->h,
                                   color_mode);
}

uint8_t generic_oled_ssd1306_get_utf8_len(char *string) {
    if ((string[0] & 0x80) == 0x00) {
        return 1;
    }
    if ((string[0] & 0xE0) == 0xC0) {
        return 2;
    }
    if ((string[0] & 0xF0) == 0xE0) {
        return 3;
    }
    if ((string[0] & 0xF8) == 0xF0) {
        return 4;
    }
    return 0;
}

void generic_oled_ssd1306_draw_string(uint8_t chXpos, uint8_t chYpos, char *str, const Font *font,
                                      generic_oled_ssd1306_color_mode color_mode) {
    uint16_t i = 0; // 字符串索引
    uint8_t oneLen = (((font->h + 7) / 8) * font->w) + 4; // 一个字模占多少字节
    uint8_t found; // 是否找到字模
    uint8_t utf8Len; // UTF-8编码长度
    uint8_t *head; // 字模头指针
    while (str[i]) {
        found = 0;
        utf8Len = generic_oled_ssd1306_get_utf8_len(str + i);
        if (utf8Len == 0)
            break; // 有问题的UTF-8编码

        // 寻找字符  TODO 优化查找算法, 二分查找或者hash
        for (uint8_t j = 0; j < font->len; j++) {
            head = (uint8_t *) (font->chars) + (j * oneLen);
            if (memcmp(str + i, head, utf8Len) == 0) {
                generic_oled_ssd1306_set_block(chXpos, chYpos, head + 4, font->w, font->h, color_mode);
                // 移动光标
                chXpos += font->w;
                i += utf8Len;
                found = 1;
                break;
            }
        }

        // 若未找到字模,且为ASCII字符, 则缺省显示ASCII字符
        if (found == 0) {
            if (utf8Len == 1) {
                generic_oled_ssd1306_draw_ascii_char(chXpos, chYpos, str[i], font->ascii, color_mode);
                // 移动光标
                chXpos += font->ascii->w;
                i += utf8Len;
            } else {
                generic_oled_ssd1306_draw_ascii_char(chXpos, chYpos, ' ', font->ascii, color_mode);
                chXpos += font->ascii->w;
                i += utf8Len;
            }
        }
    }
}

void generic_oled_ssd1306_draw_bitmap(uint8_t x, uint8_t y, const Image *img,
                                       generic_oled_ssd1306_color_mode color_mode) {
     generic_oled_ssd1306_set_block(x, y, img->data, img->w, img->h, color_mode);
}

generic_err_t generic_oled_ssd1306_whoami(generic_oled_ssd1306_driver_interface_t *pfdev,
                                          generic_err_t (*generic_send_func)(uint8_t *data, size_t len),
                                          generic_err_t (*generic_receive_func)(uint8_t *data, size_t len),
                                          generic_err_t (*delay_ms_func)(uint32_t nms),
                                          generic_err_t (*delay_us_func)(uint32_t nus)) {
    pfdev->pfsystem_interface = malloc(sizeof(system_interface_t));
    if (pfdev->pfsystem_interface == NULL) {
        return GENERIC_FAIL;
    }
    pfdev->pfsystem_interface->send = generic_send_func;
    pfdev->pfsystem_interface->receive = generic_receive_func;
    pfdev->pfsystem_interface->delay_ms = delay_ms_func;
    pfdev->pfsystem_interface->delay_us = delay_us_func;
    pfdev->pfinit = generic_oled_ssd1306_init;
    pfdev->pfdisplay_on = generic_oled_ssd1306_display_on;
    pfdev->pfdisplay_off = generic_oled_ssd1306_display_off;
    pfdev->pfnewframe = generic_oled_ssd1306_newframe;
    pfdev->pfshowframe = generic_oled_ssd1306_showframe;
    pfdev->pfdraw_point = generic_oled_ssd1306_draw_point;
    pfdev->pfdraw_line = generic_oled_ssd1306_draw_line;
    pfdev->pfdraw_rectangle = generic_oled_ssd1306_draw_rectangle;
    pfdev->pfdraw_filled_rectangle = generic_oled_ssd1306_draw_filled_rectangle;
    pfdev->pfdraw_circle = generic_oled_ssd1306_draw_circle;
    pfdev->pfdraw_filled_circle = generic_oled_ssd1306_draw_filled_circle;
    pfdev->pfdraw_triangle = generic_oled_ssd1306_draw_triangle;
    pfdev->pfdraw_filled_triangle = generic_oled_ssd1306_draw_filled_triangle;
    pfdev->pfdraw_eclipse = generic_oled_ssd1306_draw_eclipse;
    pfdev->pfdirect_set_color_mode = generic_oled_ssd1306_direct_set_color_mode;
    pfdev->pfdraw_string = generic_oled_ssd1306_draw_string;
    pfdev->pfdraw_image = generic_oled_ssd1306_draw_bitmap;
    return GENERIC_OK;
}

generic_err_t generic_oled_ssd1306_del(generic_oled_ssd1306_driver_interface_t *pfdev) {
    free(pfdev->pfsystem_interface);
    pfdev->pfsystem_interface = NULL;
    free(pfdev);
    pfdev = NULL;
    return GENERIC_OK;
}
