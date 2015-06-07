#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

/*
 * inspired by http://www.fractalforums.com/new-theories-and-research/very-simple-formula-for-fractal-patterns/
 * a slight(?) different 
 * public domain
 */

#define N 60
void main( void ) {
	vec2 v = (gl_FragCoord.xy - resolution/2.0) / min(resolution.y,resolution.x) * 20.0;
	vec2 v2 = v; vec2 v3 = v;
	
	float rsum = 0.0;
	float rsum2 = 0.0;
	float rsum3 = 0.0;
	float pi2 = 3.1415926535 * 2.0;
	float a = (.5-mouse.x)*pi2;
	float C = cos(a);
	float S = sin(a);
	vec2 xaxis=vec2(C, -S);
	vec2 yaxis=vec2(S, C);
	#define MAGIC 0.618
	vec2 shift = vec2( 0, 1.0+MAGIC);
	float zoom = 1.0 + mouse.y*8.0;
	float zoom2 = 1.0 + (mouse.y)*7.9;
	float zoom3 = 1.0 + (mouse.y)*8.1;
	
	for ( int i = 0; i < N; i++ ){
		float rr = dot(v,v);
		if ( rr > 1.0 )
		{
			rr = (1.0)/rr ;
			v.x = v.x * rr;
			v.y = v.y * rr;
		}
		float rr2 = dot(v2,v2);
		if ( rr2 > 1.0 )
		{
			rr2 = (1.0)/rr2 ;
			v2.x = v2.x * rr2;
			v2.y = v2.y * rr2;
		}
		float rr3 = dot(v3,v3);
		if ( rr3 > 1.0 )
		{
			rr3 = (1.0)/rr3 ;
			v3.x = v3.x * rr3;
			v3.y = v3.y * rr3;
		}
		rsum *= .99;
		rsum += rr;
		rsum2 *= .99;
		rsum2 += rr2;
		rsum3 *= .99;
		rsum3 += rr3;
		
		v = vec2( dot(v, xaxis), dot(v, yaxis)) * zoom + shift;
		v2 = vec2( dot(v2, xaxis), dot(v2, yaxis)) * zoom2 + shift;
		v3 = vec2( dot(v3, xaxis), dot(v3, yaxis)) * zoom3 + shift;
	}
	float col1 = ((mod(rsum,2.0)>1.0)?1.0-fract(rsum):fract(rsum));
	float col2 = ((mod(rsum2,2.0)>1.0)?1.0-fract(rsum2):fract(rsum2));
	float col3 = ((mod(rsum3,2.0)>1.0)?1.0-fract(rsum3):fract(rsum3));
	gl_FragColor = vec4(col3, col1, col2, 1.0); 
}
