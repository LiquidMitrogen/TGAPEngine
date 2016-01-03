#version 330
uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform vec4 lightDir;
uniform float shininess;
uniform uint pointLight;
//varying float colorGreen;

layout(location = 0) out vec4 colorOut;
in vec4 normal;
in vec4 positionCS;
in vec4 lightDirCS;
in vec3 tangent;
in vec3 bitangent;




void main(){
	vec3 lightDirection;
	if(pointLight == 0u){
		lightDirection = normalize(lightDirCS.xyz);
	}else{
		vec4 tmp = lightDirCS - positionCS;
		lightDirection = normalize(tmp.xyz);
	}
	vec3 eyeDir = vec3(0.0f,0.0f,1.0f);
    vec3 n;
	n = normalize(normal.xyz);
    float NdotL;

    colorOut = vec4(0.1,0.1,0.1,1.0);

    //diffuse.x = colorGreen + diffuse.x;



	//lightDirection = normalize(vec4(0.0f,1.0f,0.0f,0.0f));
    NdotL = max(dot(n,lightDirection.xyz),0.0f);
    colorOut.xyz += diffuse.xyz * NdotL;
	float NdotH = max(dot(n,normalize(lightDirection.xyz + eyeDir.xyz)),0.0f);
	colorOut.xyz += specular.xyz * pow(NdotH,shininess);
	//vec3 tangent2 = abs(tangent.xyz);
	//vec3 bitangent2 = abs(bitangent.xyz);
	//vec3 normal2 = abs(normal.xyz);
	colorOut.xyz += ambient.xyz;
	//colorOut.xyz =normalize(bitangent2.xyz + normal2.xyz + tangent2.xyz);
	//colorOut.xyz = bitangent2.xyz;
}


