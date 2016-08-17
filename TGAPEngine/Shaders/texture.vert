#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;
uniform mat4 matrix;
out vec2 texCoords;
void main()
{
    texCoords = texcoord;
    gl_Position =  matrix * position;

}
