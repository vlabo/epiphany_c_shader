#include "shader_functions.h"
// vec4 shader(float x, float y, float width, float height);
<!--shader-->

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
                        size_t index = x + startX + (frame_height - (y + 1 + startY)) * frame_width;
                        uint32_t real_x = (x + startX);
                        uint32_t real_y = (y + startY);
						vec4 color = shader(real_x, real_y, frame_width, frame_height);
                        int a = (int)(color.a * 255.0f) << 24;
						int b = (int)(color.z * 255.0f) << 16;
						int g = (int)(color.y * 255.0f) << 8;
						int r = (int)(color.x * 255.0f);
						p_image[index] =   r | g | b | a;
 				}
        }

        *done = true;
        return EXIT_SUCCESS;
}

