/* Trabalho Final de Laboratório de Sistemas Microprocessados
 * Grupo:
 *       Bruno Couto Mariño - 190011106
 *       Clara Rezende Maia - 180030931
 */

#ifndef __OLED_H
#define __OLED_H

/* ====================================================================
 * OLED OLED Settings and Command Definitions
 * ==================================================================== */

#define OLED_I2C_ADDRESS             0x3C

#define OLED_LCDWIDTH                128
#define OLED_LCDHEIGHT               64
#define OLED_128_64
#define OLED_PAGES                   8

#define OLED_SETCONTRAST             0x81
#define OLED_DISPLAYALLON_RESUME     0xA4
#define OLED_DISPLAYALLON            0xA5
#define OLED_NORMALDISPLAY           0xA6
#define OLED_INVERTDISPLAY           0xA7
#define OLED_DISPLAYOFF              0xAE
#define OLED_DISPLAYON               0xAF

#define OLED_SETDISPLAYOFFSET        0xD3
#define OLED_SETCOMPINS              0xDA

#define OLED_SETVCOMDETECT           0xDB

#define OLED_SETDISPLAYCLOCKDIV      0xD5
#define OLED_SETPRECHARGE            0xD9

#define OLED_SETMULTIPLEX            0xA8

#define OLED_SETLOWCOLUMN            0x00
#define OLED_SETHIGHCOLUMN           0x10

#define OLED_SETSTARTLINE            0x40

#define OLED_MEMORYMODE              0x20
#define OLED_COLUMNADDR              0x21
#define OLED_PAGEADDR                0x22

#define OLED_COMSCANINC              0xC0
#define OLED_COMSCANDEC              0xC8

#define OLED_SEGREMAP                0xA0

#define OLED_CHARGEPUMP              0x8D

#define OLED_EXTERNALVCC             0x1
#define OLED_SWITCHCAPVCC            0x2

// currently no scroll functionality, left for possible future use
#define OLED_ACTIVATE_SCROLL                         0x2F
#define OLED_DEACTIVATE_SCROLL                       0x2E
#define OLED_SET_VERTICAL_SCROLL_AREA                0xA3
#define OLED_RIGHT_HORIZONTAL_SCROLL                 0x26
#define OLED_LEFT_HORIZONTAL_SCROLL                  0x27
#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A

/* ====================================================================
 * SSD1306 OLED Prototype Definitions
 * ==================================================================== */

void oledConfig();
void oledSendCommand(uint8_t command);
void oledSendChar(uint8_t column, uint8_t page, uint8_t byte);
void oledSetPosition(uint8_t column, uint8_t page);
void oledClearDisplay();
void sendText(uint8_t column, uint8_t page, uint8_t* str);

#endif /* __OLED_H */
