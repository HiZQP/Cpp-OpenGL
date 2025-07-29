                    #shader vertex
                    #version 330 core
        
                    layout(location = 0) in vec3 position;
                    layout(location = 1) in vec3 normal;
                    layout(location = 2) in vec2 texCoord;
                    
                    uniform mat4 u_MVP;
                    uniform vec4 u_Ambient;
                    
                    out vec4 ambient;
                    out vec2 v_TexCoord;

                    void main() {
                        ambient = u_Ambient;
                        gl_Position = u_MVP * vec4(position, 1.0f);
                        v_TexCoord = texCoord;
                    }


                    #shader fragment
                    #version 330 core
                    out vec4 Fragcolor;

                    in vec4 ambient;
                    in vec2 v_TexCoord;

                    uniform sampler2D u_Texture_Diffuse1;

                    void main() {
                        vec4 scatteredLight = ambient;
                        vec4 texColor = texture(u_Texture_Diffuse1, v_TexCoord);
                        Fragcolor = min(texColor * ambient, vec4(1.0f));
                    }