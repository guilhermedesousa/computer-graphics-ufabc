#version 400

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;
uniform vec4 lightPosition;

out vec3 fN;
out vec3 fV;
out vec3 fL;
out vec2 ftextCoord;

void main()
{
    vec4 eyePosition = view * model * vPosition;
    fN = mat3(view) * normalMatrix * vNormal;
    fL = lightPosition.xyz - eyePosition.xyz;
    fV = -eyePosition.xyz;
    gl_Position = projection * eyePosition;
    ftextCoord = vTextCoords;
}
