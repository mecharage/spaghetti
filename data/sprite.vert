#version 330

uniform vec2 screenScale;
uniform mat3 modelMatrix;

layout(location = 0) in vec2 iUv;
//layout(location = 1) in vec4 iVcol;
//layout(location = 2) in mat3 iModelMat;
//layout(location = 5) in vec2 iTlUv;
//layout(location = 6) in vec2 iBrUv;
//layout(location = 7) in vec4 iColor;

out vec2 vertexUv;
out vec2 mapUv;
//out vec2 spriteTlUv;
//out vec2 spriteBrUv;
//out vec4 spriteColor;

void main() {
	vec3 pos = modelMatrix * vec3(iUv, 1.0);
	mapUv = pos.xy;
	gl_Position = vec4(vec2(-1.0, 1.0) + vec2(2.0, -2.0) * screenScale * pos.xy, 0.0, 1.0);
	vertexUv = iUv;
//	spriteTlUv = iTlUv;
//	spriteBrUv = iBrUv;
//	spriteColor = iVcol;
}
