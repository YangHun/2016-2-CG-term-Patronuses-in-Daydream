// Fragment Shader - file "normal.frag"

#version 140

uniform sampler2D CurrentTex;
uniform sampler2D CurrentNMM;  
uniform bool useNormalMap;

varying vec3 normal, lightDir;
varying vec2 TexCoord;

void main()
{           
	vec3 N = normalize(normal);

	if(useNormalMap){

		N = (texture2D (CurrentNMM, TexCoord)).rgb;
		N = normalize(N * 2.0 - 1.0);

	}

	float NdotL;
	float NdotHV;

	vec4 color = vec4(0.0);

	NdotL = max( dot(N,lightDir), 0.0);

	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	vec4 diffuse = NdotL * (gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse);
	vec4 specColor = gl_FrontMaterial.specular * gl_LightSource[0].specular;
	
	vec4 TexColor = texture2D (CurrentTex, TexCoord);	

	if(TexColor.a < 0.1)
		discard;

	if( NdotL > 0.0 ){
		NdotHV = max( dot(N, gl_LightSource[0].halfVector.xyz), 0.0f);
				
		specColor *= pow(NdotHV, gl_FrontMaterial.shininess) ;
		
		vec4 Specular = vec4(0.0);

		if (useNormalMap){
			Specular = texture2D(CurrentNMM, TexCoord) * pow( NdotHV, gl_FrontMaterial.shininess);
			color = ambient + TexColor * diffuse + specColor * 15.0;
		}
		else{
			color = TexColor * (ambient + diffuse + Specular* 15.0);
		}
		
	}

	gl_FragColor = color;

}
