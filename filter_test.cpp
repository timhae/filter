#include "filter.h"

#define HI(num) (((num)&0x0000FF00) << 8)
#define LO(num) ((num)&0x000000FF)

/* pgm data structure */
typedef struct _PGMData {
    int32_t row;
    int32_t col;
    int32_t max_gray;
    uint8_t *matrix;
} PGMData;

/* to skip comments */
void SkipComments(FILE *fp) {
    int ch;
    char line[100];
    while ((ch = fgetc(fp)) != EOF && isspace(ch)) {
        ;
    }

    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        SkipComments(fp);
    } else {
        fseek(fp, -1, SEEK_CUR);
    }
}

/* for reading:*/
PGMData *readPGM(const char *file_name, PGMData *data) {
    FILE *pgmFile;
    char version[3];
    int i, j;
    int lo, hi;
    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        perror("cannot open file to read");
        exit(EXIT_FAILURE);
    }
    fgets(version, sizeof(version), pgmFile);
    if (strcmp(version, "P5")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &(data->col));
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &(data->row));
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &(data->max_gray));
    fgetc(pgmFile);

    if (data->max_gray > 255) {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                hi = fgetc(pgmFile);
                lo = fgetc(pgmFile);
                data->matrix[i * (data->col) + j] = (hi << 8) + lo;
            }
        }
    } else {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                lo = fgetc(pgmFile);
                data->matrix[i * (data->col) + j] = lo;
            }
        }
    }

    fclose(pgmFile);
    return data;
}

/*for writing*/
void writePGM(const char *filename, const PGMData *data) {
    FILE *pgmFile;
    int i, j;
    int hi, lo;

    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }

    fprintf(pgmFile, "P5 ");
    fprintf(pgmFile, "%d %d ", data->col, data->row);
    fprintf(pgmFile, "%d ", data->max_gray);

    if (data->max_gray > 255) {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                hi = HI(data->matrix[i * (data->col) + j]);
                lo = LO(data->matrix[i * (data->col) + j]);
                fputc(hi, pgmFile);
                fputc(lo, pgmFile);
            }
        }
    } else {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                lo = LO(data->matrix[i * (data->col) + j]);
                fputc(lo, pgmFile);
            }
        }
    }

    fclose(pgmFile);
}

/*****************************************************************************/
/*****************************************************************************/

int main() {

    // Create data structures
    struct _PGMData *image1 = new _PGMData;
    struct _PGMData *image2 = new _PGMData;

    // Allocate
    image1->matrix = new uint8_t[COLS * ROWS];
    image2->matrix = new uint8_t[COLS * ROWS];

    // Read input
    readPGM("in.pgm", image1);

    // Set image data
    image2->col = image1->col;
    image2->row = image1->row;
    image2->max_gray = image1->max_gray;

    // Compute filter
    gauss_simple(image1->matrix, image2->matrix);

    // Store output
    writePGM("out.pgm", image2);

    // Deallocate
    delete[] image1->matrix;
    delete[] image2->matrix;

    return 0;
}
