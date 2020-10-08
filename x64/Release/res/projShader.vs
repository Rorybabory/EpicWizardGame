#version 120
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec3 normal0;
varying vec2 texCoord0;
varying vec4 diffuse;
varying vec4 pos;
varying mat4 trans;
varying vec4 colorVert;
uniform mat4 transform;
uniform vec4 color;
uniform float time;
void main() {
  pos = transform * vec4(position, 1.0);
  pos.x = pos.x + sin(pos.y*3 + time/10000.0)*0.35;
  
  gl_Position = pos;
  colorVert = color;
  trans = transform;
  texCoord0 = texCoord;
}
