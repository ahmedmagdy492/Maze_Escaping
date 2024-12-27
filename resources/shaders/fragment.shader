#version 330 core

in vec2 out_texCoord;

uniform sampler2D un_texture;

out vec4 FragColor;

void main() {
	FragColor = texture(un_texture, out_texCoord);
}