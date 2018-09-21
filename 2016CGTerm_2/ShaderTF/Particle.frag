// Fragment Shader - file "Particle.frag"

#version 420 core

uniform sampler2D CurrentTex;
uniform bool useTex;

varying vec3 normal, lightDir;
varying vec2 TexCoord;

void main()
{   
	

	float r = sqrt((TexCoord.x-0.5) * (TexCoord.x-0.5) + (TexCoord.y-0.5) * (TexCoord.y-0.5));
	float l = 1 - 2 * r;

	if (l <= 0.0) l == 0.0;

	
	float max = 0.05; // 0.0 ~ 1.0
	float min = 0.0;

	vec4 color = vec4(1.0);
	
	vec3 rgb = vec3 ( 111/255.0 , 119/255.0, 165/255.0);

	color.rgb = rgb + ( 1 - rgb ) * l;
	color.a = l;
	
	if(useTex){

		//Bokeh
		max = 1.0;
		min = 0.0;
		
		color = texture2D (CurrentTex, TexCoord);
	//	color = vec4(1.0f);
	}

	if (color.a > max)
		color.a = max;
	if (color.a <= min){
		color.a = 0;
	}

	gl_FragColor = color;

}
