#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 viewPos;

// Directional light
uniform vec3 lightDir;

// Blue point light
uniform vec3 pointLightPos;

void main()
{
    
    vec3 norm     = normalize(Normal);
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 texColor = texture(texture1, TexCoord).rgb;

    // Directional light (yellow)
    vec3  yellowDir = vec3(1.0, 0.85, 0.2);
    float diff      = max(dot(norm, -lightDir), 0.0);
    vec3  ambient   = 0.15 * yellowDir * texColor;
    vec3  diffuse   = diff * yellowDir * texColor;

    // Specular
    vec3  halfDir  = normalize(-lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), 32.0);
    vec3  specular = 0.3 * spec * yellowDir * texColor;

    vec3 dirContrib = ambient + diffuse + specular;

    // Point light (blue)
    vec3  toLight   = pointLightPos - FragPos;
    float dist      = length(toLight);
    vec3  lightDir2 = normalize(toLight);

    // Attenuation
    float attenuation = 1.0 / (1.0 + 0.07 * dist + 0.017 * dist * dist);

    float diff2        = max(dot(norm, lightDir2), 0.0);
    vec3  blueCol      = vec3(0.2, 0.2, 1.0);
    vec3  pointContrib = diff2 * blueCol * texColor * attenuation;

    FragColor = vec4(dirContrib + pointContrib, 1.0);
}