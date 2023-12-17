#version 400

flat in vec4 inoutColor;

out vec4 outfragcolor;

void main()
{
    outfragcolor = inoutColor;
}
