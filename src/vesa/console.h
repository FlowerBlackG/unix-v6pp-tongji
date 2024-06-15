// SPDX-License-Identifier: MulanPSL-2.0
// https://github.com/FlowerBlackG/YurongOS/tree/master/src/video/console/console.h

/*
 * console
 *
 * 创建于 2023年7月6日 上海市嘉定区安亭镇
 */

#pragma once

#ifdef USE_VESA

#include "sys/types.h"
#include "vesa/svga.h"

namespace video {

class Console {
public:

    const static int FONT_CH_WIDTH = 8;
    const static int FONT_CH_HEIGHT = 16;

    int32_t write(const char* buf, int32_t len = -1, int32_t color = 0xffffff);
    void init(int vMemWidth, int vMemHeight, int vMemStride, int8_t* vMem);


protected:
    struct {
        int8_t* mem;
        int width;  // npixels, not nbytes
        int height;  // npixels, not nbytes
        int stride;  // npixels, not nbytes
    } videoMem;


    int currX = 0;
    int currY = 0;

    void scrollDown(int32_t lines);
    void putchar(uint8_t ch, int32_t color);

};


namespace console {


const int32_t OUTPUT_DEFAULT_COLOR = 0xffffff;
const int32_t DIAGNOSE_DEFAULT_COLOR = 0xf8df70;

/**
 * 初始化。
 * 依赖 svga。需要先初始化 svga，再调用本函数。
 */
void init();

int32_t writeOutput(const char* buf, int32_t len = -1, int32_t color = OUTPUT_DEFAULT_COLOR);
int32_t writeDiagnose(const char* buf, int32_t len = -1, int32_t color = DIAGNOSE_DEFAULT_COLOR);

}
}

#endif
