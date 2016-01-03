#version 330
uniform vec4 color;
uniform vec4 lightDir;
uniform vec4 lightDir2;
varying vec4 boneColor;
varying vec4 coords;
uniform vec4 toCameraDir;
uniform mat4 matrixToCamera;
uniform mat4 matrixToLight;
uniform sampler2D texture1;
uniform bool smbool;
layout(location = 0) out vec4 colorOut;
in vec4 normal;


void main(){







    vec3 lightDirection = normalize(toCameraDir.xzy);
    lightDirection.z = -lightDirection.z;
    vec3 n;
    float NdotL;
    float NdotL2;

    colorOut = vec4(0.12,0.12,0.12,1.0);
    vec4 diffuse = vec4(0.7f,7.0f,0.7f,1.0f);



    n = normalize(normal.xyz);
    //NdotL = max(dot(n,lightDirection),0.0);
    NdotL = dot(n,lightDirection);


    if(NdotL < 0) NdotL = -NdotL;
    colorOut += diffuse * NdotL;

    while(colorOut.g > 1.0f){
    colorOut += -1;
    }
    while(colorOut.a < 0.0f){
    colorOut.a += 1;
    }
}
