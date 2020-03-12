#include "filter.h"

void gauss_simple (uint8_t video_in[ROWS*COLS], uint8_t video_out[ROWS*COLS]) {
#pragma HLS INTERFACE axis port=video_in bundle=in_stream
#pragma HLS INTERFACE axis port=video_out bundle=out_stream

    int16_t kernel[KERN_SIZE][KERN_SIZE] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}};
#pragma HLS array_partition variable=kernel complete dim=0

    uint8_t row0[COLS+2] = { 0 },
            row1[COLS+2] = { 0 };
    uint8_t window[3][3] = { 0 };

    int counter_stream = 0;
    int16_t sum = 0;

    // read first image row into second buffer
    for (int var = 0; var < COLS; var++) {
        row1[var+1] = video_in[var];
    }

    // iterate over all image pixels
    for(int32_t i=0; i<(ROWS*COLS); i++) {
#pragma HLS pipeline

        // shift pixels in sliding window
        for (int k = 0; k < 3; k++) {
            for (int j = 0; j < 2; j++) {
                window[k][j] = window[k][j+1];
            }
        }
        window[0][2] = row0[counter_stream];
        window[1][2] = row1[counter_stream];
        if (i >= (ROWS-1)*(COLS+2)) {
            window[2][2] = 0;
        } else {
            if (!(i % (COLS+2) || i % (COLS+1))) {
                window[2][2] = 0;
            } else {
                window[2][2] = video_in[i+COLS];
            }
        }

        // calculate resulting pixel
        sum = 0;
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 3; column++) {
                sum += (int16_t)(window[row][column]) * kernel[row][column];
            }
        }

        // Normalize and store the result
        video_out[i] = (uint8_t)(MAX(MIN(255, sum >> 4), 0));

        // shift buffers and pointer
        row0[counter_stream] = row1[counter_stream];
        row1[counter_stream] = window[2][2];
        counter_stream++;
        counter_stream = counter_stream % (COLS-1);
    }
}
