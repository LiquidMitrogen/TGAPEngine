
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
uniform vec4 lightDir[4];
uniform uint lightsNumber;
//uniform mat4 bone[14];
//varying float colorGreen;
//varying vec4 boneColor;

out vec4 normal;
out vec4 positionCS;
out vec4 lightDirCS[4];

out vec3 tangent;
out vec3 bitangent;
out vec2 tcoords;

out vec3 lightDirTS[4];
out vec3 eyeDirTS;

out mat3 TS;


void main()
{

    normal =  worldToCamera * modelToWorld *  vec4(normalize(normalIn.xyz),0.0f);
	positionCS = worldToCamera * modelToWorld  * position;
    gl_Position =  cameraToClip * positionCS;
	
    tangent = (worldToCamera * modelToWorld * vec4(tangentIn,0.0f)).xyz;
    bitangent = (worldToCamera * modelToWorld * vec4(bitangentIn,0.0f)).xyz;
	tcoords = texcoords;
	
	mat3 cameraToTangent = transpose(mat3(
		tangent.xyz,
		bitangent.xyz,
		normal.xyz
 ));
	 for(uint i = 0u;i<lightsNumber;i++){
			lightDirCS[i] = worldToCamera * lightDir[i];
			lightDirTS[i] = cameraToTangent * lightDirCS[i].xyz;
		}
	eyeDirTS = cameraToTangent * vec3(0.0f,0.0f,1.0f);
	
	TS = cameraToTangent;

}
