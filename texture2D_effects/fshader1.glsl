#version 400
in vec4 fColors;
in vec2 fTextCoord;

uniform sampler2D sampler;

out vec4 outfragcolor;

//void main()
//{
//    outfragcolor = texture(sampler,fTextCoord);//*fColors;
//}

// blur effect
void main(void)
{
    float step = 0.005;
    vec3 c1 = texture(sampler, vec2(fTextCoord.s - step, fTextCoord.t - step)).rgb;
    vec3 c2 = texture(sampler, vec2(fTextCoord.s + step, fTextCoord.t + step)).rgb;
    vec3 c3 = texture(sampler, vec2(fTextCoord.s - step, fTextCoord.t + step)).rgb;
    vec3 c4 = texture(sampler, vec2(fTextCoord.s + step, fTextCoord.t - step)).rgb;

    outfragcolor.a = 1.0;
    outfragcolor.rgb = (c1 + c2 + c3 + c4) / 4.0;
}
