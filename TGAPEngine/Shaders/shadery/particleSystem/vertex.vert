
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 posize;

uniform mat4 worldToCamera;
uniform mat4 cameraToClip;
uniform mat4 modelToWorld;
uniform vec4 lightDir;
//uniform mat4 bone[14];
//varying float colorGreen;
//varying vec4 boneColor;
out vec4 normal;
out vec4 positionCS;
out vec4 lightDirCS;
out vec3 tangent;
out vec3 bitangent;
out vec2 tcoords;

void main()
{

    //normal = matrix * vec4(normalize(normalIn),0.0f);
    //normal =  worldToCamera * modelToWorld *  vec4(normalize(normalIn.xyz),0.0f);
    //normal =  modelToWorld *  vec4(normalize(normalIn.xyz),0.0f);
	vec4 pos = vec4(position.x * posize.w + posize.x,position.y * posize.w + posize.y,position.z * posize.w + posize.z,1.0f);
	positionCS = worldToCamera * modelToWorld  * pos;
    gl_Position =  cameraToClip * positionCS;
    //lightDirCS = worldToCamera * lightDir;
    //tangent = (worldToCamera * modelToWorld * vec4(tangentIn,0.0f)).xyz;
    //bitangent = (worldToCamera * modelToWorld * vec4(bitangentIn,0.0f)).xyz;
    //tangent = ( modelToWorld * vec4(tangentIn,0.0f)).xyz;
    //bitangent = ( modelToWorld * vec4(bitangentIn,0.0f)).xyz;
    //colorGreen = - position.z + 1;
	//tcoords = texcoords;
	tcoords = position.xy + 0.5;

}
