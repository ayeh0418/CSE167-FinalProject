
/*
struct Light {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

main:
vec3 I = normalize(posOutput - viewPos);
vec3 norm = normalize(normalOutput);
vec3 R = reflect(I, norm);
vec3 textured = texture(skybox, R).rgb;

vec3 ambient = light.ambient * textured;

vec3 lightDir = normalize(-light.direction);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = light.diffuse * (diff * textured);

// specular
vec3 viewDir = normalize(viewPos - normalOutput);
vec3 reflectDir = reflect(-lightDir, norm);  
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
vec3 specular = light.specular * (spec * textured);  

vec3 result = ambient + diffuse + specular;
fragColor = vec4(result, 1.0);
*/

#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Light {
	vec3 lightPos;
	vec3 lightColor;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

uniform Light light;


uniform vec3 viewPos;
uniform samplerCube skybox;


void main()
{         
	
	vec3 I = normalize(fs_in.FragPos - viewPos);
	vec3 norm = normalize(fs_in.Normal);
	vec3 R = reflect(I, norm);
	vec3 textured = texture(skybox, R).rgb;

	vec3 ambient = vec3(0.15f, 0.15f, 0.15f) * textured;

	vec3 lighting = vec3(0.0);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 lightDir = normalize(light.lightPos - fs_in.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.lightColor * (diff * textured);
	float dist = length(fs_in.FragPos - light.lightPos);
	diffuse *= 1.0 / (dist * 0.8);
	lighting += diffuse;

	vec3 result = ambient + lighting;
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.3) BrightColor = vec4(result, 1.0);
	else BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

	//vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
