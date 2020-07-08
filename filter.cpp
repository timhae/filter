#include "filter.h"

void gauss_simple(uint8_t video_in[ROWS * COLS],
                  uint8_t video_out[ROWS * COLS]) {
#pragma HLS INTERFACE axis port = video_in bundle = in_stream
#pragma HLS INTERFACE axis port = video_out bundle = out_stream

    uint16_t kernel[KERN_SIZE][KERN_SIZE] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
    uint8_t line_buf[KERN_SIZE - 1][COLS] = {{0}};
    uint8_t right_side[KERN_SIZE] = {0};
    uint8_t window[KERN_SIZE][KERN_SIZE] = {{0}};
#pragma HLS array_partition variable = kernel complete dim = 0
#pragma HLS array_partition variable = line_buf complete dim = 1
#pragma HLS array_partition variable = right_side complete
#pragma HLS array_partition variable = window complete dim = 0

    uint32_t counter_stream = 0;

    // Read first image rows into line buffer
    for (int x = COLS - KERN_RAD - 1; x < COLS; x++) {
#pragma HLS pipeline
        line_buf[KERN_RAD - 1][x] = video_in[counter_stream];
        counter_stream++;
    }
    for (int y = KERN_RAD; y < KERN_SIZE - 1; ++y) {
        for (int x = 0; x < COLS; ++x) {
#pragma HLS pipeline
            line_buf[y][x] = video_in[counter_stream];
            counter_stream++;
        }
    }

    // Populate window
    for (int y = KERN_RAD; y < KERN_SIZE; ++y) {
        for (int x = KERN_RAD; x < KERN_SIZE; ++x) {
#pragma HLS pipeline
            window[y][x] = line_buf[y - 1][x + COLS - KERN_SIZE];
        }
    }

    // Iterate over all image pixels
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
#pragma HLS pipeline

            // Calculate resulting pixel
            uint16_t sum = 0;
            for (int i = -KERN_RAD; i <= KERN_RAD; i++) {
                for (int j = -KERN_RAD; j <= KERN_RAD; j++) {
                    uint32_t yi = y + i;
                    uint32_t xj = x + j;
                    if (0 <= yi && yi < ROWS && 0 <= xj && xj < COLS) {
                        sum += (uint16_t)((window[i + KERN_RAD][j + KERN_RAD]) *
                                          kernel[i + KERN_RAD][j + KERN_RAD]);
                    }
                }
            }

            // Normalize and store the result
            video_out[y * COLS + x] = (uint8_t)(MAX(MIN(255, sum >> 4), 0));

            // Shift buffer
            right_side[0] = line_buf[0][x];
            for (int y = 1; y < KERN_SIZE - 1; ++y) {
                right_side[y] = line_buf[y - 1][x] = line_buf[y][x];
            }

            // Read new value
            uint8_t val_in = 0;
            if (counter_stream < ROWS * COLS) {
                val_in = video_in[counter_stream];
                counter_stream++;
            }
            right_side[KERN_SIZE - 1] = line_buf[KERN_SIZE - 2][x] = val_in;

            // Shift window content
            for (int y = 0; y < KERN_SIZE; y++) {
                for (int x = 0; x < KERN_SIZE - 1; x++) {
                    window[y][x] = window[y][x + 1];
                }
            }

            // Update window values at right side
            for (int y = 0; y < KERN_SIZE; y++) {
                window[y][KERN_SIZE - 1] = right_side[y];
            }
        }
    }
}
