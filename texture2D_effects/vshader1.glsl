#version 400
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColors;
layout(location = 2) in vec2 vTextCoord;

out vec4 fColors;

out vec2 fTextCoord;

void main()
{
    gl_Position = vPosition;
    fColors = vColors;
    fTextCoord = vTextCoord;
}
