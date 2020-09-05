#version 120
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec3 normal0;
varying vec2 texCoord0;
uniform mat4 transform;
uniform float hitCount;

void main() {
  vec3 positionStart = vec3(0.0,0.0,0.0);
  //pos.x = pos.x + sin(pos.y*3)*(hitCount/3.0);

  gl_Position = transform * vec4(position, 1.0);
  texCoord0 = texCoord;
  normal0 = (transform * vec4(normal, 0.0)).xyz;
}
