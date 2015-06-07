#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
varying vec2 surfacePosition;
uniform sampler2D bk;

float sfract(float n){
  return smoothstep(0.0, 1.0,fract(n));
}
float rand(vec3 n){
  return fract(abs(sin(dot(n,vec3(2.2,-5.4,6.5))))*456.5+4.321);   
}
float noise(vec3 n){
    float h1 = mix(rand(vec3(floor(n.x),floor(n.y),floor(n.z))),rand(vec3(ceil(n.x),floor(n.y),floor(n.z))),sfract(n.x));
    float h2 = mix(rand(vec3(floor(n.x),ceil(n.y),floor(n.z))),rand(vec3(ceil(n.x),ceil(n.y),floor(n.z))),sfract(n.x));
    float s1 = mix(h1,h2,sfract(n.y));
    
    h1 = mix(rand(vec3(floor(n.x),floor(n.y),ceil(n.z))),rand(vec3(ceil(n.x),floor(n.y),ceil(n.z))),sfract(n.x));
    h2 = mix(rand(vec3(floor(n.x),ceil(n.y),ceil(n.z))),rand(vec3(ceil(n.x),ceil(n.y),ceil(n.z))),sfract(n.x));
    float s2 = mix(h1,h2,sfract(n.y));
    return mix(s1,s2,fract(n.z));
}
void main(void){
    float c = (noise((vec3(gl_FragCoord.xy/2.0,time*9.)))-noise(vec3(gl_FragCoord.xy/(mouse.x*200.0+1.),-time*4.)));
  gl_FragColor = vec4(c,abs(c)*.0,abs(-c)*1.0,1.0);
}