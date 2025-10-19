#include <lcom/lcf.h>
#include <stdint.h>

uint32_t cnt = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  if (lsb == NULL)
    return 1;        // ensures valid pointer to store the result
  *lsb = val & 0xFF; // AND operation to select the 8 bits
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  if (msb == NULL)
    return 1;               // ensures valid pointer to store the result
  *msb = (val >> 8) & 0xFF; // Right shift & AND operation to select the 8 bits
  return 1;
}

int(util_sys_inb)(int port, uint8_t *value) {

  // reads from I/o port and stores it in value
  // first reads the 32bit value
  // then stores the 8 bits necessary
  cnt++;
  if (value == NULL)
    return 1; // ensures valid pointer to store the result

  uint32_t temp;                  // will store original value
  int ret = sys_inb(port, &temp); // ret will store the status of the function: 1 for failure, 0 for success
  *value = temp & 0xFF;           // AND operation to select the 8 bits

  return ret;
}
