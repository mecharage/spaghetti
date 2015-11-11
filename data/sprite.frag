#version 330

uniform sampler2D spritesheet;
uniform usampler2D tilemap;
uniform int frame;

int txsz = textureSize(spritesheet, 0).y;

in vec2 mapUv;
in vec2 vertexUv;
//in vec2 spriteTlUv;
//in vec2 spriteBrUv;
//in vec4 spriteColor;

out vec4 color;

void main() {
	vec2 tlUv = vec2(float(frame), 0.0), brUv = tlUv + vec2(1.0);
	vec2 uv = mix(tlUv, brUv, vertexUv);

	uint pix = texelFetch(tilemap, ivec2(mapUv), 0).r;
	vec3 col = pix != 0u ? vec3(0.0) : vec3(1.0);
	color =	vec4(col, texelFetch(spritesheet, ivec2(txsz * uv), 0).w);
}
