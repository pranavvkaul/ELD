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

const int rev32[32] = { 0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30, 1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31 };

const float complex W[16] = { 1.0 - 0.0*I, 0.9808 - 0.1951*I, 0.9239 - 0.3827*I, 0.8315 - 0.5556*I, 0.7071 - 0.7071*I, 0.5556 - 0.8315*I, 0.3827 - 0.9239*I, 0.1951 - 0.9808*I, 0.0 - 1.0*I, -0.1951 - 0.9808*I, -0.3827 - 0.9239*I, -0.5556 - 0.8315*I, -0.7071 - 0.7071*I, -0.8315 - 0.5556*I, -0.9239 - 0.3827*I, -0.9808 - 0.1951*I };

void bitreverse(float complex dataIn[32], float complex dataOut[32]){
	bit_reversal: for (int i=0; i<32; i++){
		dataOut[i]=dataIn[rev32[i]];
	}
}

#include <complex.h>

void FFT_stages(float complex FFT_input[32], float complex FFT_output[32]) {
    float complex temp1[32], temp2[32], temp3[32], temp4[32];

    // Stage 1
    stage1: for (int i = 0; i < 32; i += 2) {
        temp1[i] = FFT_input[i] + FFT_input[i + 1];
        temp1[i + 1] = FFT_input[i] - FFT_input[i + 1];
    }

    // Stage 2
    stage2: for (int i = 0; i < 32; i += 4) {
        for (int j = 0; j < 2; j++) {
            temp2[i + j] = temp1[i + j] + W[8 * j] * temp1[i + j + 2];
            temp2[i + j + 2] = temp1[i + j] - W[8 * j] * temp1[i + j + 2];
        }
    }

    // Stage 3
    stage3: for (int i = 0; i < 32; i += 8) {
        for (int j = 0; j < 4; j++) {
            temp3[i + j] = temp2[i + j] + W[4 * j] * temp2[i + j + 4];
            temp3[i + j + 4] = temp2[i + j] - W[4 * j] * temp2[i + j + 4];
        }
    }

    // Stage 4
    stage4: for (int i = 0; i < 32; i += 16) {
        for (int j = 0; j < 8; j++) {
            temp4[i + j] = temp3[i + j] + W[2 * j] * temp3[i + j + 8];
            temp4[i + j + 8] = temp3[i + j] - W[2 * j] * temp3[i + j + 8];
        }
    }

    // Stage 5 (Final Stage)
    stage5: for (int i = 0; i < 16; i++) {
        FFT_output[i] = temp4[i] + W[i] * temp4[i + 16];
        FFT_output[i + 16] = temp4[i] - W[i] * temp4[i + 16];
    }
}

int main()
{
    init_platform();
    const float complex FFT_input[32] = { 1.0 + 1.0i, 2.0 + 2.0i, 3.0 + 3.0i, 4.0 + 4.0i, 5.0 + 5.0i, 6.0 + 6.0i, 7.0 + 7.0i, 8.0 + 8.0i, 9.0 + 9.0i, 10.0 + 10.0i, 11.0 + 11.0i, 12.0 + 12.0i, 13.0 + 13.0i, 14.0 + 14.0i, 15.0 + 15.0i, 16.0 + 16.0i, 17.0 + 17.0i, 18.0 + 18.0i, 19.0 + 19.0i, 20.0 + 20.0i, 21.0 + 21.0i, 22.0 + 22.0i, 23.0 + 23.0i, 24.0 + 24.0i, 25.0 + 25.0i, 26.0 + 26.0i, 27.0 + 27.0i, 28.0 + 28.0i, 29.0 + 29.0i, 30.0 + 30.0i, 31.0 + 31.0i, 32.0 + 32.0i };
    float complex FFT_output[32];
    float complex FFT_rev[32];
    bitreverse(FFT_input, FFT_rev);
    FFT_stages(FFT_rev, FFT_output);
    printf("\nPrinting FFT input:\r\n");
    for (int i = 0; i < 32; i++)
    {
        printf("%f %f\n", creal(FFT_input[i]), cimag(FFT_input[i]));
    }

    printf("\nPrinting FFT output:\n");
    for (int i = 0; i < 32; i++)
    {
        printf("%f %f\n", creal(FFT_output[i]), cimag(FFT_output[i]));
    }

    cleanup_platform();
    return 0;
}

