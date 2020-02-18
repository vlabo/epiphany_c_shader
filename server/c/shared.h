#ifndef SHARED_H
#define SHARED_H

// "A" stand for Address

#define A_BASE           0x8e000000

#define ROWS_COUNT 4
#define COLS_COUNT 4
#define CORES_COUNT (ROWS_COUNT * COLS_COUNT)

#define A_IS_READY       0x5000

#define A_IMAGE_SIZE 0x0000
#define A_WIDTH          0x0000
#define A_HEIGHT         0x0004
#define A_IMAGE          0x0008

#endif