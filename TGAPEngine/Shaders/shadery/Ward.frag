#version 330
uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform vec4 lightDir;
uniform float shininess;

uniform float diffuseReflectance;
uniform float specularReflectance;
uniform float sigma;

//varying float colorGreen;

layout(location = 0) out vec4 colorOut;
in vec4 normal;
in vec4 lightDirCS;




void main(){
	const float PI = 3.14159;
    vec3 lightDirection = normalize(lightDirCS.xyz);
	vec3 eyeDir = vec3(0.0f,0.0f,1.0f);
    vec3 n;
	n = normalize(normal.xyz);
    float NdotL;

    colorOut = vec4(0.1,0.1,0.1,1.0);

    //diffuse.x = colorGreen + diffuse.x;

	halfvector = normalize(lightDirection.xyz + eyeDir.xyz);
	exponential = exp(-2)

	//lightDirection = normalize(vec4(0.0f,1.0f,0.0f,0.0f));
    NdotL = max(dot(n,lightDirection.xyz),0.0f);
    colorOut.xyz += diffuse.xyz * NdotL;
	float NdotH = max(dot(n,halfvector),0.0f);
	colorOut.xyz += specular.xyz * pow(NdotH,shininess);
	colorOut.xyz += ambient.xyz;
}


