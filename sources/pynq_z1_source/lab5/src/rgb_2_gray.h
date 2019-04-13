/*
 * rgb2gray.h
 *
 *  Created on: May 28, 2015
 *      Author: shaunpur
 */

#ifndef RGB2GRAY_H_
#define RGB2GRAY_H_

/* function prototypes */
#pragma SDS data access_pattern(color:SEQUENTIAL, gray:SEQUENTIAL)
void rgb_2_gray(uint32_t color[2073600], uint8_t gray[2073600]);


#endif /* RGB2GRAY_H_ */

/*
 #pragma SDS data mem_attribute (color:PHYSICAL_CONTIGUOUS|NON_CACHEABLE,gray:PHYSICAL_CONTIGUOUS|NON_CACHEABLE)
 with
 #pragma AP INTERFACE ap_fifo port=color
 #pragma AP INTERFACE ap_fifo port=gray does not play nicely*/
