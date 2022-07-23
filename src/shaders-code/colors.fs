#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{
    // ambient part
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse part
    vec3 normal = normalize(Normal);
    vec3 lightdir = normalize(lightPos-FragPos);
    float diff = max(dot(normal, lightdir), 0.0);
    vec3 diffuse = diff * lightColor;
    // now specular part
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightdir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (diffuse + ambient + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}