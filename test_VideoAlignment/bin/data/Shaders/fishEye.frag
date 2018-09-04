#version 120

#define PI 3.1415926

uniform sampler2DRect inputTexture;
uniform vec2 resolution;
uniform float amount;
uniform vec2 center;

vec2 cartToPol(vec2 v) {
    float r = sqrt(v.x*v.x + v.y*v.y);
    float theta = atan(v.y, v.x);
    return vec2(r, theta);
}

vec2 polToCart(vec2 v) {
    float x = v.x * cos(v.y);
    float y = v.x * sin(v.y);
    return vec2(x, y);
}

float map(float value, float low1, float high1, float low2, float high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

void main() {
    
    vec2 uv = gl_FragCoord.xy / resolution;
    
    vec2 uvCenter = center / resolution;
    
//    uv.y = 1.0 - uv.y;
    
    uv.x -= uvCenter.x;
    uv.y -= uvCenter.y;
    
    vec2 pol = cartToPol(uv);
    float factor = map(pol.x, 0.0, 1.0, 1.0, amount);
    pol.x *= factor;
    
    uv = polToCart(pol);
    
    uv.x += uvCenter.x;
    uv.y += uvCenter.y;
    
    vec2 uvAbs = uv * resolution;

    vec3 tc = texture2DRect(inputTexture, uvAbs).rgb;
    
    gl_FragColor = vec4(tc, 1.0);
}
