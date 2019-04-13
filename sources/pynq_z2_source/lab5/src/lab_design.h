/*
 * lab_design.h
 *
 *  Created on: May 28, 2015
 *      Author: shaunpur
 */

#ifndef LAB_DESIGN_H_
#define LAB_DESIGN_H_

/* includes */
#include <inttypes.h>

#define BLUE_MASK   0x000000FF
#define GREEN_MASK  0x0000FF00
#define RED_MASK    0x00FF0000

// pull out the color component and return as a value between 0 and 255
#define getBlue(colorPixel)  (colorPixel & BLUE_MASK)
#define getGreen(colorPixel) (colorPixel & GREEN_MASK) >> 8
#define getRed(colorPixel)   (colorPixel & RED_MASK) >> 16

#define setBlue(existingPixel,blueComponent)   ((existingPixel & ~BLUE_MASK)  | blueComponent)
#define setGreen(existingPixel,greenComponent) ((existingPixel & ~GREEN_MASK) | (greenComponent << 8))
#define setRed(existingPixel,redComponent)     ((existingPixel & ~RED_MASK)   | (redComponent << 16))

/* define types and structures */
#define FRAME_WIDTH  1920
#define FRAME_HEIGHT 1080
#define WIDOW_WIDTH     3
#define WIDOW_HEIGHT    3

#define COLOR_CHANNEL_RED 0
#define COLOR_CHANNEL_GREEN 1
#define COLOR_CHANNEL_BLUE 2
#define COLOR_CHANNELS 3

typedef union pixel {
	struct {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};
	struct {
		uint8_t gray;
	};
} pixel_t;

typedef struct frame {
	uint32_t height;
	uint32_t width;
	uint8_t *colorFrame; //Multiply by COLOR_CHANNELS for each color channel, RGB
	uint8_t *grayFrame;
} frame_t;

#endif /* LAB_DESIGN_H_ */
