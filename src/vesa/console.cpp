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

namespace video {
namespace console {

struct ConsoleDisplayInfo {
    int32_t yOffset = 0;
    int32_t currentX = 0;
    int32_t currentY = 0;
    int32_t xLimit = 0;
    int32_t yLimit = 0;
};

static struct {
    ConsoleDisplayInfo diagnose;
    ConsoleDisplayInfo output;
} displays;


// todo: static concurrent::Mutex writeMutex;

void init() {
    auto vbeInfo = svga::modeInfo;
    auto xLimit = vbeInfo->width / 8;
    auto yLimit = vbeInfo->height / 16;
    displays.diagnose.yOffset = displays.diagnose.yLimit = displays.output.yLimit = yLimit / 2;

    displays.diagnose.xLimit = displays.output.xLimit = xLimit;
}

static inline void scrollDown(int32_t lines, ConsoleDisplayInfo& display) {
    if (lines <= 0) {
        return;
    }

    auto vbeInfo = svga::modeInfo;

    int8_t* screen = svga::screen + svga::bytesPerPixel * 8 * 16 * display.xLimit * display.yOffset;

    if (lines >= display.yLimit) {
        memset(
            screen,
            0,
            svga::bytesPerPixel * 8 * 16 * display.xLimit * display.yLimit
        );
        display.currentY = 0;

    } else {

        memcpy(
            screen,
            (void*) (intptr_t(screen) + vbeInfo->pitch * lines * 16),
            vbeInfo->height * vbeInfo->pitch / 2 /* todo: 开摆！ */ - vbeInfo->pitch * lines * 16
        );

        memset(
            (void*) (intptr_t(screen) + vbeInfo->height * vbeInfo->pitch / 2 /* todo: 开摆！ */ - vbeInfo->pitch * lines * 16),
            0,
            vbeInfo->pitch * lines * 16
        );
        display.currentY -= lines;

    }
}

// 前置声明。
static int32_t write(const char* buf, int32_t len, int32_t color, ConsoleDisplayInfo& display);


static inline void putchar(uint8_t ch, int32_t color, ConsoleDisplayInfo& display) {

    if (ch >= AsciiChar::START_OF_VISIBLE_CHARS && ch <= AsciiChar::END_OF_VISIBLE_CHARS) {

        int32_t idx = ch - ' ';
        for (int8_t row = 0; row < 16; row++) {
            auto byte = fonts[idx * 16 + row];
            for (int8_t col = 0; col < 8; col++) {

                svga::putPixel(
                    display.currentX * 8 + col,
                    (display.currentY + display.yOffset) * 16 + row,
                    (byte & (1 << (8 - col)) ? color : 0)
                );

            }
        }

        display.currentX++;
        if (display.currentX >= display.xLimit) {
            display.currentX = 0;
            display.currentY++;
        }

        if (display.currentY >= display.yLimit) {
            scrollDown(1, display);
        }

    } else switch (ch) {
        case AsciiChar::CR: { // \r
            display.currentX = 0;
            break;
        }

        case AsciiChar::LF: { // \n
            if (display.currentY + 1 == display.yLimit) {
                scrollDown(1, display);
            }

            display.currentY++;

            break;
        }

        case AsciiChar::TAB: { // \t

            auto oldX = display.currentX;
            auto posX = oldX;
            posX &= 0xFFFFFFF8;	/* 向左对齐到前一个Tab边界 */
            posX += 8;
            
            if ( posX >= display.xLimit )
                write("\n", -1, color, display);
            else {
                while (posX -- > oldX) {
                    putchar(' ', color, display);
                }
            }

            break;
        }

        case AsciiChar::BS: { // \b, backspace
            // todo: back to prev line is not currently supported.
            if (display.currentX == 0) {
                break;
            }

            display.currentX--;
            putchar(' ', color, display);
            display.currentX--;

            break;
        }

        default: {
            break;
        }
    }

}

static int32_t write(const char* buf, int32_t len, int32_t color, ConsoleDisplayInfo& display) {

    // todo: writeMutex.lock();

    const char* p = buf;
    while ((len < 0 || p - buf < len) && *p != AsciiChar::NUL) {

        // 对于 \n，如果前一个不是 \r，则补充输出 \r。
        if (*p == AsciiChar::LF && ((p > buf && *(p - 1) != AsciiChar::CR) || p == buf)) {
            putchar(AsciiChar::CR, color, display);
        }

        putchar(*p, color, display);
        ++p;

    }

    // todo: writeMutex.unlock();

    return p - buf;
}


int32_t writeOutput(const char* buf, int32_t len, int32_t color) {
    return write(buf, len, color, displays.output);
}

int32_t writeDiagnose(const char* buf, int32_t len, int32_t color) {
    return write(buf, len, color, displays.diagnose);
}


}
}

#endif
