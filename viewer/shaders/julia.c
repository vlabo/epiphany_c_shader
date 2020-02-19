const int max_iterations = 255;

vec2 complex_square( vec2 v ) {
	return vec2{
		v.x * v.x - v.y * v.y,
		v.x * v.y * 2.0f
    };
}

vec4 shader(float x, float y, float width, float height) {
	vec2 uv = vec2{x - width * 0.5f, y - height * 0.5f};
	uv = (uv * 2.5f) / min( width, height );
	
	vec2 c = vec2{ 0.285, 0.01 };
	vec2 v = uv;
	float scale = 0.01;
	
	int count = max_iterations;
	
	for ( int i = 0 ; i < max_iterations; i++ ) {
		v = c + complex_square( v );
		if ( dot( v, v ) > 4.0 ) {
			count = i;
			break;
		}
	}
	vec4 background = {0.0f, 0.0f, 0.0f, 1.0f};
    vec4 mandl = vec4f( float( count ) * scale );

	return mix(background, mandl, mandl.a); 
}