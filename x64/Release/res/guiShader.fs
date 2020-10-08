#version 330 core

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
uniform sampler2D sampler;

void main() {
	vec2 tempCoord = texCoord0;
	tempCoord.y = 1-tempCoord.y;
	gl_FragColor = texture(sampler,tempCoord);
}