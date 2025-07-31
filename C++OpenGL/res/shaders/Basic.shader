#shader vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_FragPos;
out vec2 v_TexCoord;
out vec3 v_Normal;
out mat3 v_TBN;

void main() {
    v_FragPos = vec3(u_Model * vec4(position, 1.0));
    v_Normal = normalize(mat3(u_Model) * normal);
    v_TexCoord = texCoord;
    
    // 计算TBN矩阵
    vec3 T = normalize(mat3(u_Model) * tangent);
    vec3 B = normalize(mat3(u_Model) * bitangent);
    vec3 N = normalize(mat3(u_Model) * normal);
    v_TBN = mat3(T, B, N);
    
    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}

#shader fragment
#version 450 core
#define MAX_DIRECTIONAL_LIGHTS 32

struct DirectionalLight {
    vec4 color;
    vec4 direction;
};

layout(std140, binding = 0) uniform directionalLight {
    int directionalLightCount;
    DirectionalLight lights[MAX_DIRECTIONAL_LIGHTS];
};

uniform sampler2D u_Texture_Diffuse1;
uniform sampler2D u_Texture_Normal1;
uniform vec3 u_CameraPos;
uniform vec4 u_Ambient;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in mat3 v_TBN;

out vec4 Fragcolor;

void main() {
    vec4 texColor = texture(u_Texture_Diffuse1, v_TexCoord);
    // 从法线贴图获取切线空间法线 [0,1] -> [-1,1]
    vec3 normalMap = texture(u_Texture_Normal1, v_TexCoord).rgb;
    normalMap = normalize(normalMap * 2.0 - 1.0);
    vec3 map_Nnormal = normalize(v_TBN * normalMap);
    
    vec3 viewDir = normalize(u_CameraPos - v_FragPos);
    
    vec4 totalDiffuse = vec4(0.0);
    vec4 totalSpecular = vec4(0.0);
    float shininess = 64.0;
    
    for (int i = 0; i < directionalLightCount; i++) {
        vec3 lightDir = normalize(-lights[i].direction.xyz);
        
        // 漫反射
        float diff = max(dot(map_Nnormal, lightDir), 0.0);
        totalDiffuse += diff * lights[i].color * texColor;
        
        // 镜面反射 (Blinn-Phong)
        vec3 halfDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(map_Nnormal, halfDir), 0.0), shininess);
        totalSpecular += spec * lights[i].color;
    }
    
    Fragcolor = texColor * u_Ambient + totalDiffuse + totalSpecular;
}
