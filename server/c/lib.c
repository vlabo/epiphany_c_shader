#include <stdint.h>
#include <stdlib.h>
#include <e-hal.h>
#include <stdbool.h>

#include "shared.h"

int m_width = 0;
int m_height = 0;

extern int usleep(int);
extern int e_load_group(const char*, e_epiphany_t*, unsigned, unsigned, unsigned, unsigned, e_bool_t);

void epiphany_init() {
    e_init(NULL);
}

void epiphany_destroy() {
    e_finalize();
}

char* epiphany_alloc(uint32_t width, uint32_t height) {
    char* image_data = malloc(width * height * sizeof(uint32_t));
    m_width = width;
    m_height = height;
    return image_data;
}

void epiphany_free(char* image_data) {
    free(image_data);
}

void get_frame(char* image_data) {
    e_platform_t platform;
    e_epiphany_t dev;
    
	uint32_t size = m_width * m_height * 4;

    e_mem_t image_mem;
    e_alloc(&image_mem, A_IMAGE, size);
    e_mem_t image_size;
    e_alloc(&image_size, A_IMAGE_SIZE, sizeof(uint32_t) * 2);
    e_write(&image_size, 0, 0, 0x0, &m_width, sizeof(uint32_t));
    e_write(&image_size, 0, 0, 0x4, &m_height, sizeof(uint32_t));

    e_reset_system();
    e_get_platform_info(&platform);
    e_open(&dev, 0, 0, ROWS_COUNT, COLS_COUNT);
    e_reset_group(&dev);
    e_load_group("e_main.elf", &dev, 0, 0, ROWS_COUNT, COLS_COUNT, E_FALSE);

	bool not_done = false;
    // Set "done" flags
    for(int row = 0; row < ROWS_COUNT; row++) {
        for(int col = 0; col < COLS_COUNT; col++) {
            e_write(&dev, row, col, A_IS_READY, &not_done, sizeof(bool));
        }
    }

    // Start cores
    e_start_group(&dev);

    // Wait for cores to finish
    bool e_done = false;
    while(!e_done) {
        usleep(1000);
        e_done = true;
        for(int row = 0; row < ROWS_COUNT; row++) {
            for(int col = 0; col < COLS_COUNT; col++) {
                bool is_done = false;
                if(e_read(&dev, row, col, A_IS_READY, &is_done, sizeof(bool)) != sizeof(bool)) {
                }

                if(!is_done) {
                    e_done = false;
                }
            }
        }
    }

    // Done executing, get the image from memory
    e_read(&image_mem, 0, 0, 0, image_data, size);
//	memset(image_data, 255, size);
}
