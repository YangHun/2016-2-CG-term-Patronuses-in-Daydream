// Fragment Shader - file "light.frag"

#version 140


uniform sampler2D;

varying vec3 normal, lightDir;

varying vec2 TexCoord; // Texture coord

void main() {

	float NdotL;
	float NdotHV;

	vec4 color = vec4(0.0);

	NdotL = max( dot(normal,lightDir), 0.0);

	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	vec4 diffuse = NdotL * (gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse);
	vec4 specColor = gl_FrontMaterial.specular * gl_LightSource[0].specular;

	diffuse *= texture2D (tColor, TexCoord);
	//예제코드에는 diffuse에만 texture2D를 곱하는데 왜 다른데에는 안 곱함?
//	ambient *= texture2D (tColor, TexCoord); 
//	specColor *= texture2D (tColor, TexCoord);

	if( NdotL > 0.0 ){
		NdotHV = max( dot(normal, gl_LightSource[0].halfVector.xyz), 0.0f);
		specColor *= pow(NdotHV, gl_FrontMaterial.shininess) ;
		
		color = ambient + diffuse + specColor;
	
	}
	
	gl_FragColor = color;
}

/*
varying float NdotL;

void main()
{
	vec4 color = vec4(1.0);
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	vec4 diffuse =  NdotL * (gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse);
		//0번 Light의 ambient 값
	gl_FragColor = ambient + diffuse;
}
*/


