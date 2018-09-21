// Vertex Shader - "patronus.vert"

#version 140

uniform vec3 CameraPos;
uniform mat4 modelMat;

varying vec3 normal, CameraDir,lightDir;
varying vec2 TexCoord; // Texture Coord;

void main(){

	TexCoord = gl_MultiTexCoord0.xy;

	vec4 ecPos = modelMat * gl_Vertex; //vertex

	normal = normalize(gl_NormalMatrix * gl_Normal) ;

	CameraDir = normalize( ecPos.xyz * -1.0f);
	lightDir = normalize(gl_LightSource[0].position.xyz - ecPos.xyz);

//	CameraDir = normalize ( vec3 ( 0.0, 0.0, 1.0));


	gl_Position = ftransform();
}