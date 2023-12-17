#version 400

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vcoordText;

out vec2 ftexCoord;

void main() {
    gl_Position = vPosition;
    ftexCoord = vcoordText;
}
