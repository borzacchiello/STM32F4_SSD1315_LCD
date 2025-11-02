#ifndef INC_SSD1315_PORT_H_
#define INC_SSD1315_PORT_H_

#include "ssd1315.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

int32_t SSD1315_AttachToI2C1(SSD1315_Object_t* pObj, I2C_HandleTypeDef* _hi2c);
void    SSD1315_PrintText(SSD1315_Object_t* pObj, uint8_t lnum, uint8_t cnum,
                          const char* text);
void    SSD1315_PrintChar(SSD1315_Object_t* pObj, uint8_t lnum, uint8_t cnum,
                          char c);

static inline uint32_t SSD1315_TextRows() { return SSD1315_LCD_PIXEL_HEIGHT / 8; }
static inline uint32_t SSD1315_TextCols() { return SSD1315_LCD_PIXEL_WIDTH / 8; }

#endif /* INC_SSD1315_PORT_H_ */
