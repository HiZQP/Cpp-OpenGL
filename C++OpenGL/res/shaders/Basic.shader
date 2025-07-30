                    #shader vertex
                    #version 450 core
        
                    layout(location = 0) in vec3 position;
                    layout(location = 1) in vec3 normal;
                    layout(location = 2) in vec2 texCoord;
                    
                    uniform mat4 u_MVP;
                    uniform vec4 u_Ambient;
                    
                    out vec4 ambient;
                    out vec2 v_TexCoord;
                    out vec3 v_Normal;

                    void main() {
                        v_Normal = normal;
                        ambient = u_Ambient;
                        gl_Position = u_MVP * vec4(position, 1.0f);
                        v_TexCoord = texCoord;
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
                        //int padding[3];// 狗日的std140不管你有没有显式对齐内存都会强制内存对齐，浪费劳资几个小时搞UBO，看了RenderDoc才发现
                        DirectionalLight lights[MAX_DIRECTIONAL_LIGHTS];
                    };

                    out vec4 Fragcolor;

                    in vec4 ambient;
                    in vec2 v_TexCoord;
                    in vec3 v_Normal;

                    uniform sampler2D u_Texture_Diffuse1;

                    void main() {
                        vec4 texColor = texture(u_Texture_Diffuse1, v_TexCoord);
                        vec4 totalDiffuse = vec4(0.0, 0.0, 0.0, 1.0);
                        for (int i; i < directionalLightCount; i++){
                            vec3 lightDir = normalize(-lights[i].direction.xyz);
                            float diff = max(dot(v_Normal, lightDir), 0.0);
                            totalDiffuse += diff * lights[i].color * texColor;
                        }
                        Fragcolor = texColor * ambient + totalDiffuse;
                    }