#include <stdint.h>
#include <stdlib.h>

int m_width = 0;
int m_height = 0;

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
    uint32_t* pixels = (uint32_t*) image_data;
    for(int i = 0; i < m_width * m_height; i++) {
        pixels[i] = 0xFF00FF00;
    }
}