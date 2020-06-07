#version 330 core
in vec3 TexCoords;

varying vec3 texCoord0;
varying vec3 normal0;
varying vec3 position0;
uniform samplerCube skybox;
uniform vec4 color;
uniform int inverted;
void main() {
    gl_FragColor = texture(skybox, TexCoords);
//	gl_FragColor = vec4(vec3(TexCoords.y),1.0);
}
