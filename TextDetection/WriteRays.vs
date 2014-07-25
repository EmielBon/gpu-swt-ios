#pragma include Util.fsh
#pragma include Codec.fsh
#pragma include TextureUtil.fsh

uniform sampler2D OppositePositions;

     in  vec3  Position;
flat out float Value;

void main()
{
    ivec2 pos0    = ivec2(Position.xy);
    float pos1_id = fetch(OppositePositions, pos0).r;
    ivec2 pos1    = decode(pos1_id);
    
    Value = ifelse(pos1_id == 0.0, 0, 1000 - distance(pos0, pos1));
    
    vec2 p0 = getScreenSpaceCoord(OppositePositions, pos0);
    vec2 p1 = getScreenSpaceCoord(OppositePositions, pos1);

    // Position.z is 1 if the position needs to be scattered to pos1, 0 otherwise ("scatter" to pos0).
    // mix does x * a + y * (1-a)
    gl_Position = vec4( mix(p1, p0, Position.z), 0, 1);
}