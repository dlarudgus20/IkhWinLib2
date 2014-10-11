#version 110

varying vec3 v_pos3;
varying vec3 v_nor;

void main()
{
	// re-normalize
	vec3 nor = normalize(v_nor);

	vec3 lightdir_no_normal = vec3(gl_LightSource[0].position) - v_pos3;
	float lightdist = length(lightdir_no_normal);
	vec3 lightdir = lightdir_no_normal / lightdist;

	// ambient
	vec4 amb = gl_FrontLightProduct[0].ambient;

	// diffuse
	float dif_cos = max(dot(lightdir, nor), 0.0);
	vec4 dif = gl_FrontLightProduct[0].diffuse * dif_cos;

	// specular
	vec3 refl = normalize(reflect(-lightdir, nor));
	float spc_cos = max(dot(refl, -v_pos3) / length(v_pos3), 0.0);
	float spc_intensity = pow(spc_cos, 0.04 * gl_FrontMaterial.shininess);
	float spc_dist = lightdist / 128.0;
	vec4 spc = gl_FrontLightProduct[0].specular * spc_intensity / (spc_dist * spc_dist);

	gl_FragColor = amb + dif + spc;
}
