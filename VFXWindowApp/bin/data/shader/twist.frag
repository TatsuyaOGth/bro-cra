#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
varying vec2 surfacePosition;

const float pi= 3.14159265359;
const vec3 c1=vec3(1.,1.,1.);
const vec3 c2=vec3(0.9,0.9,0.9);
const float spinspeed=0.2;
const float spirals=1.;
const float twist = 8.;
const float patternPeriod = 1.;

void main(void)
{
  vec2 mm = vec2(0,resolution.y);
  vec2 uv = ((gl_FragCoord.xy / resolution.yy) - vec2(1.0, 0.5));
  //vec2 uv = resolution.yx;
  vec3 color;
  
  float l = length(uv);
  float pos = atan(uv.x, uv.y);
  float spin = fract(time / spinspeed) * pi;
  float distortion = twist / sqrt(l);
  float f = fract(((pos + spin + distortion) / pi) * spirals) * (mouse.x * 20. + 0.5);
  
  if(f<1.)
    color = c1;
  else
    color = c2;
  
  //f = abs(fract(f - 0.5) * 2. - 1.);
  //f = clamp(sqrt(-f * f + 3. * f) * sqrt(l), 0., 1.);
  gl_FragColor = vec4(f * l * color, 1.);
}