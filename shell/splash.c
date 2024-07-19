// 2051565 GTY
// created on 2024.7.12, on a Fuxing G160 train

// https://www.wikiwand.com/en/BMP_file_format

#include "sys.h"
#include "file.h"
#include "stdio.h"
#include "malloc.h"

// this header doesn't contains symbols to be linked.
// bad method, but it actually can be imported directly.
#include "../src/libyrosstd/sys/types.h"


static const char* SPLASH_BMP = "/etc/v6pp_splash.bmp";


typedef struct {
    int8_t magic[2]; // 'B' 'M' for Windows series.
    int32_t fileSize; // in bytes
    int16_t reserved0;
    int16_t reserved1;
    int32_t offset; // starting addr of the byte where img data lies.

} __packed BmpFileHeader;


typedef struct {
    int32_t headerSize; // should be 40.
    int32_t width; // in pixels.
    int32_t height; // in pixels.
    int16_t numberOfColorPlanes; // should be 1.
    int16_t bitsPerPixel;
    int32_t compressionMethod;
    int32_t imageSize;
    int32_t horizontalResolution;
    int32_t verticalResolution;
    int32_t numberOfColorsInColorPalette;
    int32_t numberOfImportantColors;

} __packed BmpInfoHeader;


#include "../src/include/__splash.h"  // bad way. but can run.


__v6pptty_draw_splash_datapack syscallData;

static int drawImg(const char* filePath) {
    int file = open(filePath, 0111);

    if (file == -1) {
        return -1;
    }

    BmpFileHeader fileHeader;

    seek(file, 0, 0);
    read(file, (void*) &fileHeader, sizeof(fileHeader));
    if (fileHeader.magic[0] != 'B' || fileHeader.magic[1] != 'M') {
        printf("splash: bad magic!\n");
        return -1;
    }

    printf("file size: %d\n", fileHeader.fileSize);

    BmpInfoHeader infoHeader;
    read(file, (void*) &infoHeader, sizeof(infoHeader));

    printf("width: %d\n", infoHeader.width);
    printf("height: %d\n", infoHeader.height);
    printf("compress: %d\n", infoHeader.compressionMethod);
    printf("img size: %d\n", infoHeader.imageSize);
    printf("bits pp: %d\n", infoHeader.bitsPerPixel);

    // todo: validate fields.

    char* imgData = syscallData.buf;
    syscallData.width = infoHeader.width;
    syscallData.height = infoHeader.height;
    syscallData.stride = infoHeader.width * 4;
    if (!imgData) {
        printf("splash: failed to alloc image data buffer!\n");
        return -2;
    }

    seek(file, fileHeader.offset, 0);
    read(file, imgData, infoHeader.imageSize);
    
    __v6pptty_draw_splash((int) &syscallData);

    // todo


    return 0;
}


int splash() {
    __v6pptty_clear(0);

    drawImg(SPLASH_BMP);

    //sleep(1);
    __v6pptty_clear(0);

    return 0;
}
