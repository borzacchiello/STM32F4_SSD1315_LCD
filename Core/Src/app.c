#include "app.h"
#include "ssd1315.h"
#include "ssd1315_port.h"

#include <stdint.h>
#include <string.h>

extern UART_HandleTypeDef huart1;
extern SSD1315_Object_t   hssd1315;

static void UART_Print(const char* msg)
{
    HAL_UART_Transmit(&huart1, (const uint8_t*)msg, strlen(msg), 100);
}

static void SSD1315_ClearScreen()
{
    SSD1315_FillRect(&hssd1315, 0, 0, SSD1315_LCD_PIXEL_WIDTH,
                     SSD1315_LCD_PIXEL_HEIGHT, SSD1315_COLOR_BLACK);
}

void app_main()
{
    UART_Print("app_main started\r\n");

    SSD1315_ClearScreen();
    SSD1315_Refresh(&hssd1315);

    uint32_t l = 0;
    while (1) {
        UART_Print("app_main looping...\r\n");
        HAL_Delay(1000);

        SSD1315_ClearScreen();
        SSD1315_PrintText(&hssd1315, l, 2, "Hello World!");
        SSD1315_Refresh(&hssd1315);

        l = (l + 1) % SSD1315_TextRows();
    }
}
