#pragma include TextureUtil.fsh

uniform sampler2D Texture;

void main()
{
    gl_FragColor = fetch(Texture, gl_FragCoord.xy);
}