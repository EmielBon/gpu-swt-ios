#pragma include TextureUtil.fsh
#pragma include Util.fsh

uniform sampler2D Gradients;
uniform float LowerThreshold;// = 0.08;
uniform float UpperThreshold;// = 0.16;

const float Pi = 3.14159265359;
const float PiOver2 = Pi / 2.0;
const float PiOver4 = Pi / 4.0;
const float PiOver8 = Pi / 8.0;

void main()
{
    ivec2 pos      = ivec2(gl_FragCoord.xy);
    vec2  gradient = fetch(Gradients, pos).xy;
    /*
    float angle     = gradient.x;
    float magnitude = gradient.y;
    
    // todo: make constant for a
    vec4 a = vec4(PiOver8, 3.0 * PiOver8, 5.0 * PiOver8, 7.0 * PiOver8);
    vec4 b = vec4(angle);
    vec4 r = step(a, b);
    float binned = dot(r, vec4(PiOver4));
    
    // todo: is round implementation dependant?
    vec2 binnedGradient = round( vec2(cos(binned), sin(binned)) );
    ivec2 dir = ivec2(sign(binnedGradient));
    
    float  forwardNeighbourGradient = fetch(Gradients, pos + dir).g;
    float backwardNeighbourGradient = fetch(Gradients, pos - dir).g;
    
    float sum = 0.0;
    // todo: seprarable convolution (averaging) operation
    for (int i = -1; i <= 1; ++i)
    for (int j = -1; j <= 1; ++j)
        // todo: look at lerp instead of smoothstep
        sum += smoothstep(LowerThreshold, UpperThreshold, fetch(Gradients, pos + ivec2(i, j)).g);
    
    bool localMaximum     = magnitude > forwardNeighbourGradient && magnitude > backwardNeighbourGradient;
    bool strongEdge       = magnitude > UpperThreshold;
    bool weakEdge         = magnitude >= LowerThreshold;
    bool acceptedWeakEdge = weakEdge && sum / 9.0 >= UpperThreshold;
    bool isEdgePixel      = localMaximum && (strongEdge || acceptedWeakEdge);
    
    //if (!isEdgePixel)
    //    discard; // used to directly build the stencil buffer
    //gl_FragColor = vec4(1, 1, 1, 1);
    if (!isEdgePixel)
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    else
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    //gl_FragColor = vec4(int(isEdgePixel));*/
    gl_FragColor = vec4(gradient, 0, 1);
}