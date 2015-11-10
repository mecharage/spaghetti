#version 330

uniform usampler2D tilemap;

in vec2 tilemapUv;

out vec4 col;

ivec2 tilemapSize = textureSize(tilemap, 0);

void main() {
	uint pix = texelFetch(tilemap, ivec2(tilemapUv), 0).r;

	col = pix == 1u ? vec4(0.0, 0.0, 0.0, 1.0) : vec4(1.0);
}
