#pragma include TextureUtil.fsh

uniform sampler2D Texture;

void main()
{
    vec3 color   = fetch(Texture, gl_FragCoord.xy).rgb;
    vec3 weights = vec3(0.2126, 0.7152, 0.0722);
    vec3 gray    = vec3(dot(color, weights));
    gl_FragColor = vec4(gray, 1);
}