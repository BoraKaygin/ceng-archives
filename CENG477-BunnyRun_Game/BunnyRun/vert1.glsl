#version 120 

attribute vec3 inVertex;
attribute vec3 inNormal;

uniform mat4 modelingMat;
uniform mat4 modelingMatInvTr;
uniform mat4 perspectiveMat;

varying vec4 fragPos;

void main(void)
{

	vec4 p = modelingMat * vec4(inVertex, 1); // translate to world coordinates
	
	fragPos = p;

    gl_Position = perspectiveMat * modelingMat * vec4(inVertex, 1);
}
