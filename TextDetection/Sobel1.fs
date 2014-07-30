#pragma include TextureUtil.fsh

uniform sampler2D Texture;

const vec3 weights = vec3(3, 10, 3);

void main()
{
    ivec2 uv = ivec2(gl_FragCoord.xy);
    ivec2 offset = ivec2(0, 1);

    vec3 hResult, vResult;
    
    hResult[0] = fetch(Texture, uv - offset.xy).r;
    vResult[0] = fetch(Texture, uv - offset.yx).r;
    
    hResult[1] = vResult[1] = fetch(Texture, uv).r;
    
    hResult[2] = fetch(Texture, uv + offset.xy).r;
    vResult[2] = fetch(Texture, uv + offset.yx).r;
    
    float vHor = dot(hResult, weights);
    float vVer = dot(vResult, weights);
    
    gl_FragColor = vec4(vec2(vHor, vVer) / 16.0, 0, 1);
}