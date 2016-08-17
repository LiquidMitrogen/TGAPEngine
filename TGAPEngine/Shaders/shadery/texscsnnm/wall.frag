#version 330
uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform vec4 lightDir;
uniform float shininess;
uniform uint pointLight;
uniform sampler2D texture1;
uniform sampler2D normalMap;
//uniform sampler2D specTexMap;
//uniform sampler2D shininessMap;
//varying float colorGreen;

layout(location = 0) out vec4 colorOut;
in vec4 normal;
in vec4 positionCS;
in vec4 lightDirCS;
in vec3 tangent;
in vec3 bitangent;
in vec2 tcoords;


in vec3 lightDirTS;
in vec3 eyeDirTS;

in mat3 TS;



void main(){
	vec3 lightDirection;
	if(pointLight == 0u){
		lightDirection = normalize(lightDirTS.xyz);
	}else{
		vec4 tmp = lightDirCS - positionCS;
		lightDirection = normalize(TS * tmp.xyz);
	}
	vec3 eyeDir = normalize(eyeDirTS);
    vec3 n;
	//n = normalize(normal.xyz);
    float NdotL;
	n = normalize(texture( normalMap, tcoords * 3 ).rgb*2.0 - 1.0);
    colorOut = ambient;
	//vec4 diffuse = texture( texture1, tcoords );
	//vec4 specular = texture( specTexMap, tcoords );
	//float shininess = texture( shininessMap, tcoords ).r * 128.0f;
    //diffuse.x = colorGreen + diffuse.x;



	//lightDirection = normalize(vec4(0.0f,1.0f,0.0f,0.0f));
    NdotL = max(dot(n,lightDirection.xyz),0.0f);
    colorOut.xyz += diffuse.xyz * NdotL;
	float NdotH = max(dot(n,normalize(lightDirection.xyz + eyeDir.xyz)),0.0f);
	colorOut.xyz += specular.xyz * pow(NdotH,shininess);
	//vec3 tangent2 = abs(tangent.xyz);
	//vec3 bitangent2 = abs(bitangent.xyz);
	//vec3 normal2 = abs(normal.xyz);
	colorOut.xyz += ambient.xyz * diffuse.xyz;
	//colorOut.xyz =normalize(bitangent2.xyz + normal2.xyz + tangent2.xyz);
	//colorOut.xyz = lightDirection;
	colorOut.a = texture( texture1, tcoords * 3 ).a;
	if(colorOut.a < 0.01f) discard;
}


