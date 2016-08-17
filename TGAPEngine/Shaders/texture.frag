#version 330
layout(location = 0) out vec4 colorOut;
in vec2 texCoords;
uniform sampler2D texture1;

void main(){

        vec2 tex;
        tex.x = texCoords.x;
        tex.y = 0.0;
        float val = texture(texture1,tex).r;
        if(texCoords.y > val - 0.01 && texCoords.y < val + 0.01){
            colorOut.r = 1;
            colorOut.g = 0;
            colorOut.a = 1.0;
        }else{
            colorOut = vec4(1.0,1.0,1.0,0.0);
        }
        //colorOut.a = 1.0;

}
