#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core

uniform float red;
uniform float green;
uniform float blue;
uniform bool disco;
out vec4 FragColor;

void main()
{
    if (disco)
        FragColor = vec4(red,green,blue,1.0);
    else
        FragColor = vec4(1.0);
}