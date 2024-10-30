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

const int rev128[128] = { 0, 64, 32, 96, 16, 80, 48, 112, 8, 72, 40, 104, 24, 88, 56, 120,
	    4, 68, 36, 100, 20, 84, 52, 116, 12, 76, 44, 108, 28, 92, 60, 124,
	    2, 66, 34, 98, 18, 82, 50, 114, 10, 74, 42, 106, 26, 90, 58, 122,
	    6, 70, 38, 102, 22, 86, 54, 118, 14, 78, 46, 110, 30, 94, 62, 126,
	    1, 65, 33, 97, 17, 81, 49, 113, 9, 73, 41, 105, 25, 89, 57, 121,
	    5, 69, 37, 101, 21, 85, 53, 117, 13, 77, 45, 109, 29, 93, 61, 125,
	    3, 67, 35, 99, 19, 83, 51, 115, 11, 75, 43, 107, 27, 91, 59, 123,
	    7, 71, 39, 103, 23, 87, 55, 119, 15, 79, 47, 111, 31, 95, 63, 127
	};

const float complex W[64] = {
    1.0 - 0.0*I, 0.9988 - 0.0491*I, 0.9952 - 0.0980*I, 0.9892 - 0.1467*I,
    0.9808 - 0.1951*I, 0.9700 - 0.2430*I, 0.9569 - 0.2903*I, 0.9415 - 0.3369*I,
    0.9239 - 0.3827*I, 0.9039 - 0.4276*I, 0.8819 - 0.4714*I, 0.8577 - 0.5141*I,
    0.8315 - 0.5556*I, 0.8032 - 0.5957*I, 0.7730 - 0.6344*I, 0.7409 - 0.6716*I,
    0.7071 - 0.7071*I, 0.6716 - 0.7409*I, 0.6344 - 0.7730*I, 0.5957 - 0.8032*I,
    0.5556 - 0.8315*I, 0.5141 - 0.8577*I, 0.4714 - 0.8819*I, 0.4276 - 0.9039*I,
    0.3827 - 0.9239*I, 0.3369 - 0.9415*I, 0.2903 - 0.9569*I, 0.2430 - 0.9700*I,
    0.1951 - 0.9808*I, 0.1467 - 0.9892*I, 0.0980 - 0.9952*I, 0.0491 - 0.9988*I,
    0.0 - 1.0*I, -0.0491 - 0.9988*I, -0.0980 - 0.9952*I, -0.1467 - 0.9892*I,
    -0.1951 - 0.9808*I, -0.2430 - 0.9700*I, -0.2903 - 0.9569*I, -0.3369 - 0.9415*I,
    -0.3827 - 0.9239*I, -0.4276 - 0.9039*I, -0.4714 - 0.8819*I, -0.5141 - 0.8577*I,
    -0.5556 - 0.8315*I, -0.5957 - 0.8032*I, -0.6344 - 0.7730*I, -0.6716 - 0.7409*I,
    -0.7071 - 0.7071*I, -0.7409 - 0.6716*I, -0.7730 - 0.6344*I, -0.8032 - 0.5957*I,
    -0.8315 - 0.5556*I, -0.8577 - 0.5141*I, -0.8819 - 0.4714*I, -0.9039 - 0.4276*I,
    -0.9239 - 0.3827*I, -0.9415 - 0.3369*I, -0.9569 - 0.2903*I, -0.9700 - 0.2430*I,
    -0.9808 - 0.1951*I, -0.9892 - 0.1467*I, -0.9952 - 0.0980*I, -0.9988 - 0.0491*I
};

void bitreverse(float complex dataIn[128], float complex dataOut[128]){
	bit_reversal: for (int i=0; i<128; i++){
		dataOut[i]=dataIn[rev128[i]];
	}
}

void FFT_stages(float complex FFT_input[128], float complex FFT_output[128]) {
    float complex temp1[128], temp2[128], temp3[128], temp4[128], temp5[128], temp6[128];

    // Stage 1
    stage1: for (int i = 0; i < 128; i += 2) {
        temp1[i] = FFT_input[i] + FFT_input[i + 1];
        temp1[i + 1] = FFT_input[i] - FFT_input[i + 1];
    }

    // Stage 2
    stage2: for (int i = 0; i < 128; i += 4) {
        for (int j = 0; j < 2; j++) {
            temp2[i + j] = temp1[i + j] + W[32 * j] * temp1[i + j + 2];
            temp2[i + j + 2] = temp1[i + j] - W[32 * j] * temp1[i + j + 2];
        }
    }

    // Stage 3
    stage3: for (int i = 0; i < 128; i += 8) {
        for (int j = 0; j < 4; j++) {
            temp3[i + j] = temp2[i + j] + W[16 * j] * temp2[i + j + 4];
            temp3[i + j + 4] = temp2[i + j] - W[16 * j] * temp2[i + j + 4];
        }
    }

    // Stage 4
    stage4: for (int i = 0; i < 128; i += 16) {
        for (int j = 0; j < 8; j++) {
            temp4[i + j] = temp3[i + j] + W[8 * j] * temp3[i + j + 8];
            temp4[i + j + 8] = temp3[i + j] - W[8 * j] * temp3[i + j + 8];
        }
    }

    // Stage 5
    stage5: for (int i = 0; i < 128; i += 32) {
        for (int j = 0; j < 16; j++) {
            temp5[i + j] = temp4[i + j] + W[4 * j] * temp4[i + j + 16];
            temp5[i + j + 16] = temp4[i + j] - W[4 * j] * temp4[i + j + 16];
        }
    }

    // Stage 6
    stage6: for (int i = 0; i < 128; i += 64) {
        for (int j = 0; j < 32; j++) {
            temp6[i + j] = temp5[i + j] + W[2 * j] * temp5[i + j + 32];
            temp6[i + j + 32] = temp5[i + j] - W[2 * j] * temp5[i + j + 32];
        }
    }

    // Final Stage
    stage7: for (int i = 0; i < 64; i++) {
        FFT_output[i] = temp6[i] + W[i] * temp6[i + 64];
        FFT_output[i + 64] = temp6[i] - W[i] * temp6[i + 64];
    }
}

int main()
{
    init_platform();
    const float complex FFT_input[128] = {
    	    1.0 + 1.0i, 2.0 + 2.0i, 3.0 + 3.0i, 4.0 + 4.0i, 5.0 + 5.0i, 6.0 + 6.0i, 7.0 + 7.0i, 8.0 + 8.0i,
    	    9.0 + 9.0i, 10.0 + 10.0i, 11.0 + 11.0i, 12.0 + 12.0i, 13.0 + 13.0i, 14.0 + 14.0i, 15.0 + 15.0i, 16.0 + 16.0i,
    	    17.0 + 17.0i, 18.0 + 18.0i, 19.0 + 19.0i, 20.0 + 20.0i, 21.0 + 21.0i, 22.0 + 22.0i, 23.0 + 23.0i, 24.0 + 24.0i,
    	    25.0 + 25.0i, 26.0 + 26.0i, 27.0 + 27.0i, 28.0 + 28.0i, 29.0 + 29.0i, 30.0 + 30.0i, 31.0 + 31.0i, 32.0 + 32.0i,
    	    33.0 + 33.0i, 34.0 + 34.0i, 35.0 + 35.0i, 36.0 + 36.0i, 37.0 + 37.0i, 38.0 + 38.0i, 39.0 + 39.0i, 40.0 + 40.0i,
    	    41.0 + 41.0i, 42.0 + 42.0i, 43.0 + 43.0i, 44.0 + 44.0i, 45.0 + 45.0i, 46.0 + 46.0i, 47.0 + 47.0i, 48.0 + 48.0i,
    	    49.0 + 49.0i, 50.0 + 50.0i, 51.0 + 51.0i, 52.0 + 52.0i, 53.0 + 53.0i, 54.0 + 54.0i, 55.0 + 55.0i, 56.0 + 56.0i,
    	    57.0 + 57.0i, 58.0 + 58.0i, 59.0 + 59.0i, 60.0 + 60.0i, 61.0 + 61.0i, 62.0 + 62.0i, 63.0 + 63.0i, 64.0 + 64.0i,
    	    65.0 + 65.0i, 66.0 + 66.0i, 67.0 + 67.0i, 68.0 + 68.0i, 69.0 + 69.0i, 70.0 + 70.0i, 71.0 + 71.0i, 72.0 + 72.0i,
    	    73.0 + 73.0i, 74.0 + 74.0i, 75.0 + 75.0i, 76.0 + 76.0i, 77.0 + 77.0i, 78.0 + 78.0i, 79.0 + 79.0i, 80.0 + 80.0i,
    	    81.0 + 81.0i, 82.0 + 82.0i, 83.0 + 83.0i, 84.0 + 84.0i, 85.0 + 85.0i, 86.0 + 86.0i, 87.0 + 87.0i, 88.0 + 88.0i,
    	    89.0 + 89.0i, 90.0 + 90.0i, 91.0 + 91.0i, 92.0 + 92.0i, 93.0 + 93.0i, 94.0 + 94.0i, 95.0 + 95.0i, 96.0 + 96.0i,
    	    97.0 + 97.0i, 98.0 + 98.0i, 99.0 + 99.0i, 100.0 + 100.0i, 101.0 + 101.0i, 102.0 + 102.0i, 103.0 + 103.0i, 104.0 + 104.0i,
    	    105.0 + 105.0i, 106.0 + 106.0i, 107.0 + 107.0i, 108.0 + 108.0i, 109.0 + 109.0i, 110.0 + 110.0i, 111.0 + 111.0i, 112.0 + 112.0i,
    	    113.0 + 113.0i, 114.0 + 114.0i, 115.0 + 115.0i, 116.0 + 116.0i, 117.0 + 117.0i, 118.0 + 118.0i, 119.0 + 119.0i, 120.0 + 120.0i,
    	    121.0 + 121.0i, 122.0 + 122.0i, 123.0 + 123.0i, 124.0 + 124.0i, 125.0 + 125.0i, 126.0 + 126.0i, 127.0 + 127.0i, 128.0 + 128.0i
    	};
    float complex FFT_output[128];
    float complex FFT_rev[128];
    bitreverse(FFT_input, FFT_rev);
    FFT_stages(FFT_rev, FFT_output);
    printf("\nPrinting FFT input:\r\n");
    for (int i = 0; i < 128; i++)
    {
        printf("%f %f\n", creal(FFT_input[i]), cimag(FFT_input[i]));
    }

    printf("\nPrinting FFT output:\n");
    for (int i = 0; i < 128; i++)
    {
        printf("%f %f\n", creal(FFT_output[i]), cimag(FFT_output[i]));
    }

    cleanup_platform();
    return 0;
}

