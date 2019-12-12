
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

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} fs_in;

uniform Light light;
out vec4 fragColor;

//in vec3 normalOutput;
//in vec3 posOutput;

uniform int showShadows;
uniform vec3 viewPos;
uniform sampler2D shadowMap;
uniform samplerCube skybox;


float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm, vec3 lightDir)
{
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);  
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{         
	vec3 result;
	if(showShadows == 1) {
		vec3 I = normalize(fs_in.FragPos - viewPos);
		vec3 norm = normalize(fs_in.Normal);
		vec3 R = reflect(I, norm);
		vec3 textured = texture(skybox, R).rgb;
		vec3 lightPos = vec3(-20.0f, 0.0f, 20.0f);

		vec3 ambient = light.ambient * textured;

		vec3 lightDir = normalize(lightPos - fs_in.FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * (diff * textured);

		vec3 viewDir = normalize(viewPos - fs_in.FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
		vec3 specular = light.specular * (spec * textured);  

		//calculate shadow
		float shadow = ShadowCalculation(fs_in.FragPosLightSpace, norm, lightDir);
		//vec3 result = ambient + diffuse + specular;
		result = (ambient + (1.0 - shadow) * (diffuse + specular)) * textured;
	}
	else {
		vec3 I = normalize(fs_in.FragPos - viewPos);
		vec3 norm = normalize(fs_in.Normal);
		vec3 R = reflect(I, norm);
		vec3 textured = texture(skybox, R).rgb;

		vec3 ambient = light.ambient * textured;

		vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * (diff * textured);

		vec3 viewDir = normalize(viewPos - fs_in.Normal);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
		vec3 specular = light.specular * (spec * textured);  

		result = ambient + diffuse + specular;
	}
	fragColor = vec4(result, 1.0);
}
