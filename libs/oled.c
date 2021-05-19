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

const uint8_t oled_font_5x7[96][5] = {{0x00, 0x00, 0x00, 0x00, 0x00},  // space
                                       {0x00, 0x00, 0x4F, 0x00, 0x00},  // !
                                       {0x00, 0x07, 0x00, 0x07, 0x00},  // "
                                       {0x14, 0x7F, 0x14, 0x7F, 0x14},  // #
                                       {0x24, 0x2A, 0x7F, 0x2A, 0x12},  // $
                                       {0x23, 0x13, 0x08, 0x64, 0x62},  // %
                                       {0x36, 0x49, 0x55, 0x22, 0x50},  // &
                                       {0x00, 0x05, 0x03, 0x00, 0x00},  // '
                                       {0x00, 0x1C, 0x22, 0x41, 0x00},  // (
                                       {0x00, 0x41, 0x22, 0x1C, 0x00},  // )
                                       {0x14, 0x08, 0x3E, 0x08, 0x14},  // *
                                       {0x08, 0x08, 0x3E, 0x08, 0x08},  // +
                                       {0x00, 0x50, 0x30, 0x00, 0x00},  // ,
                                       {0x08, 0x08, 0x08, 0x08, 0x08},  // -
                                       {0x00, 0x60, 0x60, 0x00, 0x00},  // .
                                       {0x20, 0x10, 0x08, 0x04, 0x02},  // /
                                       {0x3E, 0x51, 0x49, 0x45, 0x3E},  // 0
                                       {0x00, 0x42, 0x7F, 0x40, 0x00},  // 1
                                       {0x42, 0x61, 0x51, 0x49, 0x46},  // 2
                                       {0x21, 0x41, 0x45, 0x4B, 0x31},  // 3
                                       {0x18, 0x14, 0x12, 0x7F, 0x10},  // 4
                                       {0x27, 0x49, 0x49, 0x49, 0x31},  // 5
                                       {0x3C, 0x4A, 0x49, 0x49, 0x30},  // 6
                                       {0x01, 0x71, 0x09, 0x05, 0x03},  // 7
                                       {0x36, 0x49, 0x49, 0x49, 0x36},  // 8
                                       {0x06, 0x49, 0x49, 0x29, 0x1E},  // 9
                                       {0x00, 0x36, 0x36, 0x00, 0x00},  // :
                                       {0x00, 0x56, 0x36, 0x00, 0x00},  // ;
                                       {0x08, 0x14, 0x22, 0x41, 0x00},  // <
                                       {0x14, 0x14, 0x14, 0x14, 0x14},  // =
                                       {0x00, 0x41, 0x22, 0x14, 0x08},  // >
                                       {0x02, 0x01, 0x51, 0x09, 0x06},  // ?
                                       {0x32, 0x49, 0x79, 0x41, 0x3E},  // @
                                       {0x7E, 0x11, 0x11, 0x11, 0x7E},  // A
                                       {0x7F, 0x49, 0x49, 0x49, 0x36},  // B
                                       {0x3E, 0x41, 0x41, 0x41, 0x22},  // C
                                       {0x7F, 0x41, 0x41, 0x22, 0x1C},  // D
                                       {0x7F, 0x49, 0x49, 0x49, 0x41},  // E
                                       {0x7F, 0x09, 0x09, 0x09, 0x01},  // F
                                       {0x3E, 0x41, 0x49, 0x49, 0x7A},  // G
                                       {0x7F, 0x08, 0x08, 0x08, 0x7F},  // H
                                       {0x00, 0x41, 0x7F, 0x41, 0x00},  // I
                                       {0x20, 0x40, 0x41, 0x3F, 0x01},  // J
                                       {0x7F, 0x08, 0x14, 0x22, 0x41},  // K
                                       {0x7F, 0x40, 0x40, 0x40, 0x40},  // L
                                       {0x7F, 0x02, 0x0C, 0x02, 0x7F},  // M
                                       {0x7F, 0x04, 0x08, 0x10, 0x7F},  // N
                                       {0x3E, 0x41, 0x41, 0x41, 0x3E},  // O
                                       {0x7F, 0x09, 0x09, 0x09, 0x06},  // P
                                       {0x3E, 0x41, 0x51, 0x21, 0x5E},  // Q
                                       {0x7F, 0x09, 0x19, 0x29, 0x46},  // R
                                       {0x46, 0x49, 0x49, 0x49, 0x31},  // S
                                       {0x01, 0x01, 0x7F, 0x01, 0x01},  // T
                                       {0x3F, 0x40, 0x40, 0x40, 0x3F},  // U
                                       {0x1F, 0x20, 0x40, 0x20, 0x1F},  // V
                                       {0x3F, 0x40, 0x38, 0x40, 0x3F},  // W
                                       {0x63, 0x14, 0x08, 0x14, 0x63},  // X
                                       {0x07, 0x08, 0x70, 0x08, 0x07},  // Y
                                       {0x61, 0x51, 0x49, 0x45, 0x43},  // Z
                                       {0x7F, 0x41, 0x41, 0x00, 0x00},  // [
                                       {0x02, 0x04, 0x08, 0x10, 0x20},  // forward slash
                                       {0x00, 0x41, 0x41, 0x7F, 0x00},  // ]
                                       {0x04, 0x02, 0x01, 0x02, 0x04},  // ^
                                       {0x40, 0x40, 0x40, 0x40, 0x40},  // _
                                       {0x00, 0x01, 0x02, 0x04, 0x00},  // `
                                       {0x20, 0x54, 0x54, 0x54, 0x78},  // a
                                       {0x7F, 0x48, 0x44, 0x44, 0x38},  // b
                                       {0x38, 0x44, 0x44, 0x44, 0x20},  // c
                                       {0x38, 0x44, 0x44, 0x48, 0x7F},  // d
                                       {0x38, 0x54, 0x54, 0x54, 0x18},  // e
                                       {0x08, 0x7E, 0x09, 0x01, 0x02},  // f
                                       {0x0C, 0x52, 0x52, 0x52, 0x3E},  // g
                                       {0x7F, 0x08, 0x04, 0x04, 0x78},  // h
                                       {0x00, 0x44, 0x7D, 0x40, 0x00},  // i
                                       {0x20, 0x40, 0x44, 0x3D, 0x00},  // j
                                       {0x7F, 0x10, 0x28, 0x44, 0x00},  // k
                                       {0x00, 0x41, 0x7F, 0x40, 0x00},  // l
                                       {0x7C, 0x04, 0x18, 0x04, 0x78},  // m
                                       {0x7C, 0x08, 0x04, 0x04, 0x78},  // n
                                       {0x38, 0x44, 0x44, 0x44, 0x38},  // o
                                       {0x7C, 0x14, 0x14, 0x14, 0x08},  // p
                                       {0x08, 0x14, 0x14, 0x18, 0x7C},  // q
                                       {0x7C, 0x08, 0x04, 0x04, 0x08},  // r
                                       {0x48, 0x54, 0x54, 0x54, 0x20},  // s
                                       {0x04, 0x3F, 0x44, 0x40, 0x20},  // t
                                       {0x3C, 0x40, 0x40, 0x20, 0x7C},  // u
                                       {0x1C, 0x20, 0x40, 0x20, 0x1C},  // v
                                       {0x3C, 0x40, 0x30, 0x40, 0x3C},  // w
                                       {0x44, 0x28, 0x10, 0x28, 0x44},  // x
                                       {0x0C, 0x50, 0x50, 0x50, 0x3C},  // y
                                       {0x44, 0x64, 0x54, 0x4C, 0x44},  // z
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

    i2cWrite(OLED_I2C_ADDRESS, &buffer, 2);
}

void oledSendChar(uint8_t column, uint8_t page, uint8_t byte) {
    uint8_t buffer[3], i;

    buffer[0] = 0x40;

    for(i = 0; i < 5; i++) {
        oledSetPosition(column + i, page);

        buffer[1] = oled_font_5x7[byte - ' '][i];
        buffer[2] = 0x00;

        i2cWrite(OLED_I2C_ADDRESS, &buffer, 3);
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
        i2cWrite(OLED_I2C_ADDRESS, &buffer, 9);
}

void ssd1306_printText(uint8_t x, uint8_t y, char *ptString) {
    ssd1306_setPosition(x, y);

    while (*ptString != '\0') {
        buffer[0] = 0x40;

        if ((x + 5) >= 127) {                                           // char will run off screen
            x = 0;                                                      // set column to 0
            y++;                                                        // jump to next page
            ssd1306_setPosition(x, y);                                  // send position change to oled
        }

        uint8_t i;
        for(i = 0; i< 5; i++) {
            buffer[i+1] = font_5x7[*ptString - ' '][i];
        }

        buffer[6] = 0x0;

        i2c_write(buffer, 7);
        ptString++;
        x+=6;
    }
} // end ssd1306_printText

void ssd1306_printTextBlock(uint8_t x, uint8_t y, char *ptString) {
    char word[12];
    uint8_t i;
    uint8_t endX = x;
    while (*ptString != '\0'){
        i = 0;
        while ((*ptString != ' ') && (*ptString != '\0')) {
            word[i] = *ptString;
            ptString++;
            i++;
            endX += 6;
        }

        word[i++] = '\0';

        if (endX >= 127) {
            x = 0;
            y++;
            ssd1306_printText(x, y, word);
            endX = (i * 6);
            x = endX;
        } else {
            ssd1306_printText(x, y, word);
            endX += 6;
            x = endX;
        }
        ptString++;
    }

}


void ssd1306_printUI32( uint8_t x, uint8_t y, uint32_t val, uint8_t Hcenter ) {
    char text[14];

    ultoa(val, text);
    if (Hcenter) {
        ssd1306_printText(HcenterUL[digits(val)], y, text);
    } else {
        ssd1306_printText(x, y, text);
    }
} // end ssd1306_printUI32

uint8_t digits(uint32_t n) {
    if (n < 10) {
        return 1;
    } else if (n < 100) {
        return 2;
    } else if (n < 1000) {
        return 3;
    } else if (n < 10000) {
        return 4;
    } else if (n < 100000) {
        return 5;
    } else if (n < 1000000) {
        return 6;
    } else if (n < 10000000) {
        return 7;
    } else if (n < 100000000) {
        return 8;
    } else if (n < 1000000000) {
        return 9;
    } else {
        return 10;
    }
} // end digits

void ultoa(uint32_t val, char *string) {
    uint8_t i = 0;
    uint8_t j = 0;
                                                                        // use do loop to convert val to string
    do {
        if (j==3) {                                                     // we have reached a separator position
            string[i++] = ',';                                          // add a separator to the number string
            j=0;                                                        // reset separator indexer thingy
        }
            string[i++] = val%10 + '0';                                 // add the ith digit to the number string
            j++;                                                        // increment counter to keep track of separator placement
    } while ((val/=10) > 0);

    string[i++] = '\0';                                                 // add termination to string
    reverse(string);                                                    // string was built in reverse, fix that
} // end ultoa

void reverse(char *s)
{
    uint8_t i, j;
    uint8_t c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
} // end reverse
