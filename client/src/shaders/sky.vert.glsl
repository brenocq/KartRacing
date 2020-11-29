#version 330 core
layout (location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    texCoords = position;
	vec4 pos = projection * view * vec4(position*1000, 1.0);
    gl_Position = pos.xyww;
}  
