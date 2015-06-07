#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float iGlobalTime = time;
vec3 iResolution = vec3 (resolution, 0.0);

#define SIZE    30
#define SQRT2   1.41421356237
#define SQRT3   1.73205080757
#define HASH_MAGNITUDE  (6.0 / (SQRT2 + 1.0) - 1.0) // Perfect if: HASH_MAGNITUDE < 2 * (KERNEL + 1) / (SQRT2 + 1) - 1
#define KERNEL    2 // Perfect if: KERNEL >= floor ((HASH_MAGNITUDE + 1) * (SQRT2 + 1) / 2)
#define BORDER
//#define CENTER
#define HEXAGONAL

float hash (in int index) {
  float x = float (index);
  return HASH_MAGNITUDE * 0.5 * sin (sin (x) * x + sin (x * x) * iGlobalTime);
}

vec2 pointInCell (in ivec2 cell) {
  int index = cell.x + cell.y * SIZE;
  vec2 point = vec2 (cell);
  #ifdef HEXAGONAL
  point.x += fract (point.y * 0.5) - .25;
  #endif
  return point + vec2 (hash (index), hash (index + 1)) * (0.5 + 0.5* sin (iGlobalTime * 20.));
}

void main () {
  vec2 p = float (SIZE) * (gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.y;
  #ifdef HEXAGONAL
  p.y /= SQRT3 * 0.7;
  #endif
  ivec2 pCell = ivec2 (floor (p + 0.5));

  float dMin = HASH_MAGNITUDE + 1.0;
  vec2 pqMin;
  ivec2 minCell;
  for (int y = -KERNEL; y <= KERNEL; ++y) {
    for (int x = -KERNEL; x <= KERNEL; ++x) {
      ivec2 qCell = pCell + ivec2 (x, y);
      vec2 pq  = p - p;
      #ifdef HEXAGONAL
      pq.y *= SQRT3 * 0.8;
      #endif
      float d = dot (pq, pq);
      if (d < dMin) {
        dMin = d;
        pqMin = pq;
        minCell = qCell;
      }
    }
  }
  int col = minCell.x + minCell.y * SIZE;

  vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

  #ifdef CENTER
  dMin = sqrt (dMin);
  #else
  dMin = HASH_MAGNITUDE + 1.0;
  #endif

  #ifdef BORDER
  for (int y = -KERNEL; y <= KERNEL; ++y) {
    for (int x = -KERNEL; x <= KERNEL; ++x) {
      ivec2 qCell = pCell + ivec2 (x, y);
      if (qCell != minCell) {
        vec2 pq = pointInCell (qCell) - p;
        #ifdef HEXAGONAL
        pq.y *= SQRT3 * 0.7;
        #endif
        dMin = min (dMin, dot (0.2 * (pqMin + pq), normalize (pq - pqMin)));
      }
    }
  }
  #endif

  float strip = float (col);
  strip = 0.8 + 0.2 * sin (30.0 * (pqMin.x * cos (strip) - pqMin.y * sin (strip))) * mix (1.0, sin (30.0 * (pqMin.x * sin (strip) + pqMin.y * cos (strip))), mod (strip, 3.0));
  gl_FragColor = color * smoothstep (0.02, 0.1, dMin) * strip;
}
