/*
 * sharpen.h
 *
 *  Created on: Jun 5, 2015
 *      Author: lancer
 */

#ifndef SHARPEN_H_
#define SHARPEN_H_

/* function proto-types */

//#pragma SDS data sys_port(input:AFI, output:AFI)
#pragma SDS data access_pattern(input:SEQUENTIAL, output:SEQUENTIAL)
void sharpen_filter(uint8_t input[2073600], uint8_t output[2073600]);



#endif /* SHARPEN_H_ */
