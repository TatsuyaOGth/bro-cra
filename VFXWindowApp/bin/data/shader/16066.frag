#ifdef GL_ES
precision lowp float;
#endif



//uniform vec2 mouse;
uniform vec2 resolution;

uniform float time;
#define PI 3.1414159265359
#define M  0.9
#define D  0.6

#define fmod 0.1

void main()
{
	vec2 p = (gl_FragCoord.xy - 0.5* resolution) / min(resolution.x, resolution.y);
  	vec2 t = vec2(gl_FragCoord.xy / resolution);

	vec3 c = vec3(0);
  	float y=0.;
	float x=0.;
	for(int i = 0; i < 200; i++) {
		float t = float(i) * time * 0.5;
		 x = float(i) / (200.*1./1.3) * cos(0.007*t);
		 y = 1.3 * sin(t * 0.007);
		vec2 o = 0.3 * vec2(x, y);
		float r = fract(y+x);
		float g = 1. - r;
		c += 0.0007 / (length(p-o)) * vec3(r, g, x+y);
	}

	gl_FragColor = vec4(c, 1);
}
