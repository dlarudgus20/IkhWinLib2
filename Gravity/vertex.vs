#version 110

void main()
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	vec3 pos3 = vec3(pos);
	vec3 nor = normalize(gl_NormalMatrix * gl_Normal);

	vec3 lightdir_no_normal = vec3(gl_LightSource[0].position) - pos3;
	float distance = length(lightdir_no_normal);
	vec3 lightdir = lightdir_no_normal / distance;

	vec3 refl = normalize(reflect(-lightdir, nor));

	vec4 amb = gl_FrontLightProduct[0].ambient;
	vec4 dif = gl_FrontLightProduct[0].diffuse
		* max(dot(lightdir, nor), 0.0);
	vec4 spc = gl_FrontLightProduct[0].specular
		* pow(max(dot(refl, -pos3) / length(pos3), 0.0), 0.3) / (distance * distance);

	gl_FrontColor = amb + dif + spc;
	gl_Position = gl_ProjectionMatrix * pos;
}
