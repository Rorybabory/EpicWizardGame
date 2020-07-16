#version 330 core

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
uniform sampler2D sampler;
uniform vec4 color;
uniform int inverted;
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

const float offset = 1.0 / 300.0;
void main() {
    vec2 tex_offset = 1.0 / textureSize(sampler, 0); // gets size of single texel
    vec3 result = texture(sampler, texCoord0).rgb * weight[0]; // current fragment's contribution

	if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampler, texCoord0 + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(sampler, texCoord0 - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(sampler, texCoord0 + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(sampler, texCoord0 - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
	if (result.r > 0.17 && 
		result.g > 0.17 && 
		result.b > 0.17) {
			result = vec3(0,0,0);
	}

    gl_FragColor = vec4(result, 1.0);
}
