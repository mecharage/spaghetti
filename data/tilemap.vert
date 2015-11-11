#version 330

uniform vec2 screenScale;

layout(location = 0) in vec2 pos;

out vec2 tilemapUv;

void main() {
	gl_Position = vec4(vec2(-1.0, 1.0) + vec2(2.0, -2.0) * screenScale * pos, 0.0, 1.0);
	tilemapUv = pos;
}
