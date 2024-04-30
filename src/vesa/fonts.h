// SPDX-License-Identifier: MulanPSL-2.0
// https://github.com/FlowerBlackG/YurongOS/tree/master/src/video/console/fonts.h

/*
 * 控制台字体。
 *
 * 创建于 2023年7月6日 上海市嘉定区安亭镇
 */

#pragma once

#ifdef USE_VESA

#include "sys/types.h"

namespace video {
namespace console {

extern uint8_t fonts[];

}
}

#endif
