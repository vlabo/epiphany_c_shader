#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "e_lib.h"
#include <math.h>
#include <shared.h>

volatile bool* done              = (bool*) A_IS_READY;

volatile uint32_t*      p_width         = (uint32_t*) (A_BASE + A_WIDTH);
volatile uint32_t*      p_height        = (uint32_t*) (A_BASE + A_HEIGHT);
volatile uint32_t* 		p_image         = (uint32_t*) (A_BASE + A_IMAGE);

struct vec2 {
	float x;
	float y;

	vec2 operator*(const float v) {
		return vec2{x*v, y*v};
	}

	vec2 operator-(const vec2& v) {
		return vec2{x - v.x, y - v.y};
	}

	vec2 operator+(const vec2& v) {
		return vec2{x + v.x, y + v.y};
	} 

	vec2 operator-(const float v) {
		return vec2{x - v, y - v};
	}

	vec2 operator/(const float v) {
		return vec2{ x / v, y / v};
	}
};

struct vec3 {
	float x;
	float y;
	float z;

	vec2 xy() {
		return vec2{x, y};
	}
};

struct vec4 {
	float x;
	float y;
	float z;
	float a;

	vec3 xyz() {
		return vec3{x, y, z};
	}
	
	vec2 xy() {
		return vec2{x, y};
	}
};

struct bvec2 {
	bool x;
	bool y;
};

struct bvec3 {
	bool x;
	bool y;
	bool z;
};

struct bvec4 {
	bool x;
	bool y;
	bool z;
	bool a;
};

struct mat2 {
	float a;
	float b;
	float c;
	float d;	

	vec2 operator*(const vec2& v) {
		return vec2{v.x*a + v.y*c, v.x*b + v.y*d };
	}
};

vec4 vec4f(vec3 v, float a) {
	return vec4{v.x, v.y, v.z, a};
}

vec4 vec4f(float v) {
	return vec4{v, v, v, v};
}

vec3 vec3f(vec2 v, float z) {
	return vec3{v.x, v.y, z};
}

vec3 vec3f(float v) {
	return vec3{v, v, v};
}

vec2 vec2f(float v) {
	return vec2{v, v};
}

float dot(vec2 v1, vec2 v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

float dot(vec3 v1, vec3 v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

float dot(vec4 v1, vec4 v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.a*v2.a;
}

float length(vec2 v) {
	return sqrtf(v.x*v.x + v.y*v.y);
}

float length(vec3 v) {
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
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

bvec3 lessThan(vec3 v1, vec3 v2) {
	bvec3 bvec = {false, false, false};	
    if(v1.x < v2.x) {
        bvec.x = true;
    }

	if(v1.y < v2.y) {
		bvec.y = true;
	}

	if(v1.z < v2.z) {
		bvec.z = true;
	}

    return bvec;
}

bvec3 greaterThan(vec3 v1, vec3 v2) {
	bvec3 bvec = {false, false, false};	
    if(v1.x > v2.x) {
        bvec.x = true;
    }

	if(v1.y > v2.y) {
		bvec.y = true;
	}

	if(v1.z > v2.z) {
		bvec.z = true;
	}

    return bvec;
}

bool all(bvec3 v) {
	if(v.x && v.y && v.z) {
		return true;
	}
	return false;
}

bool any(bvec3 v) {
	if(v.x || v.y || v.z) {
		return true;
	}
	return false;
}

float smoothstep(float edge0, float edge1, float x) {
	float t; 
	t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}

float min(float a, float b) {
	if(a < b) {
		return a;
	}

	return b;
}
