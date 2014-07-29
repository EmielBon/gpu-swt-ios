#pragma include TextureUtil.fsh
#pragma include Util.fsh

uniform sampler2D Texture;

const vec2 weights = vec2(-1.0,/*0,*/1.0);

void main()
{
    vec2 pos = vec2(gl_FragCoord.xy);
    vec2 offset = vec2(0.0, 1.0);
    vec2 hResult, vResult;
    
    hResult[0] = fetch(Texture, pos - offset.yx).r;
    vResult[0] = fetch(Texture, pos - offset.xy).g;
    
    hResult[1] = fetch(Texture, pos + offset.yx).r;
    vResult[1] = fetch(Texture, pos + offset.xy).g;
    
    float vHor = dot(hResult, weights);
    float vVer = dot(vResult, weights);
    
    // Gradients can potentially range between [-16, 16] with the Scharr operator, this normalizes that range to [-1, 1]
    vec2 gradient = vec2(vHor, vVer) / /*16*/5.0;
    float angle = max(atan2(gradient), atan2(-gradient));
    
    gl_FragColor = vec4(angle, length(gradient), 0, 1);
}