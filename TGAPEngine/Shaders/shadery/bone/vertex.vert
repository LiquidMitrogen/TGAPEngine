#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec4 weights;
layout(location = 3) in uvec4 indices;
layout(location = 4) in vec2 texcoords;
layout(location = 5) in vec3 tangentIn;
layout(location = 6) in vec3 bitangentIn;

uniform mat4 worldToCamera;
uniform mat4 cameraToClip;
uniform mat4 modelToWorld;
//uniform vec4 lightDir;
uniform vec4 lightDir[4];
uniform uint lightsNumber;
uniform mat4 bone[14];


out vec2 tcoords;
out vec4 normal;
out vec4 positionCS;
out vec4 lightDirCS[4];

out vec3 tangent;
out vec3 bitangent;


out vec3 lightDirTS[4];
out vec3 eyeDirTS;

out mat3 TS;



void main()
{

//vec4 normalizedWeights = normalize(weights);
    //vec4 pos = position;
    vec4 normalO =  vec4(normalize(normalIn),0.0f);
    normal = (bone[indices[0]] * normalO) * weights[0];
 vec4 pos = (bone[indices[0]] * position) * weights[0];
int i;
//boneColor = vec4(0,0,0,1);
for(i=1;i<4;i++){
        pos = (bone[indices[i]] * position) * weights[i] + pos;
        normal = (bone[indices[i]] * normalO) * weights[i] + normal;
        //boneColor.x = weights[i];

    }
    //normal = normalize(matrix * normal);
    //gl_Position =  matrix * pos;
    //colorGreen = - pos.z + 1;
    //if((indices[1] == 13) &&(weights[0] > 0.0f)){
    //boneColor.x = 1.0f;
    //}
   // boneColor.y = 0.5f;
	
	
	normal =  worldToCamera * modelToWorld *  normalize(normal);
	positionCS = worldToCamera * modelToWorld  * pos;
    gl_Position =  cameraToClip * positionCS;
	mat3 cameraToTangent = transpose(mat3(
		tangent.xyz,
		bitangent.xyz,
		normal.xyz
 ));
    for(uint i = 0u;i<lightsNumber;i++){
			lightDirCS[i] = worldToCamera * lightDir[i];
			lightDirTS[i] = cameraToTangent * lightDirCS[i].xyz;
		}
    tangent = (worldToCamera * modelToWorld * vec4(tangentIn,0.0f)).xyz;
    bitangent = (worldToCamera * modelToWorld * vec4(bitangentIn,0.0f)).xyz;
	tcoords = texcoords;
	

	eyeDirTS = cameraToTangent * vec3(0.0f,0.0f,1.0f);
	TS = cameraToTangent;
}
