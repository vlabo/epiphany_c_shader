#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "e_lib.h"
#include <math.h>
#include <shared.h>

volatile bool* done              = (void*) A_IS_READY;

volatile uint32_t*      p_width         = (void*) (A_BASE + A_WIDTH);
volatile uint32_t*      p_height        = (void*) (A_BASE + A_HEIGHT);
volatile uint32_t* 		p_image         = (void*) (A_BASE + A_IMAGE);

typedef struct vec2 {
	float x;
	float y;
} vec2;

typedef struct vec3 {
	float x;
	float y;
	float z;
} vec3;

typedef struct vec4 {
	float x;
	float y;
	float z;
	float a;
} vec4;

float dot(vec2 v1, vec2 v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

float length(vec2 v) {
	return sqrtf(v.x*v.x + v.y*v.y);
}

float mod(float x, float y) {
	return x - y * floorf(x/y);
}

float clamp(float value, float min, float max) {
	if(value > max) {
		return max;
	}

	if(value < min) {
		return min;
	}

	return value;
}

vec4 mix(vec4 c1, vec4 c2, float a) {
	vec4 result = {c1.x * (1.0f - a) + c2.x * a,
	               c1.y * (1.0f - a) + c2.y * a,
	               c1.z * (1.0f - a) + c2.z * a,
	               c1.a };
	return result;
}

vec4 shader(float x, float y, float width, float height);

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

<!--shader-->
