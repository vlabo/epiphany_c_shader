vec4 shader(float x, float y, float width, float height )
{
    vec3 p  = vec3{ x / width, y / height, -1.0f},
    n1 = vec3{-1.0f,0.6f,0.0f},  n2 =vec3{1.0f,0.6f,1.0f},    n3 = vec3{0.0f,-1.0f,-0.2f}, 
  	d  = vec3{ dot(p,n1), dot(p,n2), dot(p,n3) };
    vec4 color = vec4f( all(lessThan(d,vec3f(.01f))) && any(greaterThan(d,vec3f(0.0f)))  ? 0.1f : 1.0f);

    return color;
}