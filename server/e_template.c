#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "e_lib.h"
#include <shared.h>

volatile bool* done              = (void*) A_IS_READY;

volatile uint32_t*      p_width         = (void*) (A_BASE + A_WIDTH);
volatile uint32_t*      p_height        = (void*) (A_BASE + A_HEIGHT);
volatile uint32_t* 		p_image         = (void*) (A_BASE + A_IMAGE);

uint32_t shader(float x, float y, float width, float height);

int main(void) {
        e_coreid_t coreid = e_get_coreid();
	    unsigned int row, col;
        e_coords_from_coreid(coreid, &row, &col);

        uint32_t frame_width = *p_width;
        uint32_t frame_height = *p_height;

        size_t width = frame_width / COLS_COUNT;
        size_t height = frame_height / ROWS_COUNT;
        size_t startX = col * width;
        size_t startY = row * height;

        for(size_t y = 0; y < height; y++) {
                for(size_t x = 0; x < width; x++) {
                        size_t index = x + startX + (y + startY) * frame_width;
                        uint32_t real_x = (x + startX);
                        uint32_t real_y = (y + startY);
                        p_image[index] = shader(real_x, real_y, frame_width, frame_height);
                }
        }

        *done = true;
        return EXIT_SUCCESS;
}

<!--shader-->
