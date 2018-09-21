// Fragment Shader - file "alpha.frag"

#version 140

uniform sampler2D tColor;
uniform sampler2D tex0;
uniform sampler2D tex1;

uniform float translucency;

varying vec3 normal, lightDir;
varying vec2 TexCoord;

varying vec2 Tex0, Tex1;

void main(){

	

	float NdotL;
	float NdotHV;

	vec4 color = vec4(0.0);

	NdotL = max( dot(normal,lightDir), 0.0);

	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	vec4 diffuse = NdotL * (gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse);
	vec4 specColor = gl_FrontMaterial.specular * gl_LightSource[0].specular;

	
	vec4 alpha = texture2D (tColor, TexCoord);	
	if(alpha.a < 0.1)
		discard;
	
	diffuse *= alpha;

	if( NdotL > 0.0 ){
		NdotHV = max( dot(normal, gl_LightSource[0].halfVector.xyz), 0.0f);
		specColor *= pow(NdotHV, gl_FrontMaterial.shininess) ;
		
		color = ambient + diffuse + specColor;
	
	}

//	color.a = alpha.a*1.5;

	gl_FragColor = color;
	

	/*
	vec4 color = vec4(0.0);

	float f_src, f_destination;
	vec4 c_src, c_destination;

	c_src = texture2D(tex0, TexCoord);
	f_src = c_src.a;
	f_destination = 1 - f_src;
	c_destination = vec4(1.0);
	c_destination.a = 0.0f;
	
	// c_destination.a = 0.0f이면 검게나오는 ㅠㅠㅠ 문제 ㅠㅠㅠ


	color = c_src * f_src + c_destination * f_destination;
	

	if(color.a < 0.3f) 
		discard;

	gl_FragColor = color;
	*/

}

