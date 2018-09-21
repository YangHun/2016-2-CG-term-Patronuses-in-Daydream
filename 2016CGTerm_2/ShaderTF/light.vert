//Vertex Shader - file "light.vert"

#version 140

/*	160518	*/

varying vec3 normal, lightDir;

varying vec2 TexCoord; // Texture coord



void main() {

	TexCoord  = gl_MultiTexCoord0.xy;
	
	lightDir = (gl_LightSource[0].position).xyz;
	normal = gl_NormalMatrix * gl_Normal;

	lightDir = normalize(lightDir);
	normal = normalize(normal);

	gl_Position = ftransform();

}

/*
varying float NdotL;

void main()
{
//	vector4 color = vec4(1.0);
//	gl_FragColor = color;

	vec4 l = gl_LightSource[0].position;
	vec3 n = gl_NormalMatrix * gl_Normal;

	NdotL = dot(l.xyz,n);
	gl_Position = ftransform();


}
*/

