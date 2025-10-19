#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include <lcom/timer.h>

int(util_sys_inb)(int port, uint8_t *value);
int(util_get_MSB)(uint16_t val, uint8_t *msb);
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

#endif
