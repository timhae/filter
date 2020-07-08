#ifndef __FILTER_H__
#define __FILTER_H__

#include <algorithm>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLS 512
#define ROWS 512
#define KERN_SIZE 3
#define KERN_RAD (KERN_SIZE >> 1)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void gauss_simple(uint8_t video_in[ROWS * COLS],
                  uint8_t video_out[ROWS * COLS]);

#endif
