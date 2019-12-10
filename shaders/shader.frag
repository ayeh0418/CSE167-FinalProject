
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
struct Light {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

uniform Light light;
out vec4 fragColor;

in vec3 normalOutput;
in vec3 posOutput;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{             
//    vec3 I = normalize(posOutput - viewPos);
//    vec3 R = reflect(I, normalize(normalOutput));
//	vec3 textured = texture(skybox, R).rgb;
//    fragColor = vec4(textured, 1.0);
	vec3 I = normalize(posOutput - viewPos);
	vec3 norm = normalize(normalOutput);
	vec3 R = reflect(I, norm);
	vec3 textured = texture(skybox, R).rgb;

	vec3 ambient = light.ambient * textured;

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * textured);

	vec3 viewDir = normalize(viewPos - normalOutput);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = light.specular * (spec * textured);  

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}
