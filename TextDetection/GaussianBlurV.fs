#pragma include TextureUtil.fsh

uniform sampler2D Texture;

const vec3 offsets = vec3(0.0, 1.3846153846, 3.2307692308);
const vec3 weights = vec3(0.2270270270, 0.3162162162, 0.0702702703);

void main()
{
    vec2 pos = vec2(gl_FragCoord.xy);
    
    gl_FragColor = sample(Texture, pos) * weights[0];
    
    for(int i = 1; i < 3; ++i)
    {
        vec2 offset = vec2(offsets[i], 0);
        gl_FragColor += sample(Texture, pos + offset) * weights[i];
        gl_FragColor += sample(Texture, pos - offset) * weights[i];
    }
}