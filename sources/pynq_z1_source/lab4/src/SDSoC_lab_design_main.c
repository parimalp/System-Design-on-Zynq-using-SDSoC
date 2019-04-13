/*
 * *************************************************************************************************************
 *
 * *** SDSoC_lab_design
 *
 * Contains main() and 3 functions
 *    rgb2gray()   - color to gray scale -
 *    sharpen()    - sharpens image -
 *    edgeDetect() - edge detection - uses Sobel edge detection (filter) *tentative*
 *
 * *** definitions
 *    pixels - RGB value - 8 bits per color for a total of 24 bits
 *    window - (kernel) - small region around current pixel, typically 3x3
 *    frame  - full image
 *
 * ***
 *
 * History:
 *    V0.2 - 07/15/2016 - sc			 - removed small color array allocation bug (it was oversized by 4 times)
 *    V0.2 - 06/12/2015 - sc, wk         - performance improvements through coding style, cleanup for release
 *    V0.1 - 05/27/2015 - sp, sm, sk, wk - initial version
 *
 * *************************************************************************************************************
 *
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <sds_lib.h>
#include "lab_design.h"
#include "rgb_2_gray.h"
#include "sharpen.h"
#include "edge_detect.h"

/* globals */
unsigned long long sw_sds_counter_total[4]     = { 0, 0, 0, 0 };
unsigned int       sw_sds_counter_num_calls[4] = { 0, 0, 0, 0 };
unsigned long long sw_sds_counter[4]           = { 0, 0, 0, 0 };

/* timing related macros */
#define sw_sds_clk_start(IDX) { sw_sds_counter[(IDX)] = sds_clock_counter(); sw_sds_counter_num_calls[(IDX)]++; }
#define sw_sds_clk_stop(IDX) { unsigned long long tmp = sds_clock_counter(); sw_sds_counter_total[(IDX)] += ((tmp < sw_sds_counter[(IDX)]) ? (sw_sds_counter[(IDX)] - tmp): (tmp - sw_sds_counter[(IDX)])); }
#define sw_avg_cpu_cycles(IDX) (sw_sds_counter_total[(IDX)] / sw_sds_counter_num_calls[(IDX)])
#define RGB2GRAY		0
#define SHARPEN			1
#define EDGE_DETECT		2
#define WHOLE_PROCESS   3
#define LOOPS			5

/* function prototypes */
void dummyfill(uint32_t* f);
void dummyprint_color(uint32_t* f);
void dummyprint_gray(uint8_t* f);


/*
 * *** main()
 *
 * configures data structures, run frame processing
 *
 */
int main(int argc, char* argv[]) {
	// local variables
	int i;

	// set up memory structures for moving frames of data
	uint32_t *array_c = (uint32_t*) sds_alloc(FRAME_HEIGHT * FRAME_WIDTH * sizeof(uint32_t));	// color frames are 24 bits per pixel, 32 bits is the smallest un-packed type we can fit it into
	uint8_t *array_g_1 = (uint8_t*) sds_alloc(FRAME_HEIGHT * FRAME_WIDTH * sizeof(uint8_t));					// gray frames are 8 bits per pixel
	uint8_t *array_g_2 = (uint8_t*) sds_alloc(FRAME_HEIGHT * FRAME_WIDTH * sizeof(uint8_t));
	uint8_t *array_g_3 = (uint8_t*) sds_alloc(FRAME_HEIGHT * FRAME_WIDTH * sizeof(uint8_t));

	// let the user know what is happening...
	printf("Running frame operations...\n");

	// initialize the color frame buffer (initial input into the processesing sequence
	dummyfill(array_c);

	/* process image */
	sw_sds_clk_start(WHOLE_PROCESS);
	for (i = LOOPS; i != 0; i--) {

#ifdef TIME_RGB2GRAY
   sw_sds_clk_start(RGB2GRAY);
#endif
		rgb_2_gray(array_c, array_g_1);
#ifdef TIME_RGB2GRAY
		sw_sds_clk_stop(RGB2GRAY);
#endif
#ifdef TIME_SHARPEN
   sw_sds_clk_start(SHARPEN);
#endif
		sharpen_filter(array_g_1, array_g_2);
#ifdef TIME_SHARPEN
		sw_sds_clk_stop(SHARPEN);
#endif
#ifdef TIME_EDGE_DETECT
   sw_sds_clk_start(EDGE_DETECT);
#endif
		sobel_filter(array_g_2, array_g_3);
#ifdef TIME_EDGE_DETECT
		sw_sds_clk_stop(EDGE_DETECT);
#endif

		printf(".\n");
	}
	sw_sds_clk_stop(WHOLE_PROCESS);

	// manage dynamically allocated memory to avoid memory leaks
	//BUG in the tools??? Enabling these will stop the optimization of the above hardware methods and not join them together!
	//sds_free(array_g_3);
	//sds_free(array_g_2);
	//sds_free(array_g_1);
	//sds_free(array_c);

	/* display time */
	printf("Average SW cycles for all of the image functions:    %llu\n",sw_avg_cpu_cycles(WHOLE_PROCESS));
#ifdef TIME_RGB2GRAY
	printf("Average SW cycles for RGB to Gray: %llu\n",sw_avg_cpu_cycles(RGB2GRAY));
#endif
#ifdef TIME_SHARPEN
	printf("Average SW cycles for sharpen:     %llu\n",sw_avg_cpu_cycles(SHARPEN));
#endif
#ifdef TIME_EDGE_DETECT
	printf("Average SW cycles for edge_detect: %llu\n",sw_avg_cpu_cycles(EDGE_DETECT));
#endif

	return 0;
}


/*** supporting functions ***/
// fills a color frame with a constant color
void dummyfill(uint32_t* f) {
	int i, j;
	for (i = 0; i < FRAME_HEIGHT; i++) {						// loop through all rows
		for (j = 0; j < FRAME_WIDTH; j++) {						// and columns
			int index = (i * FRAME_WIDTH + j);					// compute the offset into the array based on row and column
			pixel_t p = { { 10, 20, 30 } };						// select a color
			f[index] = p.red << 16 | p.green << 8 | p.blue;		// and shift each color into its proper position
		}
	}
}

// dump the values of the color buffer to the screen as r,g,b components
void dummyprint_color(uint32_t* f) {
	int i, j;
	for (i = 0; i < FRAME_HEIGHT; i++) {						// loop through all rows
		for (j = 0; j < FRAME_WIDTH; j++) {						// loop through all columns
			int index = (i * FRAME_WIDTH + j);					// compute the offset into the array based on row and column
			int pixel = f[index];								// get the specific pixel from the frame
			pixel_t p;											// definition of a pixel
			p.red = (pixel >> 16);								// pull the red component from the pixel and place in lowest 8 bits (0-255)
			p.green = (pixel >> 8) & 0x000000ff;				// pull the green component from the pixel and place it in lowest 8 bits (0-255)
			p.blue = (pixel) & 0x000000ff;						// pull the blue component from the pixel and place it in the lowest 8 bits (0-255)
			printf("[%d,%d,%d]", p.red, p.green, p.blue);		// print as [r,g,b]
		}
		printf("\n");											// provide a new line at the end of the row
	}
	printf("\n");												// place a new line at the end of a frame
}

// dump the values of the gray buffer to the screen (as an integer)
void dummyprint_gray(uint8_t* f) {
	int i, j;
	for (i = 0; i < FRAME_HEIGHT; i++) {						// loop through all rows
		for (j = 0; j < FRAME_WIDTH; j++) {						// loop through all columns
			uint8_t gray_pixel = f[i * FRAME_WIDTH + j];		// compute the offset into the array based on row and column and get the pixel from the buffer
			printf("[%d]", gray_pixel);							// display the value of the pixel [g]
		}
		printf("\n");											// provide a new line at the end of a row
	}
	printf("\n");												// provide a new line at the end of a frame
}
