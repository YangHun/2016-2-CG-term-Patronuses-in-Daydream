// Vertex Shader - "normal.vert"

#version 140

varying vec3 normal, lightDir;
varying vec2 TexCoord; // Texture Coord;

void main(){

	TexCoord = gl_MultiTexCoord0.xy;

	vec4 ecPos = gl_ModelViewMatrix * gl_Vertex;

	lightDir = normalize(gl_LightSource[0].position.xyz - ecPos.xyz);
	normal = normalize(gl_NormalMatrix * gl_Normal);

	lightDir = normalize(lightDir);
	normal = normalize(normal);

	gl_Position = ftransform();
}