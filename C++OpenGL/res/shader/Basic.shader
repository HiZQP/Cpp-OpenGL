        #shader vertex
        #version 330 core
        
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 color;

        out vec3 v_color;

        void main() {
           gl_Position = vec4(position, 1.0);
           v_color = color;
        };


        #shader fragment
        #version 330 core
        
        in vec3 v_color;

        out vec4 fragColor;

        void main() {
           fragColor = vec4(v_color, 1.0);
        };