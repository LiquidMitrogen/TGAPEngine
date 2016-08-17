#version 330
uniform vec4 color;
uniform vec4 lightDir;
varying float colorGreen;
varying vec4 boneColor;
layout(location = 0) out vec4 colorOut;
in vec4 normal;
in vec2 texCoords;
uniform sampler2D texture1;

void main(){

    vec4 texColor;
    texColor = texture(texture1,texCoords);
    vec4 lightDirection = normalize(lightDir);
    vec3 n;
    float NdotL;

    colorOut = vec4(0.1,0.1,0.1,1.0);//ambient
    vec4 ambient = texColor * 0.1;
    vec4 diffuse = color * texColor;
    diffuse = normalize(diffuse);


    n = normalize(normal.xyz);
    NdotL = max(dot(n,lightDirection.xyz),0.0);
    if (NdotL > 0.0) {
        colorOut += diffuse * NdotL;
    }
    //colorOut = boneColor;
}
