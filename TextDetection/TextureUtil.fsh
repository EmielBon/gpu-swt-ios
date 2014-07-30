precision mediump int;
precision highp   float;

vec2 size(sampler2D sampler)
{
    // todo: remove hardcoding, let shader compiler determine this
    return vec2(800, 600);//textureSize(sampler, 0);
}

vec2 getScreenSpaceCoord(sampler2D sampler, vec2 screenCoord)
{
    // todo: no idea why the +0.5 only with x and not with y
    vec2 pos = screenCoord + vec2(0.5, 0.5);
    return (pos / (size(sampler) - vec2(0.5))) * vec2(2) - vec2(1);
}

vec2 getScreenSpaceTexCoord(sampler2D sampler, vec2 screenCoord)
{
    //return (screenCoord + vec2(0.5)) / (size(sampler));
    return (getScreenSpaceCoord(sampler, screenCoord) + vec2(1.0)) / 2.0;
}

vec4 sample(sampler2D sampler, vec2 screenCoord)
{
    return texture2D(sampler, getScreenSpaceTexCoord(sampler, screenCoord)/*xy / size(sampler)*/);
}

vec4 fetch(sampler2D sampler, ivec2 xy)
{
    return sample(sampler, vec2(xy));
}

vec4 fetch(sampler2D sampler, vec2 xy)
{
    return fetch(sampler, ivec2(xy));
}