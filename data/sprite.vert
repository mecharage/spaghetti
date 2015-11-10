#version 330

uniform mat3 pvMatrix;

layout(location = 0) in vec2 iUv;
layout(location = 1) in vec4 iVcol;
layout(location = 2) in mat3 iModelMat;
layout(location = 5) in vec2 iTlUv;
layout(location = 6) in vec2 iBrUv;
layout(location = 7) in vec4 iColor;

out vec2 vertexUv;
out vec2 spriteTlUv;
out vec2 spriteBrUv;
out vec4 spriteColor;

void main() {
	gl_Position = vec4(pvMatrix * iModelMat * vec3(iUv, 1.0f), 1.0);
	vertexUv = iUv;
	spriteTlUv = iTlUv;
	spriteBrUv = iBrUv;
	spriteColor = iVcol;
}
