#version 120

uniform float speed;

varying vec4 fragPos;

void main(void)
{
	bool x = bool(mod(int(fragPos.x*1.3+10), 2));
	bool y = bool(mod(int(fragPos.y), 2));
	bool z = bool(mod(int(fragPos.z*0.5-speed), 2));

	bool xorXY = x!=y;

	if (xorXY != z){
		gl_FragColor = vec4(0.25f, 0.25f, 0.8f, 1.0f);
	} else {
		gl_FragColor = vec4(0.f, 0.f, 0.2f, 1.0f);
	}
}