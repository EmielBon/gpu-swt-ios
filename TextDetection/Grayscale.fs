#pragma include TextureUtil.fsh

uniform sampler2D Texture;

void main()
{
    mediump vec3 color   = fetch(Texture, gl_FragCoord.xy).rgb;
    mediump vec3 weights = mediump vec3(0.2126, 0.7152, 0.0722);
    mediump vec3 gray    = mediump vec3(dot(color, weights));
    gl_FragColor = lowp vec4(gray, 1);
}