vec2 swizzleIf(bool condition, vec2 v)
{
    return mix(v.xy, v.yx, float(condition));
}

ivec2 swizzleIf(bool condition, ivec2 v)
{
    return ivec2(swizzleIf(condition, vec2(v)));
}

float ifelse(bool condition, float a, float b)
{
    return mix(a, b, float(!condition));
}

int ifelse(bool condition, int a, int b)
{
    return int(ifelse(condition, float(a), float(b)));
}

vec2 ifelse(bool condition, vec2 a, vec2 b)
{
    return mix(a, b, float(!condition));
}

ivec2 ifelse(bool condition, ivec2 a, ivec2 b)
{
    return ivec2(ifelse(condition, vec2(a), vec2(b)));
}

vec4 ifelse(bool condition, vec4 a, vec4 b)
{
    return mix(a, b, float(!condition));
}

float getRatio(float x, float y)
{
    return max(x / y, y / x);
}