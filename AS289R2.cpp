/**
 ******************************************************************************
 * @file    AS289R2.cpp
 * @author  Toyomasa Watarai
 * @version V1.1.0
 * @date    20 January 2020
 * @brief   AS289R2 class implementation
 ******************************************************************************
 * @attention
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mbed.h"
#include "AS289R2.h"

AS289R2::AS289R2(PinName tx, PinName rx, uint32_t baud)
    :
    _serial_p(new RawSerial(tx, rx)),
    _serial(*_serial_p)
{
    _serial.baud(baud);
    initialize();
}

AS289R2::AS289R2(RawSerial &serial_obj, uint32_t baud)
    :
    _serial_p(NULL),
    _serial(serial_obj)
{
    _serial.baud(baud);
    initialize();
}

AS289R2::~AS289R2()
{
}

void AS289R2::initialize(void)
{
    _serial.putc(0x1B);
    _serial.putc(0x40);
}

void AS289R2::putLineFeed(uint32_t lines)
{
    for(uint32_t i = 0; i < lines; i++) {
        _serial.putc('\r');
    }
}

void AS289R2::clearBuffer(void)
{
    _serial.putc(0x18);
}

void AS289R2::setDoubleSizeHeight(void)
{
    _serial.printf("\x1B\x4E\x31");
}

void AS289R2::clearDoubleSizeHeight(void)
{
    _serial.printf("\x1B\x4E\x30");
}

void AS289R2::setDoubleSizeWidth(void)
{
    _serial.printf("\x1B\x57\x31");
}

void AS289R2::clearDoubleSizeWidth(void)
{
    _serial.printf("\x1B\x57\x30");
}

void AS289R2::setLargeFont(void)
{
    _serial.printf("\x1B\x4C\x31");
}

void AS289R2::clearLargeFont()
{
    _serial.printf("\x1B\x4C\x30");
}

void AS289R2::setANKFont(uint32_t font)
{
    _serial.putc(0x1B);
    _serial.putc(0x68);
    _serial.putc(font);
}

void AS289R2::setKanjiFont(uint32_t font)
{
    _serial.putc(0x12);
    _serial.putc(0x53);
    _serial.putc(font);
}

void AS289R2::printQRCode(uint32_t err, const char* param)
{
    uint32_t len = strlen(param);
    char buf[4] = {0x1D, 0x78};
    buf[2] = err;
    buf[3] = len;
    for (uint32_t i = 0; i < sizeof(buf); i++) {
        _serial.putc(buf[i]);
    }
    for (uint32_t i = 0; i < len; i++) {
        _serial.putc(param[i]);
    }
}

void AS289R2::printBarCode(uint32_t code, const char* param)
{
    char buf[3] = {0x1D, 0x6B};
    buf[2] = code;
    for (uint32_t i = 0; i < sizeof(buf); i++) {
        _serial.putc(buf[i]);
    }
    for (uint32_t i = 0; i < strlen(param); i++) {
        _serial.putc(param[i]);
    }
    _serial.putc('\0');
}

void AS289R2::printBitmapImage(uint32_t mode, uint16_t lines, const uint8_t * image)
{
    char buf[3] = {0x1C, 0x2A};
    buf[2] = mode;
    for (uint32_t i = 0; i < sizeof(buf); i++) {
        _serial.putc(buf[i]);
    }
    _serial.putc((lines >> 8) & 0xFF); // n1
    _serial.putc((lines >> 0) & 0xFF); // n2

    if (mode == 0x61) {
        return;
    }

    for (uint32_t i = 0; i < (48 * lines); i++) {
        _serial.putc(image[i]);
    }
}

void AS289R2::setLineSpaceing(uint32_t space)
{
    _serial.putc(0x1B);
    _serial.putc(0x33);
    _serial.putc(space);
}

void AS289R2::defaultLineSpaceing()
{
    _serial.printf("\x1B\x33\x04");
}

void AS289R2::setPrintDirection(uint32_t direction)
{
    _serial.putc(0x1B);
    _serial.putc(0x49);
    _serial.putc(direction);
}

void AS289R2::putPaperFeed(uint32_t space)
{
    _serial.putc(0x1B);
    _serial.putc(0x4A);
    _serial.putc(space);
}

void AS289R2::setInterCharacterSpace(uint32_t space)
{
    _serial.putc(0x1B);
    _serial.putc(0x20);
    _serial.putc(space);
}

void AS289R2::defaultInterCharacterSpace()
{
    _serial.printf("\x1B\x20\x01");
}

void AS289R2::putPrintPosition(uint32_t position)
{
    _serial.putc(0x1B);
    _serial.putc(0x6c);
    _serial.putc(position);
}

void AS289R2::setScript(script_mode script)
{
    _serial.putc(0x1B);
    _serial.putc(0x73);
    _serial.putc(script);
}

void AS289R2::clearScript()
{
    _serial.printf("\x1B\x73\x30");
}

void AS289R2::setQuadrupleSize()
{
    _serial.printf("\x1C\x57\x31");
}

void AS289R2::clearQuadrupleSize()
{
    _serial.printf("\x1C\x57\x30");
}

void AS289R2::setEnlargement(uint32_t width, uint32_t height)
{
    _serial.putc(0x1C);
    _serial.putc(0x65);
    _serial.putc(width);
    _serial.putc(height);
}

void AS289R2::clearEnlargement()
{
    _serial.printf("\x1C\x65\x31\x31");
}

void AS289R2::setBarCodeHeight(uint32_t height)
{
    _serial.putc(0x1D);
    _serial.putc(0x68);
    _serial.putc(height);
}

void AS289R2::defaultBarCodeHeight()
{
    _serial.printf("\x1D\x68\x50");
}

void AS289R2::setBarCodeBarSize(uint32_t narrowbar, uint32_t widebar)
{
    _serial.putc(0x1D);
    _serial.putc(0x77);
    _serial.putc(narrowbar);
    _serial.putc(widebar);
}

void AS289R2::defaultBarCodeBarSize()
{
    _serial.printf("\x1D\x77\x02\x05");
}

int AS289R2::_putc(int value)
{
    _serial.putc(value);
    return value;
}

int AS289R2::_getc()
{
    return -1;
}
