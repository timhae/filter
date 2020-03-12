#ifndef __FILTER_H__
#define __FILTER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>

#define COLS 850
#define ROWS 680
#define KERN_SIZE 3
#define KERN_RAD (KERN_SIZE >> 1)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


void sobel_simple (uint8_t video_in[ROWS*COLS], uint8_t video_out[ROWS*COLS]);
void gauss_simple (uint8_t video_in[ROWS*COLS], uint8_t video_out[ROWS*COLS]);

#endif
