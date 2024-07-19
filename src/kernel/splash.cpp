// 2051565 GTY
// created on 2024.7.12, on a Fuxing G160 train

// https://www.wikiwand.com/en/BMP_file_format


#ifdef ENABLE_SPLASH

#include "../libyrosstd/sys/types.h"
#include "../vesa/svga.h"
#include "Lib.h"
#include "Video.h"
#include "Kernel.h"


static const char* SPLASH_BMP = "/etc/v6pp_splash.bmp";


struct BmpFileHeader {
    int8_t magic[2]; // 'B' 'M' for Windows series.
    int32_t fileSize; // in bytes
    int16_t reserved0;
    int16_t reserved1;
    int32_t offset; // starting addr of the byte where img data lies.

} __packed ;


struct BmpInfoHeader {
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

} __packed ;


static int drawImg(const char* filePath) {
    int file = lib_open(filePath, 0111);

    if (file == -1) {
        return -1;
    }

    BmpFileHeader fileHeader;

    lib_seek(file, 0, 0);
    lib_read(file, (char*) &fileHeader, sizeof(fileHeader));
    if (fileHeader.magic[0] != 'B' || fileHeader.magic[1] != 'M') {
        Diagnose::Write("splash: bad magic!\n");
        return -1;
    }



    BmpInfoHeader infoHeader;
    lib_read(file, (char*) &infoHeader, sizeof(infoHeader));



    // todo: validate fields.

    auto& kpm = Kernel::Instance().GetKernelPageManager();

    intptr_t buf = kpm.AllocMemory(infoHeader.imageSize);
    if (!buf) {
        Diagnose::Write("splash: failed to alloc memory for splash buffer!\n");
        return -1;
    }

    buf += 0xC0000000;

    char* imgData = (char*) buf;
    if (!imgData) {
        Diagnose::Write("splash: failed to alloc image data buffer!\n");
        return -2;
    }

    lib_seek(file, fileHeader.offset, 0);
    lib_read(file, imgData, infoHeader.imageSize);
    
    
	for (int h = infoHeader.height - 1; h >= 0; h--) {
        for (int w = 0; w < infoHeader.width; w++) {
			auto pPixel = (int*) (imgData + (w + h * infoHeader.width) * 4);
			int color = *pPixel;
			video::svga::putPixel(w, infoHeader.height - h, color);
		}
	}


    kpm.FreeMemory(infoHeader.imageSize, buf - 0xC0000000);

    // todo


    return 0;
}


int splash() {
    video::svga::clear();

    drawImg(SPLASH_BMP);

    lib_sleep(1);
    video::svga::clear();

    return 0;
}


#endif
