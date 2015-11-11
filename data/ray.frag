#version 330

uniform usampler2D tilemap;

in vec2 tilemapUv;
in vec4 vcol;

out vec4 col;

ivec2 tilemapSize = textureSize(tilemap, 0);

void main() {
	uint pix = texelFetch(tilemap, ivec2(tilemapUv), 0).r;

	col = vec4(pix == 1u ? vec3(0.0, 0.0, 0.0) : vec3(1.0), 1.0/*vcol.w*/);
}
