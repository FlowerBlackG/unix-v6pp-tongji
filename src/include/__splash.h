// 2051565 GTY
// created on 2024.07.19

// this is a bad way but can run.

#pragma once

#include "../libyrosstd/sys/types.h"

typedef struct {
    int32_t width;
    int32_t height;
    int32_t stride; // in bytes.

    char buf[1920000];  // change this when splash image changes.

} __packed __v6pptty_draw_splash_datapack;

