#version 330 core
out vec4 FragColor;

uniform float mixValue;
uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 FragPos;  
in vec3 Normal;  
uniform vec3 lightPos;  
uniform vec3 viewPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
    float shininess;
}; 
  
uniform Material material;

in vec2 TexCoords;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  


void main()
{    
    // ambient
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	vec3 emission = texture(material.emission, TexCoords).rgb;
        
    vec3 result = ambient + diffuse + specular + emission;

    FragColor = vec4(result, 1.0);
}
