// Fragment Shader - file "patronus.frag"

#version 140

uniform bool UseTexture;
uniform bool UseBumped;
uniform sampler2D CurrentTex;
uniform sampler2D CurrentNMM;  

varying vec3 normal, CameraDir, lightDir;
varying vec2 TexCoord;

void main()
{           
	float NdotL;
	float NdotHV;

	//Color
	//vec4 color = vec4(0/255.0 , 204/255.0, 153/255.0, 0.0f);
	vec4 color = vec4 (1.0f);

	if (UseTexture){
		vec4 TexColor = texture2D(CurrentTex,TexCoord);
		if(TexColor.a <= 0.1f) 
			discard;
	}

	NdotL =  dot(normal,CameraDir);

	if(NdotL < 0.0f) 
		discard;

	color.a = 1 - NdotL;

	if (UseBumped){

		//NormalMap
		vec3 N = normalize(normal);

		N = (texture2D (CurrentNMM, TexCoord)).rgb;
		N = normalize(N * 2.0 - 1.0);
		
		NdotL = max( dot(N,lightDir), 0.0);
		vec4 specColor = gl_FrontMaterial.specular * gl_LightSource[0].specular;

		vec4 lightcolor;

		if( NdotL > 0.0 ){
			NdotHV = max( dot(N, gl_LightSource[0].halfVector.xyz), 0.0f);
				
			specColor *= pow(NdotHV, gl_FrontMaterial.shininess) ;
	
			lightcolor = color;
			lightcolor.a += specColor.a * 15.0f;
		}
	
		gl_FragColor = lightcolor;

	}
	else{

		gl_FragColor = color;

	}
}
