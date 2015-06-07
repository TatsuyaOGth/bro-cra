#ifdef GL_ES
  precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

//---------------------------------------------------------
void main( void ) 
{
  float scale = resolution.y / (20.0 + mouse.x*100.0);
  float ring = 25.0 + floor((- 0.5)* 20.0);
  float radius = resolution.x*1.0;
  float gap = scale*.5;
  float test = 0.3;
  vec2 pos = (gl_FragCoord.xy - resolution.xy*.5) * test;
  
  float d = length(pos);
  
  // Create the wiggle
  d += (sin(pos.y*.25/scale+time)*sin(pos.x*0.5/scale+time*.5))*scale*(sin(time)*13.);
  
  // Compute the distance to the closest ring
  float v = mod(d + radius/(ring*2.0), radius/ring);
  v = abs(v - radius/(ring*4.0));
  
  v = clamp(v-gap, 0.0, 1.0);
  
  d /= radius;
  vec3 m = fract((time*0.01)*vec3(ring*-1.3, -ring, ring*(time*0.005)));
  
  gl_FragColor = vec4( m*v, 1.0 );
}