/**
 ******************************************************************************
 * @file    AS289R2.h
 * @author  Toyomasa Watarai
 * @version V1.1.0
 * @date    20 January 2020
 * @brief   This file contains the class of a AS289R2 thermal control component
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

#ifndef MBED_AS289R2_H
#define MBED_AS289R2_H

#include "mbed.h"

#if defined(__CC_ARM)
// To avoid "invalid multibyte character sequence" warning
#pragma diag_suppress 870
#endif

/**  A printer interface for driving AS-289R2 thermal printer shield of NADA Electronics, Ltd.
 *
 * @code
 * #include "mbed.h"
 * #include "AS829R2.h"
 *
 * Serial pc(USBTX, USBRX);
 * AS829R2 tp(D1, D0); // tx, rx, 9600bps (default)
 *
 * int main()
 * {
 *     tp.initialize();
 *     tp.putLineFeed(2);
 *
 *     tp.printf("** Thermal Printer Shield **\r\r");
 *
 *     tp.setDoubleSizeWidth();
 *     tp.printf("  AS-289R2\r\r");
 *     tp.clearDoubleSizeWidth();
 *
 *     tp.printf("日本語文字列の印字テスト:24x24\r");
 *     tp.setKanjiFont(AS289R2::KANJI_16x16);
 *     tp.setANKFont(AS289R2::ANK_8x16);
 *     tp.printf("日本語文字列の印字テスト:16x16\r\r");
 *
 *     tp.setKanjiFont(AS289R2::KANJI_DEFAULT);
 *     tp.setANKFont(AS289R2::ANK_DEFAULT);
 *     tp.setDoubleSizeWidth();
 *     tp.printf("ABCDEFG 0123456789\r");
 *     tp.clearDoubleSizeWidth();
 *
 *     tp.setDoubleSizeHeight();
 *     tp.printf("ABCDEFG 0123456789\r");
 *     tp.clearDoubleSizeHeight();
 *
 *     pc.printf("AS-289R2 thermal printer shield test\n\n");
 *     while(1) {
 *         if (pc.readable()) {
 *             int c = pc.getc();
 *             pc.putc(c);
 *             tp.putc(c);
 *         }
 *     }
 * }
 * 
 * @endcode
 */
class AS289R2 : public Stream
{
public:

    /**
     * @enum Kanji_font_size
     * Value of Japanese Kanji font size
     */
    enum Kanji_font_size {
        //! 24x24 dot font
        KANJI_24x24 = 0x30,
        //! 16x16 dot font
        KANJI_16x16,
        //! Defalut font size
        KANJI_DEFAULT = KANJI_24x24
    };

    /**
     * @enum ANK_font_size
     * Value of ANK font size
     */
    enum ANK_font_size {
        //! 8x16 dot font
        ANK_8x16 = 0x30,
        //! 12x24 dot font
        ANK_12x24,
        //! 16x16 dot font
        ANK_16x16,
        //! 24x24 dot fot
        ANK_24x24,
        //! Defalut font size
        ANK_DEFAULT = ANK_12x24
    };

    /**
     * @enum QRcode_error_level
     * Value of CQ code error correction level
     */
    enum QRcode_error_level {
        //! Error correction level L (7%)
        QR_ERR_LVL_L = 0x4C,
        //! Error correction level M (15%)
        QR_ERR_LVL_M = 0x4D,
        //! Error correction level Q (25%)
        QR_ERR_LVL_Q = 0x51,
        //! Error correction level H (30%)
        QR_ERR_LVL_H = 0x48
    };

    /**
     * @enum barcode_mode
     * Value of barcode mode
     */
    enum barcode_mode {
        //!  UPC-A : 11-digit, d1-d11, C/D
        BCODE_UPC_A = 0x30,
        //! JAN13 : 12-digit, d1-d12, C/D
        BCODE_JAN13 = 0x32,
        //! JAN8 : 7-digit, d1-d7, C/D
        BCODE_JAN8,
        //! CODE39 : variable, d1-d20, C/D
        BCODE_CODE39,
        //! ITF : variable, d1-d20
        BCODE_ITF,
        //! CODABAR (NW7) : variable, d1-d20
        BCODE_CODABAR
    };
    
    /**
     * @enum script_mode
     * Value of script mode
     */
    enum script_mode {
        //! Cancel script mode
        SCRIPT_CANCEL = 0,
        //! Super script
        SCRIPT_SUPER,
        //! Sub script
        SCRIPT_SUB
    };

    /** Create a AS289R2 instance
     *  which is connected to specified Serial pin with specified baud rate
     *
     * @param tx Serial TX pin
     * @param rx Serial RX pin (dummy)
     * @param baud (option) serial baud rate (default: 9600bps)
     */
    AS289R2(PinName tx, PinName rx, uint32_t baud = 9600);

    /** Create a AS289R2 instance
     *  which is connected to specified Serial instance with specified baud rate
     *
     * @param serial_obj Serial object (instance)
     * @param baud (option) serial baud rate (default: 9600bps)
     */
    AS289R2(RawSerial &serial_obj, uint32_t baud = 9600);

    /** Destructor of AS289R2
     */
    virtual ~AS289R2();

    /** Initializa AS289R2
     *
     *  Issues initialize command for AS-289R2
     *
     */
    void initialize(void);

    /** Send line feed code
     *  which is connected to specified Serial pin with specified baud rate
     *
     * @param lines Number of line feed
     */
    void putLineFeed(uint32_t lines);

    /** Clear image buffer of the AS-289R2
     *
     */
    void clearBuffer(void);

    /** Set double height size font
     *
     */
    void setDoubleSizeHeight(void);

    /** Set normal height size font
     *
     */
    void clearDoubleSizeHeight(void);

    /** Set double width size font
     *
     */
    void setDoubleSizeWidth(void);

    /** Set normal width size font
     *
     */
    void clearDoubleSizeWidth(void);

    /** Set large size font (48x96)
     *
     */
    void setLargeFont(void);

    /** Set normal size font
     *
     */
    void clearLargeFont(void);

    /** Set ANK font
     *
     * @param font ANK font e.g. AS289R2::ANK_8x16
     */
    void setANKFont(uint32_t font);

    /** Set Kanji font size
     *
     * @param font Kanji font e.g. AS289R2::KANJI_16x16
     */
    void setKanjiFont(uint32_t font);

    /** Print QR code
     *
     * @param err QR code error correction level e.g. AS289R2::QR_ERR_LVL_M
     * @param buf Data to be printed
     */
    void printQRCode(uint32_t err, const char* buf);

    /** Print Bar code
     *
     * @param code Type of Bar code e.g. AS289R2::JAN13
     * @param buf Data to be printed
     */
    void printBarCode(uint32_t code, const char* param);

    /** Print bitmap image
     *
     * @param cmd Type of operation mode, 0x61: print image buffer, 0x62: register image buffer, 0x63: register -> print, 0x64: print -> register, 0x65: line print
     * @param lines Number of print line
     * @param image Data to be printed
     */
    void printBitmapImage(uint32_t cmd, uint16_t lines, const uint8_t * image);

    /** Set Line Spaceing
     *
     * @param space line spacing
     */
    void setLineSpaceing(uint32_t space);

    /** Set as default Line Spaceing
     *
     */
    void defaultLineSpaceing(void);

    /** Set Print Direction
     *
     * @param direction Print direction, 0: lister, 1: texter
     */
    void setPrintDirection(uint32_t direction);

    /** Send feed code
     *
     * @param space Paper feed
     */
    void putPaperFeed(uint32_t space);

    /** Set Inter Character Space
     *
     * @param space inter-character space
     */
    void setInterCharacterSpace(uint32_t space);

    /** Set as default Inter Character Space
     *
     */
    void defaultInterCharacterSpace(void);

    /** Send Print Position
     *
     * @param position Print position
     */
    void putPrintPosition(uint32_t position);

    /** Set Script
     *
     * @param script mode e.g. AS289R2::SCRIPT_SUPER
     */
    void setScript(script_mode script);

    /** Clear Script
     *
     */
    void clearScript(void);

    /** Set Quadruple size
     *
     */
    void setQuadrupleSize(void);

    /** Clear Quadruple size
     *
     */
    void clearQuadrupleSize(void);

    /** Set Enlargement size
     *
     * @param width enlargement
     * @param height enlargement
     */
    void setEnlargement(uint32_t width, uint32_t height);

    /** Clear Enlargement size
     *
     */
    void clearEnlargement(void);

    /** Set BarCode Height size
     *
     * @param height Bar height
     */
    void setBarCodeHeight(uint32_t height);

    /** Set as default BarCode Height size
     *
     */
    void defaultBarCodeHeight(void);

    /** Set BarCode Bar size
     *
     * @param narrowbar narrow bars size
     * @param widebar wide bars size
     */
    void setBarCodeBarSize(uint32_t narrowbar, uint32_t widebar);

    /** Set as default BarCode Bar size
     *
     */
    void defaultBarCodeBarSize(void);

private:
    RawSerial *_serial_p;
    RawSerial &_serial;

protected:
    // Stream implementation functions
    virtual int _putc(int value);
    virtual int _getc();
};

#endif
