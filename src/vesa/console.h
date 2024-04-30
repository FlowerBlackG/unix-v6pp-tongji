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
