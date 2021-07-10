#version 330 core
out vec4 FragColor;

uniform float mixValue;
uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 FragPos;  
in vec3 Normal;  
uniform vec3 lightPos;  
uniform vec3 viewPos;



void main()
{
	float ambientStrength = 0.1;
	float specularStrength = 0.5;


	vec3 ambient = lightColor * ambientStrength;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(FragPos - lightPos);
	float diff = max(dot(norm, -lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specular = specularStrength * spec * lightColor;  


	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
