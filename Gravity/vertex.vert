#version 110

varying vec3 v_pos3;
varying vec3 v_nor;

void main()
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;

	v_pos3 = vec3(pos);
	v_nor = normalize(gl_NormalMatrix * gl_Normal);

	gl_Position = gl_ProjectionMatrix * pos;
}
