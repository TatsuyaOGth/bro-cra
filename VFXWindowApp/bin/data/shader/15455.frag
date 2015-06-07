// 704.2 by @paulofalcao version 1.01

// js1k.com version - http://js1k.com/2014-dragons/demo/1849
// Standalone version - http://www.pouet.net/prod.php?which=62822


#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 resolution;

float t=time;
vec2 iRes=resolution;

float stime=sin(t);
float ctime=cos(t);

float inObj(in vec3 p){
  float oP=length(p);
  p.x=sin(p.x)+stime;
  p.z=sin(p.z)+ctime;
  return float(min(length(p)-1.5-sin(oP-t*4.0),p.y+3.0));
}

void main(void){
  vec2 iRes=vec2(iRes);
  vec2 vPos=-1.0+2.0*gl_FragCoord.xy/iRes;

  float zf=cos(t*0.2)*0.4+0.6;
  vec3 vuv=vec3(stime,1,0);
  vec3 vrp=vec3(sin(t*0.7)*10.0,0,cos(t*0.9)*10.0)*zf; 
  vec3 prp=vec3((sin(t*0.7)*20.0+20.0)*zf,stime*4.0+4.0+3.0,(cos(t*0.6)*20.0+14.0)*zf)+vrp;

  vec3 vpn=normalize(vrp-prp);
  vec3 u=normalize(cross(vuv,vpn));
  vec3 v=cross(vpn,u);
  vec3 scrCoord=vpn+vPos.x*u*iRes.x/iRes.y+vPos.y*v;
  vec3 scp=normalize(scrCoord);

  const vec3 e = vec3(0.1,1
		      ,0);
  const float maxd=80.0;

  float s=0.1;
  vec3 c,p,n;

  float f=-(prp.y-2.5)/scp.y;
  if (f>0.0) p=prp+scp*f;
  else f=maxd;

  vec3 outc=vec3(0,0,0);

  float far=0.0;
  for (int ref=0;ref<=1;ref++){
    if (ref>0){
      scp=reflect(scp,n);
      prp=p;
      s=0.1;f=0.1;
    }

    for(int i=0;i<32;i++){
      f+=s;
      p=prp+scp*f;
      s=inObj(p);
      if (abs(s)<.01||f>maxd||p.y>2.5||(ref>0&&i>16)) break;
    }
   
    if (f<maxd&&p.y<2.5){
      if(p.y<-2.5){
        if (fract(p.x/4.0)>.5)
          if (fract(p.z/4.0)>.5)
            c=vec3(0,0,0);
          else
            c=vec3(1,1,1);
        else
          if (fract(p.z/4.0)>.5)
            c=vec3(1,1,1);
          else
            c=vec3(0,0,0);
        c=c*max(inObj(vec3(p.x,p.y+1.0,p.z)),0.5);
        n=vec3(0,1,0);
      }
      else{
        float d=length(p);
        c=vec3((sin(d*.25-t*4.0)+1.0)/2.0,
               (stime+1.0)/2.0,
               (sin(d-t*4.0)+1.0)/2.0);
        n=normalize(
          vec3(s-inObj(p-e.xyy),
               s-inObj(p-e.yxy),
               s-inObj(p-e.yyx)));
      }
      float b=dot(n,normalize(prp-p));
      if (ref==0) {
        outc=((b+0.2)*c+pow(b,54.0))*0.7;
        far=1.0-f*.01;
      } else {
        if (prp.y>-2.4) outc+=(b+0.2)*c*0.3;
      }
    }
    else break;
  }
  gl_FragColor=vec4(outc*far,1.0);
}
