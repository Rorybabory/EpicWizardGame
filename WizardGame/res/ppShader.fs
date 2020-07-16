#version 330 core

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
uniform sampler2D sampler;
uniform sampler2D depth;
uniform vec4 color;
uniform int inverted;
uniform float brightness;
uniform vec2 entityPos[];
uniform float entityPosSize;
uniform float red;
uniform vec3 hsv;
struct Box {
	vec2 pos;
	vec2 scale;
	vec3 color;
};
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
vec3 createBox(vec2 uv, vec3 value, Box box) {
	if (uv.x < box.pos.x+box.scale.x/2.0 && uv.x > box.pos.x-box.scale.x/2.0 &&
		uv.y < box.pos.y+box.scale.y/2.0 && uv.y > box.pos.y-box.scale.y/2.0) {
		return vec3(0.0,1.0,0.0);
	}
	return value;
}
void main() {
    vec3 value = texture(sampler,texCoord0).rgb;
    if (inverted == 1.0) {
        value = vec3(1.0)-value;
    }
	vec3 hsvValue = rgb2hsv(value) + hsv;
	vec3 newValue = hsv2rgb(hsvValue);
	vec3 depth = texture(depth, texCoord0.st).rgb;
	
	vec3 finalValue = newValue+vec3(brightness);
	float redGradient = (1.0-texCoord0.y)*red;
	finalValue.r += redGradient/2.0;
	gl_FragColor = vec4(finalValue,1.0);
}
