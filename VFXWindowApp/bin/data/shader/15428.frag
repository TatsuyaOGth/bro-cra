// Shader by Nicolas Robert [NRX]
// Latest version: http://glsl.heroku.com/e#15300
// Forked from: http://glsl.heroku.com/e#15286
//
// You can also check http://glsl.heroku.com/e#15072.

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;

float iGlobalTime = time;
vec3 iResolution = vec3 (resolution, 0.0);

#define DELTA			0.01
#define RAY_LENGTH_MAX		50.0
#define RAY_STEP_MAX		500
#define LIGHT			vec3 (0.0, -0.1, 0.0)
#define AMBIENT			0.1
#define SPECULAR_POWER		4.0
#define SPECULAR_INTENSITY	0.2
#define M_PI			3.1415926535897932384626433832795

vec3 vRotateX (in vec3 p, in float angle) {
	float c = cos (angle);
	float s = sin (angle);
	return vec3 (p.x, c * p.y + s * p.z, c * p.z - s * p.y);
}

vec3 vRotateY (in vec3 p, in float angle) {
	float c = cos (angle);
	float s = sin (angle);
	return vec3 (c * p.x - s * p.z, p.y, c * p.z + s * p.x);
}

float fixDistance (in float d, in float correction, in float k) {
	correction = max (correction, 0.0);
	k = clamp (k, 0.0, 1.0);
	return min (d, max ((d - DELTA) * k + DELTA, d - correction));
}

float smin (float a, float b, float k) {

	// From http://www.iquilezles.org/www/articles/smin/smin.htm
	float h = clamp (0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	return mix (b, a, h) - k * h * (1.0 - h);
}

float getDistance (in vec3 p) {
	float k = 0.2 + 0.2 * sin (p.y * 3.0 + iGlobalTime * 3.0);
	float body = fixDistance (length (p) + k - 4.5, 0.5, 0.8);

	p.y += 1.0;
	float angle = 2.0 * M_PI / 12.0;
	vec3 pp = vRotateY (p, -angle * 0.5);
	angle *= floor (atan (pp.x, pp.z) / angle);
	p = vRotateY (p, angle);
	p = vRotateX (p, p.z * 0.02 + 0.1 * sin (p.z * 0.3 + iGlobalTime + angle * 3.0));
	p = vRotateY (p, 0.1 * sin (p.z * 0.2 + iGlobalTime * 2.0 + angle * 7.0));
	p.z = sin (p.z * 2.0) / 2.0;
	float tentacle = fixDistance (length (p) - 1.0 - 0.5 * sin (abs (p.x * p.y * p.y* p.y + p.z)), 20.0, 0.1);

	return smin (body, tentacle, 0.3);
}

void main () {

	// Define the ray corresponding to this fragment
	vec2 frag = (2.0 * gl_FragCoord.xy - iResolution.xy) / iResolution.y;
	vec3 direction = normalize (vec3 (frag, 2.0));

	// Set the camera
	vec3 origin = vec3 ((40.0 * cos (iGlobalTime * 0.4)), -3.0 + 16.0 * sin (iGlobalTime), 35.0 * sin (iGlobalTime * 0.3));
	vec3 forward = -origin;
	vec3 up = vec3 (0.0, 1.0, 0.0);
	mat3 rotation;
	rotation [2] = normalize (forward);
	rotation [0] = normalize (cross (up, forward));
	rotation [1] = cross (rotation [2], rotation [0]);
	direction = rotation * direction;

	// Ray marching
	vec3 p = origin;
	float dist = RAY_LENGTH_MAX;
	float rayLength = 0.0;
	for (int rayStep = 0; rayStep < RAY_STEP_MAX; ++rayStep) {
		dist = getDistance (p);
		rayLength += dist;
		if (dist < DELTA || rayLength > RAY_LENGTH_MAX) {
			break;
		}
		p = origin + direction * rayLength;
	}

	// Compute the fragment color
	vec3 lightDirection = normalize (LIGHT);
	vec4 backColor = vec4 (0.0, 0.0, 0.0, 1.0);
	if (dist < DELTA) {

		// Get the normal
		vec2 h = vec2 (DELTA, 0.0);
		vec3 normal = normalize (vec3 (
			getDistance (p + h.xyy) - getDistance (p - h.xyy),
			getDistance (p + h.yxy) - getDistance (p - h.yxy),
			getDistance (p + h.yyx) - getDistance (p - h.yyx)));

		// Lighting
		vec3 reflectDirection = reflect (direction, normal);
		float diffuse = max (0.0, dot (normal, lightDirection));
		float specular = pow (max (0.0, dot (reflectDirection, lightDirection)), SPECULAR_POWER) * SPECULAR_INTENSITY;
		gl_FragColor = mix (backColor, ((AMBIENT + diffuse) * vec4 (1.0, 0.4, 0.0, 1.0) + specular),1.0);
	}
	else {
		gl_FragColor = backColor;
	}
}
