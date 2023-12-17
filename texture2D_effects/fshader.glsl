#version 400
in vec4 fColors;
in vec2 fTextCoord;

uniform sampler2D sampler;

out vec4 outfragcolor;

void main()
{
    outfragcolor = texture(sampler,fTextCoord); //*fColors;
}
