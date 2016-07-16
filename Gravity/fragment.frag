// Copyright (c) 2014, ÀÓ°æÇö
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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

	float lightdist_touched = lightdist / 178.0;
	float attenuation = 1.0 / (1.0 + 0.045 * lightdist_touched * lightdist_touched);

	// ambient
	vec4 amb = gl_FrontLightProduct[0].ambient;

	// diffuse
	float dif_cos = max(dot(lightdir, nor), 0.0);
	vec4 dif = gl_FrontLightProduct[0].diffuse * dif_cos;

	// specular
	vec3 refl = normalize(reflect(-lightdir, nor));
	float spc_cos = max(dot(refl, -v_pos3) / length(v_pos3), 0.0);
	float spc_intensity = pow(spc_cos, 0.04 * gl_FrontMaterial.shininess);
	vec4 spc = gl_FrontLightProduct[0].specular * spc_intensity;

	vec4 color = amb + (dif + spc) * attenuation;
	// gamma correction
	gl_FragColor = vec4(pow(vec3(color), vec3(1.0/2.2)), color.a);
}
