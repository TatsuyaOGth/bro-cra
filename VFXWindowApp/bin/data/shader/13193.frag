#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
varying vec2 surfacePosition;

const float pi= 3.14159265359;
const vec3 c1=vec3(0.9,0.9,0.9);
const vec3 c2=vec3(0.9,0.1,0.1);
const float spinspeed=1.0;
const float spirals=3.;
const float twist = 3.3;
const float patternPeriod = 1.5;

void main(void)
{
	vec2 mm = vec2(0,resolution.y/2.);
	vec2 uv = ((gl_FragCoord.xy / resolution.yy ) - mouse );
	vec3 color;
	
	float l = length(uv);
	float pos = atan(uv.x, uv.y);
	float spin = fract(time / spinspeed) * pi;
	float distortion = twist / sqrt(l);
	float f = fract(((pos + spin + distortion) / pi) * spirals) * patternPeriod;
	
	if(f<1.)
		color = c1;
	else
		color = c2;
	
	//f = abs(fract(f - 0.5) * 2. - 1.);
	//f = clamp(sqrt(-f * f + 3. * f) * sqrt(l), 0., 1.);
	gl_FragColor = vec4(f * l * color, 1.);
}