#include "filter.h"

void gauss_simple(uint8_t video_in[ROWS * COLS],
                  uint8_t video_out[ROWS * COLS]) {

    int16_t kernel[KERN_SIZE][KERN_SIZE] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

    for (int32_t y = 0; y < ROWS; y++) {
        pipeline:for (int32_t x = 0; x < COLS; x++) {

            int16_t sum = 0;
            for (int32_t i = -KERN_RAD; i <= KERN_RAD; i++) {
                for (int32_t j = -KERN_RAD; j <= KERN_RAD; j++) {
                    int32_t yi = y + i;
                    int32_t xj = x + j;
                    if ((xj >= 0) && (xj < COLS) && (y + i >= 0) &&
                        (y + i < ROWS))
                        sum += (int16_t)(video_in[yi * COLS + xj]) *
                               kernel[i + KERN_RAD][j + KERN_RAD];
                }
            }
            // Normalize and store the result
            video_out[y * COLS + x] = (uint8_t)(MAX(MIN(255, sum >> 4), 0));
        }
    }
}
