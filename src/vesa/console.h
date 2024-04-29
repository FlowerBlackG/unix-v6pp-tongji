// SPDX-License-Identifier: MulanPSL-2.0
// https://github.com/FlowerBlackG/YurongOS/tree/master/src/video/console/console.h

/*
 * console
 *
 * ������ 2023��7��6�� �Ϻ��мζ�����ͤ��
 */

#pragma once

#include "sys/types.h"
#include "vesa/svga.h"

namespace video {
namespace console {

/**
 * ��ʼ����
 * ���� svga����Ҫ�ȳ�ʼ�� svga���ٵ��ñ�������
 */
void init();

int32_t write(const char* buf, int32_t len = -1, int32_t color = 0xffffff);

}
}
