#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec4 weights;
layout(location = 3) in uvec4 indices;
uniform mat4 matrixToCamera;
uniform mat4 matrixToLight;
uniform bool smbool;
uniform mat4 bone[14];
varying vec4 coords;
varying vec4 boneColor;
out vec4 normal;
void main()
{
//vec4 normalizedWeights = normalize(weights);
    //vec4 pos = position;
    vec4 normalO =  vec4(normalize(normalIn),0.0f);
    normal = (bone[indices[0]] * normalO) * weights[0];
 vec4 pos = (bone[indices[0]] * position) * weights[0];
int i;
boneColor = vec4(0,0,0,1);
for(i=1;i<4;i++){
        pos = (bone[indices[i]] * position) * weights[i] + pos;
        normal = (bone[indices[i]] * normalO) * weights[i] + normal;
        //boneColor.x = weights[i];

    }
        normal = normalize(matrixToCamera * normal);
        if(smbool == true){
        gl_Position = matrixToLight * pos;
        }else{
        gl_Position =  matrixToCamera * pos;
        coords =  matrixToLight * pos;
        }
    if((indices[1] == 13u) &&(weights[0] > 0.0f)){
    boneColor.x = 1.0f;
    }
    boneColor.y = 0.5f;
}
