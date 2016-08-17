#version 330
uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shininess;
uniform vec3 lightCol[4];
uniform uint lightsNumber;
uniform uint pointLight[4];
uniform sampler2D texture1;

layout(location = 0) out vec4 colorOut;
in vec4 normal;
in vec4 positionCS;
in vec4 lightDirCS[4];



void main(){

	vec4 diff = diffuse;
	vec3 eyeDir = vec3(0.0f,1.0f,0.0f);
	vec3 n;
	n = normalize(normal.xyz);
	colorOut = vec4(0.0f,0.0f,0.0f,1.0f);
	
	
	for(uint i=0u;i<lightsNumber;i++){
		vec3 lightDirection;
		if(pointLight[i] == 0u){
			lightDirection = normalize(lightDirCS[i].xyz);
		}else{
			vec4 tmp = lightDirCS[i] - positionCS;
			lightDirection = normalize(tmp.xyz);
		}
		

		float NdotL;
		
		NdotL = max(dot(n,lightDirection.xyz),0.0f);
		colorOut.xyz += diff.xyz * lightCol[i] * NdotL;
		float NdotH = max(dot(n,normalize(lightDirection.xyz + eyeDir)),0.0f);
		colorOut.xyz += specular.xyz * lightCol[i] * pow(NdotH,shininess);
		colorOut.xyz += ambient.xyz * lightCol[i];
	}
}


