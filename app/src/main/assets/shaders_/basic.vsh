attribute vec3 pos;  // in local coords
attribute vec3 nextPos;  // in local coords
attribute vec2 texCoord;

varying vec2 TexCoord;

uniform mat4 mvpmat;
uniform float interpolate;

void main()
{
	float interpolatedDeltaX = (nextPos.x - pos.x) * interpolate;
	float interpolatedDeltaY = (nextPos.y - pos.y) * interpolate;
	float interpolatedDeltaZ = (nextPos.z - pos.z) * interpolate;
	vec3 interpolatedPos = vec3(pos.x + interpolatedDeltaX, pos.y + interpolatedDeltaY, pos.z + interpolatedDeltaZ);
	gl_Position = mvpmat * vec4(interpolatedPos, 1.0);
	TexCoord = texCoord;
}
