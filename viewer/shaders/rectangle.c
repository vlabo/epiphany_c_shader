vec4 rectangle(vec2 uv, vec2 pos, float width, float height, vec3 color) {
	float t = 0.0f;
	if ((uv.x > pos.x - width / 2.0f) && (uv.x < pos.x + width / 2.0f)
		&& (uv.y > pos.y - height / 2.0f) && (uv.y < pos.y + height / 2.0f)) {
		t = 1.0;
	}
    vec4 result = {color.x, color.y, color.z, t};
	return result;
}

vec4 shader(float x, float y, float width, float height) {
    vec2 uv = {width / 2, height / 2};
    vec2 pos = {x, y};
    vec3 color = {1.0f, 1.0f, 0.2f};

    vec4 c = rectangle(uv, pos, 250.0f, 250.0f, color); 
    vec4 white = {0.6f, 0.6f, 0.6f, 1.0f};
    return mix(white, c, c.a);
} 