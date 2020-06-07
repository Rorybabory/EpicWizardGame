#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec3 normal0;
varying vec3 texCoord0;
uniform mat4 transform;
void main() {
  vec4 pos = transform * vec4(aPos,1.0);
  pos.y -= 1.0;
  gl_Position = pos.xyzw;
  TexCoords = aPos;
  normal0 = (transform * vec4(normal, 0.0)).xyz;
}
