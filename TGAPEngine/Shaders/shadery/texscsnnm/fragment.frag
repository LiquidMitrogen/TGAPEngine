
#version 330
uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec3 lightCol[4];
uniform uint lightsNumber;
uniform uint pointLight[4];
uniform sampler2D texture1;
uniform sampler2D normalMap;
uniform sampler2D specTexMap;
uniform sampler2D shininessMap;
//varying float colorGreen;

layout(location = 0) out vec4 colorOut;
in vec4 normal;
in vec4 positionCS;
in vec4 lightDirCS[4];
in vec3 tangent;
in vec3 bitangent;
in vec2 tcoords;


in vec3 lightDirTS[4];
in vec3 eyeDirTS;

in mat3 TS;


void main(){

	vec4 diff = normalize(texture( texture1, tcoords ).rgba);
	float shininess = texture( shininessMap, tcoords ).r * 128.0f;
	vec4 specular = texture( specTexMap, tcoords );
	vec3 eyeDir = vec3(0.0f,1.0f,0.0f);
	vec3 n;
	n = normalize(texture( normalMap, tcoords ).rgb*2.0 - 1.0);
	colorOut = vec4(0.0f,0.0f,0.0f,1.0f);
	
	
	for(uint i=0u;i<lightsNumber;i++){
		vec3 lightDirection;
		if(pointLight[i] == 0u){
			lightDirection = normalize(lightDirTS[i].xyz);
		}else{
			vec4 tmp = lightDirCS[i] - positionCS;
			lightDirection = normalize(TS * tmp.xyz);
		}
		

		float NdotL;
		
		NdotL = max(dot(n,lightDirection.xyz),0.0f);
		colorOut.xyz += diff.xyz * 2 * NdotL;
		float NdotH = max(dot(n,normalize(lightDirection.xyz + eyeDir)),0.0f);
		//colorOut.xyz += specular.xyz * lightCol[i] * pow(NdotH,shininess);
		colorOut.xyz += ambient.xyz * lightCol[i];
	}
}




