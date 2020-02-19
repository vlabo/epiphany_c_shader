// Created by Vinicius Graciano Santos - vgs/2013
// I've learned a lot about fractals in this series of blog posts:
// http://blog.hvidtfeldts.net/index.php/2011/08/distance-estimated-3d-fractals-iii-folding-space/

#define TAU 6.283185

// play with this value (try 3)
#define SIDES 3

// uncomment to see that the polygon lies within the unit circle.
//#define UCIRCLE

float fractal(vec2 uv) {
	float c = cosf(1.0/float(SIDES)*TAU);
	float s = sinf(1.0/float(SIDES)*TAU);
	
	mat2 m = mat2{c, s, -s, c};
	vec2 p = vec2{1.0, 0.0};
    vec2 r = p;
	
	for (int i = 0; i < 7; ++i) {
		float dmin = length(uv - r);
		for (int j = 0; j < SIDES; ++j) {
			p = m*p;
			float d = length(uv - p); 
			if (d < dmin) {dmin = d; r = p;}
		}
		uv = uv * 2.0 - r;
	}
	
	return (length(uv-r)-0.15)/pow(2.0, 7.0);
}

vec4 shader(float x, float y, float width, float height) {
	vec2 uv = vec2{x / width, y / height};
	float vig = 0.15 + pow(uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y), 0.1);
	
	uv = uv*2.0f - 1.0f;
	
	float d = fractal(uv);
	d = smoothstep(0.001, 0.015, d);
	
	#ifdef UCIRCLE
	d *= 1.0-length(uv);
	#endif
		
	vec4 color = vec4f(vec3f(pow(vig*d, 0.45)),1.0);
    return color;
}