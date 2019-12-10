#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

struct Material{
    sampler2D texture;
    float shininess;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform PointLight pointLights[4];
uniform Material material;
uniform vec3 viewPos;
uniform float red;
uniform float green;
uniform float blue;
uniform bool disco;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);    

    vec3 result = vec3(0.0);
     for (int i = 0; i < 4; ++i)
         result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

/*========================================================================================*/
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance      = length(light.position - fragPos);
    float attenuation   = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 ambient    = light.ambient * vec3(texture(material.texture, TexCoords));
    vec3 diffuse    = light.diffuse * diff * vec3(texture(material.texture, TexCoords));
    vec3 specular   = light.specular * spec * vec3(texture(material.texture, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    if(disco)
        return (vec3(red,green,blue) * (ambient + diffuse + specular));
    else
        return (ambient + diffuse + specular);
}