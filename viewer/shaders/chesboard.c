#define S 8.0f // Scale

vec4 shader(float x, float y, float width, float height) {
    float cx = floorf(S * x / height);
    float cy = floorf(S * y / height);
    float v = mod(cx + cy, 2.0f);
	vec4 c = {v, v, v, 1};
    return c;
}