#version 400

in vec2 ftexCoord;

uniform sampler2D colorTexture;

out vec4 outColor;

void main(void) {
    outColor =  texture(colorTexture, ftexCoord);
}
