#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 out_texCoord;

uniform mat4 un_model;
uniform mat4 un_view;
uniform mat4 un_projection;

void main() {
	out_texCoord = aTexCoord;
	gl_Position =  un_projection * un_view * un_model * vec4(aPos, 1.0);
}