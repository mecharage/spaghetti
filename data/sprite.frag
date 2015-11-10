#version 330

uniform sampler2D spritesheet;

in vec2 vertexUv;
in vec2 spriteTlUv;
in vec2 spriteBrUv;
in vec4 spriteColor;

out vec4 color;

void main() {
	vec2 uv = mix(spriteTlUv, spriteBrUv, vertexUv);
	color = spriteColor * texture(spritesheet, uv);
}
