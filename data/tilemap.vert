#version 330

uniform mat3 pvmMatrix;

layout(location = 0) in vec2 pos;

out vec2 tilemapUv;

void main() {
	gl_Position = vec4(pvmMatrix * vec3(pos, 1.0f), 1.0);
	tilemapUv = pos;
}
