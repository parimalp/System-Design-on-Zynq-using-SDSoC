/*
(c) Copyright 2013 - 2016 Xilinx, Inc. All rights reserved. 

This file contains confidential and proprietary information of Xilinx, Inc. and
is protected under U.S. and international copyright and other intellectual
property laws.

DISCLAIMER 
This disclaimer is not a license and does not grant any rights to the materials
distributed herewith. Except as otherwise provided in a valid license issued to
you by Xilinx, and to the maximum extent permitted by applicable law: (1) THESE
MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX HEREBY
DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY,
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether
in contract or tort, including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature related to, arising
under or in connection with these materials, including for any direct, or any
indirect, special, incidental, or consequential loss or damage (including loss
of data, profits, goodwill, or any type of loss or damage suffered as a result
of any action brought by a third party) even if such damage or loss was
reasonably foreseeable or Xilinx had been advised of the possibility of the
same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any
application requiring fail-safe performance, such as life-support or safety
devices or systems, Class III medical devices, nuclear facilities, applications
related to the deployment of airbags, or any other applications that could lead
to death, personal injury, or severe property or environmental damage
(individually and collectively, "Critical Applications"). Customer assumes the
sole risk and liability of any use of Xilinx products in Critical Applications,
subject only to applicable laws and regulations governing limitations on product
liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
ALL TIMES. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifdef __SDSCC__
#include "sds_lib.h"
#else 
#define sds_alloc(x)(malloc(x))
#define sds_free(x)(free(x))
#endif

#define N 32
#define NUM_ITERATIONS N

typedef short data_t;

#pragma SDS data access_pattern(a:SEQUENTIAL, b:SEQUENTIAL)
void dataflow(data_t a[N], data_t b[N])
{
#pragma HLS dataflow
  data_t _int[N];
  for(int i = 0; i < N; i++) {
    _int[i] = a[i];
  }
  for(int i = N-1; i > 0; i--) {
    b[N-1-i] = _int[i];
  }
}

void dataflow_sw(data_t *a, data_t *b)
{
  #pragma HLS dataflow
     for(int i = 0; i < N; i++) {
          b[N-1-i] = a[i];
     }
}

int compare(data_t swOut[N], data_t hwOut[N])
{
     for (int i = 0; i < N; i++) {
          if (swOut[i] != hwOut[i]) {
               std::cout << "Values differ: swOut[" << i 
                         << swOut[i] << "],  hwOut[" << i 
                         << "] = " << hwOut[i] << std::endl;
               return -1;
          }
     }
     std::cout << "RESULTS MATCH" << std::endl << std::endl;
     return 0;
}

int main(int argc, char* argv[])
{
     data_t  Bs[N];
     data_t *A = (data_t*)sds_alloc(N * sizeof(data_t));
     data_t *B = (data_t*)sds_alloc(N * sizeof(data_t));

     for (int j = 1; j < N; j++) {
       A[j]  = j;
       B[j] = 0;
       Bs[j] = 0;
     }
     dataflow_sw(A, Bs);
     
     unsigned single_start = sds_clock_counter();
     dataflow(A, B);
     unsigned single_stop = sds_clock_counter();
     
     unsigned start = sds_clock_counter();
#pragma SDS async(1)
     dataflow(A, B);
#pragma SDS async(2)
     dataflow(A, B);
#pragma SDS async (3)
     dataflow(A, B);
#pragma SDS async (4)
     dataflow(A, B);
     
#pragma SDS wait(1)
#pragma SDS wait(2)
#pragma SDS wait(3)
#pragma SDS wait(4)
     unsigned stop = sds_clock_counter();
     
     int result = compare(Bs, B);
     if(result == 0)  {
       std::cout << "Completed successfully" << std::endl;
       
       std::cout << "non-dataflow execution time:  " << (single_stop - single_start) << " cycles" << std::endl;
       std::cout << "dataflow execution time:      " << ((stop - start)/4) << " cycles" << std::endl;
     }
     
     return result;
}

