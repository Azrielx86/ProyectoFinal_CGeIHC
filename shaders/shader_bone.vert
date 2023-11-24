#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 vColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 color;
uniform vec2 toffset;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1)
            continue;
        if(boneIds[i] >= MAX_BONES)
        {
            totalPosition = vec4(pos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(pos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * norm;
    }

    gl_Position = projection * view * model * vec4(pos, 1.0);
    vCol = vec4(0.0, 1.0, 0.0, 1.0f);
    vColor=vec4(color,1.0f);
    TexCoord = tex+toffset;
    //para tomar las Transformaciones geom�tricas y la transpuesta es para que las escalas se inviertan en caso de escalas no uniformes en las normales
    Normal = mat3(transpose(inverse(model))) * norm;

    FragPos = (model * vec4(pos, 1.0)).xyz; //para obtener un vec3 s�lo posici�n con respecto al origen en orden xyz: Swizzling 
}