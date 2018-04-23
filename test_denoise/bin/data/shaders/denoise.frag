#version 120

uniform sampler2DRect inputTexture;
uniform vec2 resolution;

float sigma_spatial=0.05;
float sigma_color=0.1;
float filter_window=0.2;

float random(vec3 scale, float seed) {
    return fract(sin(dot(gl_FragCoord.xyz + seed, scale)) * 43758.5453 + seed);
}

vec3 NLFilter(vec2 uv, float sigma_spatial,float sigma_color)
{
	float wsize = filter_window/14.0;
    vec3 res_color = vec3(0.0,0.0,0.0);
    float res_weight = 0.0;
    vec3 center_color=texture2DRect(inputTexture,uv*resolution).rgb;
    float sigma_i=0.5*wsize*wsize/sigma_spatial/sigma_spatial;
    float offset = random(vec3(12.9898, 78.233, 151.7182), 0.0);
    float offset2 = random(vec3(112.9898, 178.233, 51.7182), 0.0);
    float regionRadius = 4.0;    
    for (float i = -regionRadius; i <= regionRadius; i++) {
        for (float j= -regionRadius; j<= regionRadius; j++) {
            vec2 uv_sample = uv+vec2(float(i+offset-0.5)*wsize,float(j+offset-0.5)*wsize);
        	vec3 tmp_color = texture2DRect(inputTexture,uv_sample*resolution).rgb;   
            vec3 diff_color = (tmp_color-center_color);
            float tmp_weight = exp(-(i*i+j*j)*sigma_i);
            tmp_weight *= exp(-(dot(diff_color,diff_color)/2.0/sigma_color/sigma_color));
            res_color += tmp_color * tmp_weight;
            res_weight += tmp_weight;   
        }
    }
    vec3 res = res_color/res_weight;
    return res;
}

void main() {
    vec2 uvAbs = gl_FragCoord.xy;

    vec2 uv = uvAbs / resolution;

    vec3 col = texture2DRect(inputTexture,uv*resolution).rgb;

    vec3 result = NLFilter(uv, sigma_spatial, sigma_color);

    result = mix(result, col, uv.y*uv.y);

    // vec3 tc = texture2DRect(inputTexture, uvAbs).rgb;
    
    // tc.r *= 2.0;
    
    gl_FragColor = vec4(result, 1.0);
}