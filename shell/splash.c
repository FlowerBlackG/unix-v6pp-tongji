// 2051565 GTY
// created on 2024.7.12, on a Fuxing G160 train

#include "sys.h"
#include "file.h"

static const char* SPLASH_BMP = "/etc/v6pp_splash.bmp";


typedef struct {
    // todo
} BmpImage;


static int drawImg(const char* filePath) {
    int file = open(filePath, 0111);

    if (file == -1) {
        return -1;
    }

    // todo


    return 0;
}


int splash() {
    __v6pptty_clear(0);

    drawImg(SPLASH_BMP);

    sleep(3);
    __v6pptty_clear(0);

    return 0;
}