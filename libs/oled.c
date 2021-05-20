#include <msp430.h>
#include <stdint.h>
#include "font.h"
#include "i2c.h"
#include "oled.h"

/* ====================================================================
 * Horizontal Centering Number Array
 * ==================================================================== */
const unsigned char HcenterUL[] = {                                           // Horizontally center number with separators on screen
                               0,                                       // 0 digits, not used but included to size array correctly
                               61,                                      // 1 digit
                               58,                                      // 2 digits
                               55,                                      // 3 digits
                               49,                                      // 4 digits and 1 separator
                               46,                                      // 5 digits and 1 separator
                               43,                                      // 6 digits and 1 separator
                               37,                                      // 7 digits and 2 separators
                               34,                                      // 8 digits and 2 separators
                               31,                                      // 9 digits and 2 separators
                               25                                       // 10 digits and 3 separators
};


void oledConfig() {

    oledSendCommand(0xAE);    // Set display OFF

    oledSendCommand(0xD5);    // Set Display Clock Divide Ratio / OSC Frequency
    oledSendCommand(0x80);    // Display Clock Divide Ratio / OSC Frequency

    oledSendCommand(0xA8);    // Set Multiplex Ratio
    oledSendCommand(0x3F);    // Multiplex Ratio for 128x64 (64-1)

    oledSendCommand(0xD3);    // Set Display Offset
    oledSendCommand(0x00);    // Display Offset

    oledSendCommand(0x40);    // Set Display Start Line

    oledSendCommand(0x8D);    // Set Charge Pump
    oledSendCommand(0x14);    // Charge Pump (0x10 External, 0x14 Internal DC/DC)

    oledSendCommand(0x20);
    oledSendCommand(0x01);
    oledSendCommand(0xA0 | 0x1);
    oledSendCommand(0xC8);

    oledSendCommand(0xDA);    // Set COM Hardware Configuration
    oledSendCommand(0x12);    // COM Hardware Configuration

    oledSendCommand(0x81);    // Set Contrast
    oledSendCommand(0xCF);    // Contrast

    oledSendCommand(0xD9);    // Set Pre-Charge Period
    oledSendCommand(0xF1);    // Set Pre-Charge Period (0x22 External, 0xF1 Internal)

    oledSendCommand(0xDB);    // Set VCOMH Deselect Level
    oledSendCommand(0x40);    // VCOMH Deselect Level

    oledSendCommand(0xA4);    // Set all pixels OFF
    oledSendCommand(0xA6);    // Set display not inverted
    oledSendCommand(0x2E);
    oledSendCommand(0xAF);    // Set display On

}

void oledSendCommand(uint8_t command) {
    uint8_t buffer[2];
    buffer[0] = 0x80;
    buffer[1] = command;

    i2cWrite(OLED_I2C_ADDRESS, buffer, 2);
}

void oledSendChar(uint8_t column, uint8_t page, uint8_t byte) {
    uint8_t buffer[3], i;

    buffer[0] = 0x40;

    for(i = 0; i < 5; i++) {
        oledSetPosition(column + i, page);

        buffer[1] = oled_font_5x7[byte - ' '][i];
        buffer[2] = 0x00;

        i2cWrite(OLED_I2C_ADDRESS, buffer, 3);
    }
}

void oledSendString(uint8_t column, uint8_t page, uint8_t *string) {
    while(*string) {
        oledSendChar(column, page, *string++);
        column = column + 5;
    }
}

void oledSetPosition(uint8_t column, uint8_t page) {
    if (column > 128) {
        column = 0;
    }

    if (page > 8) {
        page = 0;
    }

    oledSendCommand(0x21);
    oledSendCommand(column);
    oledSendCommand(127);

    oledSendCommand(0x22);
    oledSendCommand(page);
    oledSendCommand(7);
}

void oledClearDisplay() {
    uint8_t buffer[9] = {0x00}, i;

    oledSetPosition(0, 0);

    buffer[0] = 0x40;

    for (i=0; i < 128; i++)
        i2cWrite(OLED_I2C_ADDRESS, buffer, 9);
}
