vec4 circle(vec2 uv, vec2 pos, float rad, vec3 color) {
    vec2 relative = { pos.x - uv.x, pos.y - uv.y};
	float d = length(relative) - rad;
	float t = clamp(d, 0.0f, 1.0f);
    vec4 result = {color.x, color.y, color.z, 1.0f - t};
	return result;
}

vec4 shader(float x, float y, float width, float height) {
    float value = (x / width);
    vec2 uv = {width / 2, height / 2};
    vec2 pos = {x, y};
    vec3 color = {1.0f, 0.0f, 0.0f};

    vec4 c = circle(uv, pos, 150.0f, color); 
    vec4 white = {1.0f, 1.0f, 1.0f, 1.0f};
    return mix(white, c, c.a);
}