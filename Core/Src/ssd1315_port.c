#include "stm32f4xx_hal.h"
#include "ssd1315.h"
#include "font.h"

extern void* memcpy(void*, const void*, unsigned int);

#define SSD1315_I2C_ADDR_7B (0x3C)
#define SSD1315_I2C_ADDR_W  ((SSD1315_I2C_ADDR_7B) << 1)

static uint8_t framebuffer[SSD1315_LCD_COLUMN_NUMBER * SSD1315_LCD_PAGE_NUMBER +
                           1] __attribute__((aligned(16)));
static I2C_HandleTypeDef* hi2c;

static int32_t SSD1315_I2C_Init(void)
{
    if (HAL_I2C_IsDeviceReady(hi2c, SSD1315_I2C_ADDR_W, 2, 100) != HAL_OK)
        return SSD1315_ERROR;
    return SSD1315_OK;
}

static int32_t SSD1315_I2C_DeInit(void) { return SSD1315_OK; }

static int32_t SSD1315_GetTick(void) { return (int32_t)HAL_GetTick(); }

static int32_t SSD1315_I2C_WriteReg(uint16_t Reg /*unused*/, uint8_t* pData,
                                    uint16_t Length)
{
    (void)Reg;

    HAL_StatusTypeDef st;

    if (Length == 1) {
        uint8_t b = pData[0];

        if (b == SSD1315_READWRITE_CMD) {
            return SSD1315_OK;
        }

        // Single command byte; ensure it is sent with control byte 0x80
        uint8_t buf[2];
        buf[0] = SSD1315_READWRITE_CMD;
        buf[1] = b;

        st = HAL_I2C_Master_Transmit(hi2c, SSD1315_I2C_ADDR_W, buf, sizeof(buf),
                                     100);
        return (st == HAL_OK) ? SSD1315_OK : SSD1315_ERROR;
    } else {
        // TODO: the port requires to copy the data to a private framebuffer,
        // maybe modify the original code to have room for the extra 0x40 byte

        // Framebuffer or other data burst: prefix with 0x40 (data stream)
        framebuffer[0] = 0x40; // control: Co=0, D/C#=1 (data)
        memcpy(&framebuffer[1], pData, Length);

        st = HAL_I2C_Master_Transmit(hi2c, SSD1315_I2C_ADDR_W, framebuffer,
                                     Length + 1, 100);
        return (st == HAL_OK) ? SSD1315_OK : SSD1315_ERROR;
    }
}

static int32_t SSD1315_I2C_ReadReg(uint16_t Reg, uint8_t* pData,
                                   uint16_t Length)
{
    (void)Reg;
    (void)pData;
    (void)Length;
    return SSD1315_ERROR;
}

int32_t SSD1315_AttachToI2C1(SSD1315_Object_t* pObj, I2C_HandleTypeDef* _hi2c)
{
    hi2c            = _hi2c;
    SSD1315_IO_t io = {.Init     = SSD1315_I2C_Init,
                       .DeInit   = SSD1315_I2C_DeInit,
                       .WriteReg = SSD1315_I2C_WriteReg,
                       .ReadReg  = SSD1315_I2C_ReadReg,
                       .GetTick  = SSD1315_GetTick};
    return SSD1315_RegisterBusIO(pObj, &io);
}

/*
 * Some utility functions to print text on the screen
 */

void SSD1315_PrintChar(SSD1315_Object_t* pObj, uint8_t lnum, uint8_t cnum,
                       char c)
{
    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            uint8_t  mask  = 1 << i;
            uint32_t color = SSD1315_COLOR_BLACK;
            if (font8x8_basic[(uint8_t)c][j] & mask) {
                color = SSD1315_COLOR_WHITE;
            }
            SSD1315_SetPixel(pObj, cnum * 8 + i, lnum * 8 + j, color);
        }
    }
}

void SSD1315_PrintText(SSD1315_Object_t* pObj, uint8_t lnum, uint8_t cnum,
                       const char* text)
{
    const char* curr = text;
    while (*curr) {
        SSD1315_PrintChar(pObj, lnum, cnum++, *curr++);
    }
}
