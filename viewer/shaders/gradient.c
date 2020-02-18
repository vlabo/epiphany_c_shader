vec4 shader(float x, float y, float width, float height) {
    float value = (x / width);
    vec4 color = {value, value, value, 1.0f};
    return color; 
}