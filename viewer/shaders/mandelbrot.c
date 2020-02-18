float mandelbrot(float x, float y) {

    const float B = 256.0;
    float l = 0.0f;
    vec2 z  = {0.0f, 0.0f};
    for( int i=0; i<512; i++ )
    {
        vec2 tz = z;
        tz.x = z.x*z.x - z.y*z.y + x; 
        tz.y = 2.0*z.x*z.y + y;
        z = tz;
        if( dot(z,z)>(B*B) ) break;
        l += 1.0;
    }

    if( l>511.0 ) return 0.0;
    return l;
}

uint32_t shader(float x, float y, float width, float height) {
    uint32_t value =  mandelbrot(x / width, y / height) * 255.0f;
    value %= 255;
    int a = 0xFF  << 24;
    int b = value << 16;
    int g = value << 8;
    int r = value; return r | g | b | a; 
}