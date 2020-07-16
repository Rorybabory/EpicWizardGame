#version 120
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec3 normal0;
varying vec2 texCoord0;
varying vec4 diffuse;
varying vec4 pos;
uniform mat4 transform;
uniform vec4 color;

void main() {
  pos = transform * vec4(position, 1.0);
  gl_Position = transform * vec4(position, 1.0);
  texCoord0 = texCoord;
  vec3 lDirection = vec3(-1,0,0);
  vec3 lColor = color.xyz;
  normal0 = (transform * vec4(normal, 0.0)).xyz;
  diffuse = color*vec4(max(dot(lDirection, -normal0), 0) * lColor, 1);
}
