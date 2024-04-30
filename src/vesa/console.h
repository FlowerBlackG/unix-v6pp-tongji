// SPDX-License-Identifier: MulanPSL-2.0
// https://github.com/FlowerBlackG/YurongOS/tree/master/src/video/console/console.h

/*
 * console
 *
 * ������ 2023��7��6�� �Ϻ��мζ�����ͤ��
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
 * ��ʼ����
 * ���� svga����Ҫ�ȳ�ʼ�� svga���ٵ��ñ�������
 */
void init();

int32_t writeOutput(const char* buf, int32_t len = -1, int32_t color = OUTPUT_DEFAULT_COLOR);
int32_t writeDiagnose(const char* buf, int32_t len = -1, int32_t color = DIAGNOSE_DEFAULT_COLOR);

}
}

#endif
