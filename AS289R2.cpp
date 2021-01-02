/* AS289R2 thermal control component for mbed OS
 * Copyright (c) 2016-2020, Toyomasa Watarai
 * SPDX-License-Identifier: Apache-2.0
*/

#include "mbed.h"
#include "AS289R2.h"

AS289R2::AS289R2(PinName tx, PinName rx, uint32_t baud)
    :
    _serial_p(new UnbufferedSerial(tx, rx)),
    _serial(*_serial_p)
{
#if MBED_MAJOR_VERSION > 5
    _serial.baud(baud);
#else
    _serial.set_baud(baud);
#endif
    initialize();
}

AS289R2::AS289R2(UnbufferedSerial &serial_obj, uint32_t baud)
    :
    _serial_p(NULL),
    _serial(serial_obj)
{
#if MBED_MAJOR_VERSION > 5
    _serial.baud(baud);
#else
    _serial.set_baud(baud);
#endif
    initialize();
}

AS289R2::~AS289R2()
{
}

void AS289R2::initialize(void)
{
    _serial.write((void *)"\x1B\x40", 2);
}

void AS289R2::putLineFeed(uint32_t lines)
{
    for(uint32_t i = 0; i < lines; i++) {
        _serial.write((void *)"\r", 1);
    }
}

void AS289R2::clearBuffer(void)
{
    _serial.write((void *)"\x19", 1);
}

void AS289R2::setDoubleSizeHeight(void)
{
    _serial.write((void *)"\x1B\x4E\x31", 3);
}

void AS289R2::clearDoubleSizeHeight(void)
{
    _serial.write((void *)"\x1B\x4E\x30", 3);
}

void AS289R2::setDoubleSizeWidth(void)
{
    _serial.write((void *)"\x1B\x57\x31", 3);
}

void AS289R2::clearDoubleSizeWidth(void)
{
    _serial.write((void *)"\x1B\x57\x30", 3);
}

void AS289R2::setLargeFont(void)
{
    _serial.write((void *)"\x1B\x4C\x31", 3);
}

void AS289R2::clearLargeFont()
{
    _serial.write((void *)"\x1B\x4C\x30", 3);
}

void AS289R2::setANKFont(uint32_t font)
{
    char buf[13];
    buf[0] = 0x1B;
    buf[1] = 0x68;
    buf[2] = (char)font;
    _serial.write(buf, 3);
}

void AS289R2::setKanjiFont(uint32_t font)
{
    char buf[3];
    buf[0] = 0x12;
    buf[1] = 0x53;
    buf[2] = (char)font;
    _serial.write(buf, 3);
}

void AS289R2::printQRCode(uint32_t err, const char* param)
{
    uint32_t len = strlen(param);
    char buf[4] = {0x1D, 0x78};
    buf[2] = err;
    buf[3] = len;
    _serial.write(buf, 4);
    _serial.write(param, len);
}

void AS289R2::printBarCode(uint32_t code, const char* param)
{
    uint32_t len = strlen(param);
    char buf[4] = {0x1D, 0x6B};
    buf[2] = code;
    buf[3] = (code & 0x0F);
    _serial.write(buf, 4);
    _serial.write(param, len);
}

void AS289R2::printBitmapImage(uint32_t mode, uint16_t lines, const uint8_t * image)
{
    char buf[3] = {0x1C, 0x2A};
    buf[2] = mode;
    _serial.write(buf, 3);

    buf[0] = (lines >> 8) & 0xFF;
    buf[1] = (lines >> 0) & 0xFF;
    _serial.write(buf, 2);

    if (mode == 0x61) {
        return;
    }

    _serial.write(image, (48 * lines));
}

void AS289R2::setLineSpaceing(uint32_t space)
{
    char buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x33;
    buf[2] = (char)space;
    _serial.write(buf, 3);
}

void AS289R2::defaultLineSpaceing()
{
    _serial.write((void *)"\x1B\x33\x04", 3);
}

void AS289R2::setPrintDirection(uint32_t direction)
{
    char buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x49;
    buf[2] = (char)direction;
    _serial.write(buf, 3);
}

void AS289R2::putPaperFeed(uint32_t space)
{
    char buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x4A;
    buf[2] = (char)space;
    _serial.write(buf, 3);
}

void AS289R2::setInterCharacterSpace(uint32_t space)
{
    char buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x20;
    buf[2] = (char)space;
    _serial.write(buf, 3);
}

void AS289R2::defaultInterCharacterSpace()
{
    _serial.write((void *)"\x1B\x20\x01", 3);
}

void AS289R2::putPrintPosition(uint32_t position)
{
    char buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x6C;
    buf[2] = (char)position;
    _serial.write(buf, 3);
}

void AS289R2::setScript(script_mode script)
{
    char buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x73;
    buf[2] = (char)script;
    _serial.write(buf, 3);
}

void AS289R2::clearScript()
{
    _serial.write((void *)"\x1B\x73\x30", 3);
}

void AS289R2::setQuadrupleSize()
{
    _serial.write((void *)"\x1C\x57\x31", 3);
}

void AS289R2::clearQuadrupleSize()
{
    _serial.write((void *)"\x1C\x57\x30", 3);
}

void AS289R2::setEnlargement(uint32_t width, uint32_t height)
{
    char buf[4];
    buf[0] = 0x1C;
    buf[1] = 0x65;
    buf[2] = (char)width;
    buf[3] = (char)height;
    _serial.write(buf, 4);
}

void AS289R2::clearEnlargement()
{
    _serial.write((void *)"\x1C\x65\x31\x31", 4);
}

void AS289R2::setBarCodeHeight(uint32_t height)
{
    char buf[3];
    buf[0] = 0x1D;
    buf[1] = 0x68;
    buf[2] = (char)height;
    _serial.write(buf, 3);
}

void AS289R2::defaultBarCodeHeight()
{
    _serial.write((void *)"\x1D\x68\x50", 3);
}

void AS289R2::setBarCodeBarSize(uint32_t narrowbar, uint32_t widebar)
{
    char buf[4];
    buf[0] = 0x1D;
    buf[1] = 0x77;
    buf[2] = (char)narrowbar;
    buf[3] = (char)widebar;
    _serial.write(buf, 4);
}

void AS289R2::defaultBarCodeBarSize()
{
    _serial.write((void *)"\x1D\x77\x02\x05", 4);
}

int AS289R2::_putc(int value)
{
    //char buf[1];
    //buf[0] = value;
    _serial.write((void *)&value, 1);
    return value;
}

int AS289R2::_getc()
{
    return -1;
}
