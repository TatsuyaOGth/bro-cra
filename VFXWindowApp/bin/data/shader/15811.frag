#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main( void ) {

	vec2 position = gl_FragCoord.xy;
	float color;
	
	vec2 lightpos = resolution * mouse;
	
	vec2 norm = lightpos - position;
	float sdist = norm.x * norm.x + norm.y * norm.y;
	vec3 light_color = vec3(1.0,0.6,1.0);
	
	color = 1.0 / (sdist * 0.001);

	gl_FragColor = vec4(color,color,color,1.0)*vec4(light_color,1.0);

}
