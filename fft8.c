/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <complex.h>
#include "xil_printf.h"
#include "platform.h"

const int rev8[8] = {0,4,2,6,1,5,3,7};
const float complex W[4] = {1.0 - 0.0*I, 0.7071 - 0.70711*I, 0.0 - 1.0*I, -0.70711 - 0.70711*I};

void bitreverse(float complex dataIn[8], float complex  dataOut[8]){
	bit_reversal: for (int i=0; i<8; i++){
		dataOut[i]=dataIn[rev8[i]];
	}
}

void FFT_stages(float complex FFT_input[8], float complex FFT_output[8]){
    float complex temp1[8],temp2[8];

    // Stage 1
    stage1: for (int i = 0; i < 8; i += 2) {
        temp1[i] = FFT_input[i] + FFT_input[i+1];
        temp1[i+1] = FFT_input[i] - FFT_input[i+1];
    }

    // Stage 2
    stage2: for (int i = 0; i < 8; i += 4) {
	for (int j = 0; j < 2; j++) {
        temp2[i+j] = temp1[i+j] + W[2*j] * temp1[i+ j + 2];
        temp2[i+j+2] = temp1[i+j] - W[2*j] * temp1[i+ j + 2];

    }}

    // Stage 3
    stage3: for (int i = 0; i < 4; i++) {
        FFT_output[i] = temp2[i] + W[i] * temp2[i + 4];
        FFT_output[i + 4] = temp2[i] -W[i] * temp2[i + 4];
    }
}
int main()
{
    init_platform();
    const float complex FFT_input[8] = {1.0 + 1.0i, 2.0 + 2.0i, 3.0 + 3.0i, 4.0 + 4.0i, 5.0 + 5.0i, 6.0 + 6.0i, 7.0 + 7.0i, 8.0 + 8.0i};
    float complex FFT_output[8];
    float complex FFT_rev[8];
    bitreverse(FFT_input, FFT_rev);
    FFT_stages(FFT_rev, FFT_output);
    printf("\nPrinting FFT input:\r\n");
    for (int i = 0; i < 8; i++)
    {
        printf("%f %f\n", creal(FFT_input[i]), cimag(FFT_input[i]));
    }

    printf("\nPrinting FFT output:\n");
    for (int i = 0; i < 8; i++)
    {
        printf("%f %f\n", creal(FFT_output[i]), cimag(FFT_output[i]));
    }

    cleanup_platform();
    return 0;
}
