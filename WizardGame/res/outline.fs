#version 120

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
uniform sampler2D sampler;
uniform vec4 color;
uniform int inverted;
float near = 0.2; 
float far  = 2000.0; 
  

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}


void main() {
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    gl_FragColor = color;
}
