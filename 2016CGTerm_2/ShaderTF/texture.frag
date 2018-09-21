// Fragment Shader - file "texture.frag"

#version 140

uniform sampler2D CurrentTex;

varying vec3 normal, lightDir;
varying vec2 TexCoord;

void main()
{           
/*	float NdotL;
	float NdotHV;

	vec4 color = vec4(0.0);

	NdotL = max( dot(normal,lightDir), 0.0);

	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	vec4 diffuse = NdotL * (gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse);
	vec4 specColor = gl_FrontMaterial.specular * gl_LightSource[0].specular;
	
	vec4 TexColor = texture2D (CurrentTex, TexCoord);	

	if( NdotL > 0.0 ){
		NdotHV = max( dot(normal, gl_LightSource[0].halfVector.xyz), 0.0f);
				
		specColor *= pow(NdotHV, gl_FrontMaterial.shininess) ;
		
		color = TexColor * (ambient + diffuse + specColor);
	}

	gl_FragColor = color;
	*/
	vec4 color = texture2D (CurrentTex, TexCoord);

	gl_FragColor = color;
}
