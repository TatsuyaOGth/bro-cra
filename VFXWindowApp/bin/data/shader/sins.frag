// Guyver
#ifdef GL_ES
precision mediump float;
#endif

// This should be the starting point of every effect - dist
// i have a fixation with lines - dist

uniform float time;
uniform float lowFreq;
uniform vec2 mouse;
uniform vec2 resolution;


vec3 SUN_1 = vec3(1.0, 1.0, 1.0);
vec3 SUN_2 = vec3(1.0, 1.0, 1.0);
vec3 SUN_3 = vec3(1.0, .8, 0.9);
vec3 SUN_4 = vec3(.0, 1.0, 1.0);

float sigmoid(float x) {
  return 2./(1. + exp2(-x)) - 1.;
}


void main( void ) {
  vec2 position = gl_FragCoord.xy;
  vec2 aspect = vec2(resolution/resolution );
  position -= 0.5*resolution;
  vec2 position2 = 0.5 + (position-0.5)/resolution*3.;
  float filter = sigmoid(pow(2.,7.5)*(length((position/resolution + 0.5)*aspect) - 0.015))*0.5 +0.5 +lowFreq*lowFreq;
  position = mix(position, position2, filter) - 0.5;

  vec3 color = vec3(0.);
  float angle = atan(position.y,position.x);
  float d = length(position);
  
  float v = mouse.x;
  color += 0.07/length(vec2(.04,3.*position.y+sin(position.x*10.+time*6.)+5.*position.y+sin(position.x*10.+time*10.)+7.*position.y+sin(position.x*10.+time*14.)))*SUN_1;
  color += 0.06/length(vec2(.06,3.*position.y+sin(position.x*10.+time*6.)+1.*position.y+sin(position.x*10.+time*3.)+2.*position.y+sin(position.x*5.+time*10.)))*SUN_2;
  color += 0.05/length(vec2(.10,5.*position.y+sin(position.x*10.+time*10.)+2.*position.y+sin(position.x*10.+time*5.)+3.*position.y+sin(position.x*10.+time*6.)))*SUN_3;
  color += 0.1/length(vec2(.14,7.*position.y+sin(position.x*10.+time*14.)+.5*position.y+sin(position.x*10.+time*10.)+.5*position.y+sin(position.x*900.+time*.5)))*SUN_4;
  color += v/length(vec2(.14,7.*position.y+sin(position.x*10.+time*1.)+.5*position.y+sin(position.x*10.+time*10.)+.5*position.y+sin(position.x*900.+time*.5)))*SUN_1;
  gl_FragColor = vec4(color, 1.0);
  //gl_FragColor = vec4(filter<1.);
}
