#include "lab_design.h"
#include "rgb_2_gray.h"

//Reference https://www.mathworks.com/help/matlab/ref/rgb2gray.html for
//the values
void rgb_2_gray(uint32_t color[2073600], uint8_t gray[2073600]) {

	int i, j;
	int index;
	uint32_t red, green, blue;
	uint32_t thisPixel;

	for (i = 0; i < FRAME_HEIGHT; i++) {
		for (j = 0; j < FRAME_WIDTH; j++) {
#pragma AP PIPELINE II = 1
			index = (i * FRAME_WIDTH + j);

			thisPixel = color[index];
			red = (uint32_t) getRed(thisPixel);
			green = (uint32_t) getGreen(thisPixel);
			blue = (uint32_t) getBlue(thisPixel);

			uint16_t gr = (uint16_t)((red * 298 + green * 587 + blue * 114) >> 10); //Divide by 1024 (should be 1000 but not using floating point)

			gray[index] = (uint8_t) gr;
		}
	}

}

