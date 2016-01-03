#version 330
uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform vec4 lightDir;
uniform float shininess;
uniform uint pointLight;
uniform sampler2D texture1;
//varying float colorGreen;

layout(location = 0) out vec4 colorOut;
in vec4 normal;
in vec4 positionCS;
in vec4 lightDirCS;
in vec2 texcoords;
in vec3 bitangent;
in vec4 pos;



void main(){
	
	float val = texture(texture1,vec2(texcoords.x,1-texcoords.y)).r;
	//vec4 sample1 = texture(texture1,vec2(texcoords.x,1-texcoords.y));
	//float val = texture(texture1,vec2(pos.x / 2 + 0.5, -(pos.y / 2 + 0.5))).r;
	//vec3 lightDirection;
	//if(pointLight == 0u){
		//lightDirection = normalize(lightDirCS.xyz);
	//}else{
		//vec4 tmp = lightDirCS - positionCS;
		//lightDirection = normalize(tmp.xyz);
	//}
	//vec3 eyeDir = vec3(0.0f,0.0f,1.0f);
    //vec3 n;
	//n = normalize(normal.xyz);
    //float NdotL;

	if(val <= 0.001f) discard;
    colorOut = vec4(ambient.xyz,val);
	//colorOut = sample1;

    //diffuse.x = colorGreen + diffuse.x;



	//lightDirection = normalize(vec4(0.0f,1.0f,0.0f,0.0f));
    //NdotL = max(dot(n,lightDirection.xyz),0.0f);
    //colorOut.xyz += diffuse.xyz * NdotL;
	//float NdotH = max(dot(n,normalize(lightDirection.xyz + eyeDir.xyz)),0.0f);
	//colorOut.xyz += specular.xyz * pow(NdotH,shininess);
	//vec3 tangent2 = abs(tangent.xyz);
	//vec3 bitangent2 = abs(bitangent.xyz);
	//vec3 normal2 = abs(normal.xyz);
	//colorOut.xyz += ambient.xyz;
	//colorOut.xyz =normalize(bitangent2.xyz + normal2.xyz + tangent2.xyz);
	//colorOut.xyz = bitangent2.xyz;
}


