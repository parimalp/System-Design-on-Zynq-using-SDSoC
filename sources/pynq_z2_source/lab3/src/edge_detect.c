/*
 * edge_detect.c
 *
 *  Created on: May 29, 2015
 *      Author: shaunpur
 */

#include "lab_design.h"
#include "edge_detect.h"

/* macros */
#define ABSDIFF(x,y)    ((x>y)? x - y : y - x)
#define ABS(x)          ((x>0)? x : -x)
#define LBUF_HEIGHT		WIDOW_HEIGHT
#define NUMROWS			FRAME_HEIGHT
#define NUMCOLS			FRAME_WIDTH
#define BYTESHIFT		0
#define BLACK_VAL		0

/* define types and structures */
typedef uint8_t window_t[WIDOW_HEIGHT][WIDOW_WIDTH];
typedef uint8_t linebuffer_t[LBUF_HEIGHT][FRAME_WIDTH];

/* function prototypes */
static unsigned char sobel_operator(window_t window);
static void linebuffer_shift_up(linebuffer_t M, int col);
static uint8_t linebuffer_getval(linebuffer_t M, int RowIndex, int ColIndex);
static void linebuffer_insert_bottom(linebuffer_t M, uint8_t value, int col);
static void window_shift_right(window_t M);
static void window_insert(window_t M, uint8_t value, int row, int col);
static uint8_t window_getval(window_t M, int RowIndex, int ColIndex);

//Sobel Computation using a 3x3 neighborhood
static unsigned char sobel_operator(window_t window) {
	short x_weight = 0;
	short y_weight = 0;

	short edge_weight;
	unsigned char edge_val;

	int i;
	int j;

	const short x_op[WIDOW_HEIGHT][WIDOW_WIDTH] = { { -1, 0, 1 }, { -2, 0, 2 },
			{ -1, 0, 1 } };

	const short y_op[WIDOW_HEIGHT][WIDOW_WIDTH] = { { 1, 2, 1 }, { 0, 0, 0 }, {
			-1, -2, -1 } };

	//Compute approximation of the gradients in the X-Y direction
	for (i = 0; i < WIDOW_HEIGHT; i++) {
		for (j = 0; j < WIDOW_WIDTH; j++) {

			// X direction gradient
			x_weight = x_weight + (window_getval(window, i, j) * x_op[i][j]);

			// Y direction gradient
			y_weight = y_weight + (window_getval(window, i, j) * y_op[i][j]);
		}
	}

	edge_weight = ABS(x_weight) + ABS(y_weight);

	//Edge thresholding
	if (edge_weight > 200)
		edge_weight = 255;
	else if (edge_weight < 100)
		edge_weight = 0;
	edge_val = (unsigned char) edge_weight;

	return edge_val;
}

//Main function for Sobel Filtering
//This function includes a line buffer for a streaming implementation
void sobel_filter(uint8_t input[2073600], uint8_t output[2073600]) {

	int row;
	int col;
	int index = 0;

	linebuffer_t buff_A;
	window_t buff_C;

	for (row = 0; row < NUMROWS + 1; row++) {
		for (col = 0; col < NUMCOLS + 1; col++) {

#pragma AP PIPELINE II = 1

			// Temp values are used to reduce the number of memory reads
			unsigned short input_data = 0;
			unsigned char temp = 0;
			unsigned char tempx = 0;

			//Line Buffer fill
			if (col < NUMCOLS) {
				linebuffer_shift_up(buff_A, col);
				temp = linebuffer_getval(buff_A, 0, col);
			}

			//There is an offset to accomodate the active pixel region
			//There are only NUMCOLS and NUMROWS valid pixels in the image
			if ((col < NUMCOLS) & (row < NUMROWS)) {
				unsigned char y;
				index = row * NUMCOLS + col;
				input_data = input[index]; //[row*NUMCOLS+col];
				y = input_data >> BYTESHIFT;
				tempx = y;
				linebuffer_insert_bottom(buff_A, tempx, col);
			}

			//Shift the processing window to make room for the new column
			window_shift_right(buff_C);

			//The Sobel processing window only needs to store luminance values
			//rgb2y function computes the luminance from the color pixel
			if (col < NUMCOLS) {
				window_insert(buff_C, linebuffer_getval(buff_A, 2, col), 0, 2);
				window_insert(buff_C, temp, 1, 2);
				window_insert(buff_C, tempx, 2, 2);
			}

			unsigned char edge;
			//The sobel operator only works on the inner part of the image
			//This design assumes there are no edges on the boundary of the image
			//Sobel operation on the inner portion of the image
			edge = sobel_operator(buff_C);


			//The output image is offset from the input to account for the line buffer
			if (row > 0 && col > 0) {
				index = (row - 1) * NUMCOLS + (col - 1);
				output[index] = (edge << BYTESHIFT) | BLACK_VAL;
			}
		}
	}
}

/* Line buffer shift up
 * Assumes new data pixel will be entered at the bottom of the line buffer
 * The bottom is row = 0
 */
static void linebuffer_shift_up(linebuffer_t M, int col) {
#pragma AP inline
	int i;
	for (i = LBUF_HEIGHT - 1; i > 0; i--) {
#pragma AP unroll
		M[i][col] = M[i - 1][col];
	}
}

/* Line buffer getval
 * Returns the data value in the line buffer at position RowIndex, ColIndex
 */
static uint8_t linebuffer_getval(linebuffer_t M, int RowIndex, int ColIndex) {
#pragma AP inline

	uint8_t return_value;
	return_value = M[RowIndex][ColIndex];
	return return_value;
}

/* Line buffer insert bottom
 * Inserts a new value in the bottom row of the line buffer at column = col
 * The bottom is row = 0
 */
static void linebuffer_insert_bottom(linebuffer_t M, uint8_t value, int col) {
#pragma AP inline

	M[0][col] = value;
}

/* Window shift right
 * Moves all the contents of the window horizontally
 * Assumes new values will be placed in column = WIDOW_WIDTH-1
 */
static void window_shift_right(window_t M) {
#pragma AP inline
	int i, j;
	for (i = 0; i < WIDOW_HEIGHT; i++) {
#pragma AP unroll
		for (j = 0; j < WIDOW_WIDTH - 1; j++) {
#pragma AP unroll
			M[i][j] = M[i][j + 1];
		}
	}
}

/* Window insert
 * Inserts a new value at any location of the window
 */
static void window_insert(window_t M, uint8_t value, int row, int col) {
#pragma AP inline
	M[row][col] = value;
}

/* Window getval
 * Returns the value of any window location
 */
static uint8_t window_getval(window_t M, int RowIndex, int ColIndex) {
#pragma AP inline
	uint8_t return_value;
	return_value = M[RowIndex][ColIndex];
	return return_value;
}
