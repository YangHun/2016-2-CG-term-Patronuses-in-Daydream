// Vertex Shader - "alpha.vert"

#version 140

varying vec3 normal, lightDir;
varying vec2 Tex0, Tex1, TexCoord;

void main(){

	TexCoord =gl_MultiTexCoord0.xy;
	
	Tex0 = gl_MultiTexCoord0.xy;
	//Tex0 = TexCoord;
	Tex1 = gl_TexCoord[1].st;


	lightDir = (gl_LightSource[0].position).xyz;
	normal = gl_NormalMatrix * gl_Normal;

	lightDir = normalize(lightDir);
	normal = normalize(normal);

	gl_Position = ftransform();

}