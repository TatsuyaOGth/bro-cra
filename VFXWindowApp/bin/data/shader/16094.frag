#ifdef GL_ES
precision mediump float;
#endif

uniform float time;

uniform vec2 resolution;

#define PI 0.01

void main( void ) {

	vec2 p = ( gl_FragCoord.xy / resolution.xy ) - 0.5;
	
	float sx = 0.2*sin( 25.0 * p.x - time);
	
	float dy = 0.9/ ( 50. * abs(p.y - sx));
	
	gl_FragColor = vec4( (p.x + 0.5) * dy, 0.5 * dy, dy-1.35, 100.0 );

}