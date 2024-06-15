// SPDX-License-Identifier: MulanPSL-2.0
// https://github.com/FlowerBlackG/YurongOS/tree/master/src/video/console/console.cpp

/*
 * console
 *
 * 创建于 2023年7月6日 上海市嘉定区安亭镇
 */

#ifdef USE_VESA

#include "./console.h"
#include "./fonts.h"
#include "./svga.h"
#include "AsciiChar.h"

#include <libyrosstd/string.h>

#define CH_NROWS(vMemHeight) (vMemHeight / Console::FONT_CH_HEIGHT)
#define CH_NCOLS(vMemWidth) (vMemWidth / Console::FONT_CH_WIDTH)


namespace video {


/* ------------ class Console ------------ */

struct {
    Console output;
    Console diagnose;
} consoles;

void Console::init(int vMemWidth, int vMemHeight, int vMemStride, int8_t* vMem) {
    this->videoMem.height = vMemHeight;
    this->videoMem.width = vMemWidth;
    this->videoMem.stride = vMemStride;
    this->videoMem.mem = vMem;
}


int32_t Console::write(const char* buf, int32_t len, int32_t color) {

    // todo: writeMutex.lock();

    const char* p = buf;
    while ((len < 0 || p - buf < len) && *p != AsciiChar::NUL) {

        // 对于 \n，如果前一个不是 \r，则补充输出 \r。
        if (*p == AsciiChar::LF && ((p > buf && *(p - 1) != AsciiChar::CR) || p == buf)) {
            putchar(AsciiChar::CR, color);
        }

        putchar(*p, color);
        ++p;

    }

    // todo: writeMutex.unlock();

    return p - buf;

}


void Console::scrollDown(int32_t lines) {
    if (lines <= 0) {
        return;
    }

    int8_t* screen = this->videoMem.mem;

    if (lines >= CH_NROWS(videoMem.height)) {
        
        for (int i = 0; i < videoMem.height; i++) {
            memset(
                screen + i * videoMem.stride * svga::bytesPerPixel, 
                0, 
                videoMem.width * svga::bytesPerPixel
            );
        }
        

        currY = currX = 0;

    } else {

        
        for (int i = 0; i < videoMem.height - lines * FONT_CH_HEIGHT; i++) {
            memcpy(
                screen + i * videoMem.stride * svga::bytesPerPixel,
                screen + (i + lines * FONT_CH_HEIGHT) * videoMem.stride * svga::bytesPerPixel,
                videoMem.width * svga::bytesPerPixel
            );
        }

        for (int i = 0; i < lines * FONT_CH_HEIGHT; i++) {
            memset(
                screen 
                    + videoMem.height * videoMem.stride * svga::bytesPerPixel 
                    - lines * FONT_CH_HEIGHT * videoMem.stride * svga::bytesPerPixel
                    + i * videoMem.stride * svga::bytesPerPixel,
                0,
                videoMem.width * svga::bytesPerPixel
            );
        }
        

        currY -= lines;

    }
}


void Console::putchar(uint8_t ch, int32_t color) {
        if (ch >= AsciiChar::START_OF_VISIBLE_CHARS && ch <= AsciiChar::END_OF_VISIBLE_CHARS) {

        int32_t idx = ch - ' ';
        for (int8_t row = 0; row < FONT_CH_HEIGHT; row++) {
            auto byte = console::fonts[idx * FONT_CH_HEIGHT + row];
            for (int8_t col = 0; col < FONT_CH_WIDTH; col++) {

                auto targetAddr = videoMem.mem;
                targetAddr += svga::bytesPerPixel * (row + currY * FONT_CH_HEIGHT) * videoMem.stride;
                targetAddr += svga::bytesPerPixel * (col + currX * FONT_CH_WIDTH);
                auto pixelColor = (byte & (1 << (FONT_CH_WIDTH - col)) ? color : 0);
                targetAddr[0] = pixelColor & 0xff;
                targetAddr[1] = (pixelColor >> 8) & 0xff;
                targetAddr[2] = pixelColor >> 16;

            }
        }

        currX++;
        if (currX >= CH_NCOLS(videoMem.width)) {
            currX = 0;
            currY++;
        }

        if (currY >= CH_NROWS(videoMem.height)) {
            scrollDown(1);
        }

    } else switch (ch) {
        case AsciiChar::CR: { // \r
            currX = 0;
            break;
        }

        case AsciiChar::LF: { // \n
            if (currY + 1 == CH_NROWS(videoMem.height)) {
                scrollDown(1);
            }

            currY++;

            break;
        }

        case AsciiChar::TAB: { // \t

            auto oldX = currX;
            auto posX = oldX;
            posX &= 0xFFFFFFF8;	/* 向左对齐到前一个Tab边界 */
            posX += 8;
            
            if ( posX >= CH_NCOLS(videoMem.width) )
                write("\n", -1, color);
            else {
                while (posX -- > oldX) {
                    putchar(' ', color);
                }
            }

            break;
        }

        case AsciiChar::BS: { // \b, backspace
            // todo: back to prev line is not currently supported.
            if (currX == 0) {
                break;
            }

            currX--;
            putchar(' ', color);
            currX--;

            break;
        }

        default: {
            break;
        }
    }

}


/* ------------ namespace console ------------ */

namespace console {

void init() {
    auto vbeInfo = svga::modeInfo;

    consoles.output.init(
        vbeInfo->width,
        vbeInfo->height / 2,
        vbeInfo->width,
        svga::screen
    );

    consoles.diagnose.init(
        vbeInfo->width,
        vbeInfo->height / 2,
        vbeInfo->width,
        svga::screen + (vbeInfo->height / 2) * vbeInfo->width * svga::bytesPerPixel
    );
}


int32_t writeOutput(const char* buf, int32_t len, int32_t color) {
    return consoles.output.write(buf, len, color);
}


int32_t writeDiagnose(const char* buf, int32_t len, int32_t color) {
    return consoles.diagnose.write(buf, len, color);
}


}
}

#endif
